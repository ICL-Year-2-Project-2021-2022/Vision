module PIXEL_BUFFER(
	// global clock & reset
	input logic clk,
	input logic reset_n,
	
	// mm slave
	input logic s_chipselect,
	input logic s_read,
	input logic s_write,
	output logic[31:0] s_readdata,
	input logic[31:0] s_writedata,
	input logic[13:0] s_address,

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

//Size parameters
parameter sub_factor = 8;
parameter clog_sub_factor = 3;

///////////////////////////////////////////////////////

logic [15:0] x_pixel, y_pixel;
logic [7:0]   red, green, blue;
logic[7:0] compressed, ram_data;

logic sop, eop, in_valid, out_ready;
logic halt_current, halt;
logic packet_video;

logic grab_px;

logic s_chipselect_d, s_read_d;
logic[12:0] wraddress, rdaddress, s_address_d;

// Addresses
`define FRAME_HALT					5000
////////////////////////////////////////////////////////////////////////

always @(*) begin	
	grab_px = x_pixel[clog_sub_factor -1 :0] == 0 && y_pixel[ clog_sub_factor -1 : 0] == 0 && ~halt_current && packet_video;
	compressed = {red[7:6], green[7:5], blue[7:5]};
	//compressed = {red, green, blue};
	wraddress = {x_pixel[9:3], y_pixel[8:3]};
	rdaddress = s_address;
	
	if (s_chipselect_d & s_read_d)begin
		if(s_address_d == `FRAME_HALT)begin
			s_readdata = {31'b0, halt};
		end else begin
			s_readdata = {ram_data};
		end
	end
	

end

always @ (posedge clk) begin

	//Advance avalon mm stuff
	s_chipselect_d <= s_chipselect;
	s_read_d <= s_read;
	s_address_d <= s_address;
	
	if (~reset_n) begin
		x_pixel <= 0;
		y_pixel <= 0;
		halt_current <= 0;
		halt <= 0;
	end else if (sop) begin
			x_pixel <= 0;
			y_pixel <= 0;	
			packet_video <= (blue[3:0] == 3'h0);
			halt_current <= halt;
	end else if (eop) begin
		packet_video <= 0;
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

	
	if (s_chipselect & s_write) begin
		if (s_address == `FRAME_HALT) halt <= s_writedata;
	end
end

//Streaming registers to buffer video signal
STREAM_REG_BUF #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red, green, blue, sop, eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG_BUF #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red, green, blue, sop, eop})
);


pixel_buffer_ram	pixel_buffer_ram_inst (
	.clock ( clk ),
	.data ( compressed ),
	.rdaddress ( rdaddress ),
	.wraddress ( wraddress ),
	.wren ( grab_px ),
	.q ( ram_data )
	);




endmodule
