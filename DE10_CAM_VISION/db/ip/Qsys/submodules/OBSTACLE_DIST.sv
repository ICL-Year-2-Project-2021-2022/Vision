module OBSTACLE_DIST(
	// global clock & reset
	input logic clk,
	input logic reset_n,
	
	// mm slave
	input logic s_chipselect,
	input logic s_read,
	input logic s_write,
	output logic[31:0] s_readdata,
	input logic[31:0] s_writedata,
	input logic[3:0] s_address,

	// stream sink
	input logic[23:0] sink_data,
	input logic sink_valid,
	output logic sink_ready,
	input logic sink_sop,
	input logic sink_eop,
	
	// streaming source
	output logic[23:0] source_data,
	output logic source_valid,
	input logic source_ready,
	output logic source_sop,
	output logic source_eop

);
///////////////////////////////////////////////////////
parameter width = 640;
parameter height = 480;
///////////////////////////////////////////////////////
logic signed [15:0] x_pixel, y_pixel;
logic [7:0]   red, green, blue;

logic sop, eop, in_valid, out_ready;
logic gate_enabled;

logic[31:0] grab_point, grab_value;
logic grab_indicator;

logic packet_video, y_indicator;

logic [10:0] y_level, stripe_count, stripe_pos_min, stripe_pos_max, potential_stripe_pos_min;
logic [10:0] stripe_count_d, stripe_dist_d, stripe_pos_min_d, stripe_pos_max_d, dead_zone, max_allowable, delta;
logic on_stripe, last_mask, mask;
logic greater_ok;


always @(*) begin
	if (y_pixel == y_level) y_indicator = 1;
	else y_indicator = 0;
	
	if(blue == 8'hff) mask = 1;
	else mask = 0;
	

	stripe_dist_d = stripe_pos_max_d - stripe_pos_min_d;
	greater_ok = (delta) > (stripe_pos_max - stripe_pos_min);
	//max_allowable =(((stripe_pos_max - stripe_pos_min) + (stripe_pos_max - stripe_pos_min)>>1) +10);
	max_allowable =((stripe_pos_max - stripe_pos_min)+(stripe_pos_max - stripe_pos_min)/2 + 10 );
	delta = (x_pixel - potential_stripe_pos_min);
	
	
end

/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define Y_LEVEL					0
`define STRIPE_COUNT				1
`define STRIPE_DIST				2
`define STRIPE_POS_MIN				3
`define STRIPE_POS_MAX			4
`define DEAD_ZONE 					5



// Copy the requested word to the output port when there is a read.
always @ (posedge clk) begin
	if (~reset_n) begin
		x_pixel <= 0;
		y_pixel <= 0;
		y_level <= 280;
		//y_level <= 1;
		
	end else if (sop) begin
			x_pixel <= 0;
			y_pixel <= 0;	
			packet_video <= (blue[3:0] == 4'h0);
			
			//Resets
			stripe_pos_min<=0;
			stripe_pos_max<=0;
			stripe_count <= 0;

	end else if (packet_video & ~sop) begin
		if (in_valid) begin
			if(x_pixel >= width-1) begin
				x_pixel <= 0;
				y_pixel <= y_pixel + 1;
			end else begin
				x_pixel <= x_pixel + 1;
			end			
		end		
		
	end
	
	//Actual counting logic
	last_mask <= mask;
	if(y_indicator & mask & ~last_mask & ((potential_stripe_pos_min - x_pixel) > dead_zone ))begin
	
		stripe_count <= stripe_count + 1;
		potential_stripe_pos_min <= x_pixel;
		
		
		if((stripe_pos_min == 0) && (stripe_pos_max == 0))begin
			stripe_pos_min<= x_pixel;
			stripe_pos_max<= x_pixel;
		end else if (stripe_pos_min == stripe_pos_max)begin
			stripe_pos_max<= x_pixel;
			
		end else if(greater_ok && (delta < max_allowable)) begin
			stripe_pos_min <= potential_stripe_pos_min;
			stripe_pos_max <= x_pixel;
		end
	end
	
	if (packet_video && eop)begin
		stripe_count_d <= stripe_count;
		stripe_pos_max_d <= stripe_pos_max;
		stripe_pos_min_d <= stripe_pos_min;
	end
	
   if (~reset_n) begin
	   s_readdata <= {32'b0};
	   stripe_count<= 0;
	   stripe_count_d <= 0;
	   stripe_pos_min <= 0;
	   stripe_pos_min_d <= 0;
	   stripe_pos_max <= 0;
	   stripe_pos_max_d <= 0;
	   potential_stripe_pos_min <= 0;
	   dead_zone <= 7;
	end
	
	else if (s_chipselect & s_read) begin
	
		//Reads
		if	 (s_address == `Y_LEVEL) s_readdata <= y_level;
		if	 (s_address == `STRIPE_COUNT) s_readdata <= stripe_count_d;
		if	 (s_address == `STRIPE_DIST) s_readdata <= stripe_dist_d;
		if	 (s_address == `STRIPE_POS_MIN) s_readdata <= stripe_pos_min_d;
		if	 (s_address == `STRIPE_POS_MAX) s_readdata <= stripe_pos_max_d;
		if	 (s_address == `DEAD_ZONE) s_readdata <= dead_zone;
	end

	else if (s_chipselect & s_write) begin
		if (s_address == `Y_LEVEL) y_level <= s_writedata;
		if	 (s_address == `DEAD_ZONE) dead_zone <= s_writedata;
	end
end

//Streaming registers to buffer video signal
STREAM_REG_DIST #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red, green, blue, sop, eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG_DIST #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red, green, blue, sop, eop})
);


endmodule
