module PIXEL_GRABBER(
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
parameter signed indicator_size = 10;
///////////////////////////////////////////////////////
logic signed [15:0] x_pixel, y_pixel;
logic [7:0]   red, green, blue;
logic [7:0]   red_out, green_out, blue_out;

logic sop, eop, in_valid, out_ready;
logic gate_enabled;

logic[31:0] grab_point, grab_value;
logic grab_indicator;
// Addresses
////////////////////////////////////////////////////////////////////////

logic packet_video, x_indicator, y_indicator;

always @(*) begin
	x_indicator = (x_pixel >= ($signed(grab_point[31:16]) - indicator_size)) && (x_pixel <= ($signed(grab_point[31:16]) + indicator_size));
	y_indicator = (y_pixel >= ($signed(grab_point[15:0]) - indicator_size)) && (y_pixel <= ($signed(grab_point[15:0]) + indicator_size));
	
	if (~sop & packet_video) begin
		if(y_indicator && x_indicator && grab_indicator ) begin
				{red_out, green_out, blue_out} = {24'hffffff};
			end
		else begin
			{red_out, green_out, blue_out} = {red,green,blue};
		end
	end else begin
		{red_out, green_out, blue_out} = {red,green,blue};
	end 
end

/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define GRAB_POINT					0
`define GRAB_VALUE						1
`define GRAB_INDICATOR				2



// Copy the requested word to the output port when there is a read.
always @ (posedge clk) begin
	if (~reset_n) begin
		x_pixel <= 0;
		y_pixel <= 0;
		grab_value <= 0;
		
		grab_point <= {16'h140, 16'h0F0 };
		grab_indicator <= 0;
		
		
		//grab_point <= {16'd20, 16'd6 };
		//grab_indicator <=1;
		
		
	end else if (sop) begin
			x_pixel <= 0;
			y_pixel <= 0;	
			packet_video <= (blue[3:0] == 3'h0);

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
	
	//Pixel grabbing
	if (x_pixel == (grab_point[31:16]) && (y_pixel == grab_point[15:0])&& in_valid) begin
		grab_value <= {8'b0, red, green, blue};
	end
	
   if (~reset_n) begin
	   s_readdata <= {32'b0};
	end
	
	else if (s_chipselect & s_read) begin
		//Reads
		if	 (s_address == `GRAB_POINT) s_readdata <= grab_point;
		if	 (s_address == `GRAB_VALUE) s_readdata <= grab_value;
		if	 (s_address == `GRAB_INDICATOR) s_readdata <= grab_indicator;
	end

	else if (s_chipselect & s_write) begin
		if (s_address == `GRAB_POINT) grab_point <= s_writedata;
		if (s_address == `GRAB_INDICATOR) grab_indicator <= s_writedata;
	end
end

//Streaming registers to buffer video signal
STREAM_REG_GRAB #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red, green, blue, sop, eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG_GRAB #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red_out, green_out, blue_out, sop, eop})
);


endmodule
