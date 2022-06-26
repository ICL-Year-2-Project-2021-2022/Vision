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
`define EIGHT_BIT_MODE 1
parameter sub_factor = 8;
parameter clog_sub_factor = 3;

///////////////////////////////////////////////////////

`ifdef EIGHT_BIT_MODE
	logic [7:0] buffer [width/sub_factor : 0] [height/sub_factor : 0];
`else
	logic [31:0] buffer [width/sub_factor : 0] [height/sub_factor : 0];
`endif

logic [15:0] x_pixel, y_pixel;
logic [7:0]   red, green, blue;

logic sop, eop, in_valid, out_ready;
logic halt_current, halt;
logic packet_video;

logic grab_px;

// Addresses
////////////////////////////////////////////////////////////////////////

always @(*) begin	
	grab_px = x_pixel[ clog_sub_factor -1 :0] == 0 && y_pixel[ clog_sub_factor -1  : 0] == 0 && ~halt_current && packet_video;
end

/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define FRAME_HALT					5000

// Copy the requested word to the output port when there is a read.
always @ (posedge clk) begin
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
	
	if (x_pixel[clog_sub_factor -1 :0] == 0 && y_pixel[ clog_sub_factor -1 : 0] == 0 && ~halt_current && packet_video) begin
		//buffer[x_pixel[15:clog_sub_factor]][y_pixel[15:clog_sub_factor]] <= {red[7:6], green[7:5], blue[7:5]};
		
		
		`ifdef EIGHT_BIT_MODE
			buffer[x_pixel[15:clog_sub_factor]][y_pixel[15:clog_sub_factor]] <= {red[7:6], green[7:5], blue[7:5]};
		`else
			buffer[x_pixel[15:clog_sub_factor]][y_pixel[15:clog_sub_factor]] <= {red, green, blue};
		`endif
		end
	
   if (~reset_n) begin
	   s_readdata <= {32'b0};
	end
	
	else if (s_chipselect & s_read) begin
		//Reads
		if	 (s_address == `FRAME_HALT) s_readdata <= halt;
		else s_readdata <= buffer[s_address[13:7]][s_address[6:0]];
	end

	else if (s_chipselect & s_write) begin
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


endmodule
