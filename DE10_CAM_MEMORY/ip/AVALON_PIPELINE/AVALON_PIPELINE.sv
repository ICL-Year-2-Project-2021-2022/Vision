module AVALON_PIPELINE(
	//global clock & reset
	input logic	clk,
	input logic reset_n,
	
	// stream sink
	input logic	[23:0] sink_data,
	input logic sink_valid,
	output logic sink_ready,
	input logic sink_sop,
	input logic sink_eop,
	
	// streaming source
	output logic [23:0] source_data,
	output logic source_valid,
	input logic source_ready,
	output logic source_sop,
	output logic source_eop
	
);

//Intermediate declarations
logic[23:0] rgb, hsv, hsv_comb;
logic sop, eop, ready, valid;

always @(*) begin
	hsv = rgb;
end


//Streaming registers to buffer video signal
STREAM_REG #(.DATA_WIDTH(26)) in_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Sink inputs
	.ready_out(sink_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop}),
	
	//Internal ports
	.ready_in(ready),
	.valid_out(valid),
	.data_out({rgb,sop,eop})
);

STREAM_REG #(.DATA_WIDTH(26)) out_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Internal ports
	.ready_out(ready),
	.valid_in(valid),
	.data_in({hsv, sop, eop}),
	
	//source ports
	
	.ready_in(source_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop})
);


endmodule

