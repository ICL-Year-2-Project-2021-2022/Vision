module EDGE_BINS(
	// global clock & reset
	input logic clk,
	input logic reset_n,
	
	// mm slave
	input logic s_chipselect,
	input logic s_read,
	input logic s_write,
	output logic[31:0] s_readdata,
	input logic[31:0] s_writedata,
	input logic[4:0] s_address,

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
//////////////////////////////////////////////////////////////////////////
parameter IMAGE_W = 11'd640;
parameter IMAGE_H = 11'd480;

logic [7:0]   red, green, blue;
logic [7:0]   red_out, green_out, blue_out;

logic         sop, eop, in_valid, out_ready;

logic [10:0] x, y;
logic packet_video;

//Bins
logic[15:0] freq_bin [19:0];
logic[15:0] freq_bin_out [19:0];

logic[15:0] bindebug, bindebug2;
////////////////////////////////////////////////////////////////////////

always @(*) begin
	{red_out, green_out, blue_out} = {red,green,blue};
	bindebug = freq_bin[0];
	bindebug2 = freq_bin_out[0];
	end


always@(posedge clk) begin
	if (sop) begin
		x <= 11'h0;
		y <= 11'h0;
		packet_video <= (blue[3:0] == 3'h0);
		
		for (int i = 0; i<20; i++)begin
			freq_bin[i] <= 0;		
		end
	end
	else if (in_valid) begin
		if (x == IMAGE_W-1) begin
			x <= 11'h0;
			y <= y + 11'h1;
		end
		else begin
			x <= x + 11'h1;
		end
		
		if (blue > 128) begin
			freq_bin[x[10:5]] <= freq_bin[x[10:5]] + 1;
		end
	end
	
	
	if (eop & in_valid & packet_video) begin  //Ignore non-video packets
		//Update out registers
		for (int i = 0; i<20; i++)begin
			freq_bin_out[i] <= freq_bin[i];		
		end
	end
	
	//Avalon mm port
	
	if (~reset_n) begin
		s_readdata <= {32'b0};
		
		for (int i = 0; i<20; i++)begin
			freq_bin[i] <= 0;	
			freq_bin_out[i] <= 0;
		end
		
	end	else if (s_chipselect & s_read) begin
		s_readdata <= freq_bin_out[s_address];
	end
	
	
end

//Streaming registers to buffer video signal
STREAM_REG_BIN #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red, green, blue, sop, eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG_BIN #(.DATA_WIDTH(26)) out_reg (
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
