module RGB_TO_GRAY(
	//global clock & reset
	input logic	clk,
	input logic reset_n,
	
	// stream sink
	input logic	[7:0] sink_data,
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
logic [23:0] rgb;
logic[7:0] din;
logic sop, eop, ready, valid, selected_gray;
logic video_packet;

always @(posedge clk) begin
	//Control logic
	if(eop || ~reset_n) begin
		video_packet <= 0;
	end
	if (sop) begin
		video_packet <= (din[3:0] == 3'h0);
	end
end

always @(*) begin
	selected_gray = video_packet && (~sop);
	if (selected_gray) begin
		rgb = {din, din, din};
	end else begin
		rgb = {din, 16'h0000};
	end
	
end


//Streaming registers to buffer video signal
STREAM_REG #(.DATA_WIDTH(10)) in_reg (
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
	.data_out({din,sop,eop})
);

STREAM_REG #(.DATA_WIDTH(26)) out_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Internal ports
	.ready_out(ready),
	.valid_in(valid),
	.data_in({rgb, sop, eop}),
	
	//source ports
	
	.ready_in(source_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop})
);

endmodule

