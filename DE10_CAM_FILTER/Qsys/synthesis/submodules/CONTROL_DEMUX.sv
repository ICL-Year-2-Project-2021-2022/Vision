module CONTROL_DEMUX(
	// global clock & reset
	input logic clk,
	input logic reset_n,

	// stream sink
	input logic[23:0] sink_data,
	input logic sink_valid,
	output logic sink_ready,
	input logic sink_sop,
	input logic sink_eop,
	
	// streaming source data
	output logic[23:0] source_data,
	output logic source_valid,
	input logic source_ready,
	output logic source_sop,
	output logic source_eop,
	
	// streaming source control
	output logic[23:0] source_data2,
	output logic source_valid2,
	input logic source_ready2,
	output logic source_sop2,
	output logic source_eop2

);


//////////////////////////////////////////////////////////////////////////
logic [7:0]   red, green, blue;
logic         in_valid, out_valid, out_valid2, ready, out_ready, out_ready2, sop, eop;
////////////////////////////////////////////////////////////////////////

always @(*) begin
	ready = out_ready & out_ready2;
	
	if(packet_video | (sop & (blue[3:0] == 3'h0))) begin
		out_valid2 = 0;
		out_valid = in_valid;
	end else begin
		out_valid = 0;
		out_valid2 = in_valid;
	end
		
	
end

logic packet_video;
always@(posedge clk) begin
	if (sop) begin
		packet_video <= (blue[3:0] == 3'h0);
	end else if (eop) begin
		packet_video <= 0;
	end
	
end

//Streaming registers to buffer video signal
STREAM_REG_DEMUX #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red, green, blue, sop, eop}),
	.ready_in(ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG_DEMUX #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(out_valid),
	.data_in({red, green, blue, sop, eop})
);

STREAM_REG_DEMUX #(.DATA_WIDTH(26)) out_reg2 (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready2),
	.valid_out(source_valid2),
	.data_out({source_data2,source_sop2,source_eop2}),
	.ready_in(source_ready2),
	.valid_in(out_valid2),
	.data_in({red, green, blue, sop, eop})
);



endmodule
