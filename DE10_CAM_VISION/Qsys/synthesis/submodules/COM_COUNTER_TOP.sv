module COM_COUNTER_TOP(
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

reg [23:0]	threshold_gate;

wire [23:0] reg_data_out;
wire [31:0] ex_raw;
wire [31:0] ey_raw;
wire [31:0] mass;

//////////////////////////////////////////////////////////////////////////
parameter IMAGE_W = 11'd640;
parameter IMAGE_H = 11'd480;

logic [7:0]   red, green, blue, grey, mask;
logic [7:0]   red_out, green_out, blue_out;

logic         sop, eop, in_valid, out_ready;
logic gate_enabled;
////////////////////////////////////////////////////////////////////////

// Switch output pixels depending on mode switch
// Don't modify the start-of-packet word - it's a packet discriptor
// Don't modify data in non-video packets
//assign {red_out, green_out, blue_out} = (mode & ~sop & packet_video) ? new_image : {red,green,blue};

//Count valid pixels to tget the image coordinates. Reset and detect packet type on Start of Packet.
logic [10:0] x, y;
logic packet_video;

always @(*) begin
	
	if (~sop & packet_video) begin
		if (gate_enabled) begin
			if (blue >= threshold_gate) begin
			{red_out, green_out, blue_out} = 24'hffffff;
			end else begin
				{red_out, green_out, blue_out} = 24'h000000;
			end
		end else begin 
			{red_out, green_out, blue_out} = {red,green,blue};
		end
	end else begin
		{red_out, green_out, blue_out} = {red,green,blue};
	end 
mask = blue_out;
end
always@(posedge clk) begin
	if (sop) begin
		x <= 11'h0;
		y <= 11'h0;
		packet_video <= (blue[3:0] == 3'h0);
	end
	else if (in_valid) begin
		if (x == IMAGE_W-1) begin
			x <= 11'h0;
			y <= y + 11'h1;
		end
		else begin
			x <= x + 11'h1;
		end
	end
end

//Find first and last masked pixels
logic [10:0] x_min, y_min, x_max, y_max;
always@(posedge clk) begin
	if (mask[0] && in_valid) begin	//Update bounds when the pixel is masked
		if (x < x_min) x_min <= x;
		if (x > x_max) x_max <= x;
		if (y < y_min) y_min <= y;
		y_max <= y;
	end
	if (sop && in_valid) begin	//Reset bounds on start of packet
		x_min <= IMAGE_W-11'h1;
		x_max <= 0;
		y_min <= IMAGE_H-11'h1;
		y_max <= 0;
	end
end

//Process bounding box at the end of the frame.
logic [10:0] left, right, top, bottom;

always@(posedge clk) begin
	if (eop & in_valid & packet_video) begin  //Ignore non-video packets
		
		//Latch edges for display overlay on next frame
		left <= x_min;
		right <= x_max;
		top <= y_min;
		bottom <= y_max;
	end
end



//Streaming registers to buffer video signal
STREAM_REG_COM #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red, green, blue, sop, eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG_COM #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red_out, green_out, blue_out, sop, eop})
);


/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define THRESHOLD_GATE				0
`define COM_X							1
`define COM_Y						2
`define COM_MASS						3
`define BB_LOW_X					4
`define BB_LOW_Y						5
`define BB_HIGH_X					6
`define BB_HIGH_Y						7
`define THRESH_ENABLED				8



// Copy the requested word to the output port when there is a read.
always @ (posedge clk)
begin
   if (~reset_n) begin
	   s_readdata <= {32'b0};
	   threshold_gate <= 127;
	   gate_enabled <=1;
	end
	
	else if (s_chipselect & s_read) begin
		//Gating threshold
		if   (s_address == `THRESHOLD_GATE) s_readdata <= {24'h0, threshold_gate};
		
		//Center of mass calculations
		if	 (s_address == `COM_X) s_readdata <= ex_raw;
		if	 (s_address == `COM_Y) s_readdata <= ey_raw;
		if	 (s_address == `COM_MASS) s_readdata <= mass;
		
		//Bounding box information
		if	 (s_address == `BB_LOW_X) s_readdata <= left;
		if	 (s_address == `BB_LOW_Y) s_readdata <= top;
		if	 (s_address == `BB_HIGH_X) s_readdata <= right;
		if	 (s_address == `BB_HIGH_Y) s_readdata <= bottom;
		
		//Enabled
		if	 (s_address == `THRESH_ENABLED) s_readdata <= gate_enabled;
	end

	else if (s_chipselect & s_write) begin
		if (s_address == `THRESHOLD_GATE) threshold_gate <= s_writedata[7:0];
		if (s_address == `THRESH_ENABLED) gate_enabled <= s_writedata[0];
	end
end


COM_COUNTER com_counter(
	.clk(clk), .reset_n(reset_n), .sink_valid(in_valid), .sink_sop(sop), .sink_eop(eop), .packet_video(packet_video),
	.ex_raw(ex_raw), .ey_raw(ey_raw), .mass(mass), .mask(mask)
);

endmodule
