module EOP_INTERRUPT(
	//global clock & reset
	input logic	clk,
	input logic reset_n,
	
	// stream sink
	input logic	[23:0] sink_data,
	input logic sink_valid,
	output logic sink_ready,
	input logic sink_sop,
	input logic sink_eop,
	
	output logic irq
	
);

//Intermediate declarations
logic[23:0] data;
logic sop, eop, ready, valid;
logic video_packet;


always @(posedge clk) begin
	//Control logic
	if(eop || ~reset_n) begin
		video_packet <= 0;
	end
	else if (sop) begin
		video_packet <= (data[3:0] == 3'h0);
	end
end

always @(*) begin	
	if (video_packet && eop && valid) begin
		irq = 1;
	end else begin
		irq = 0;
	end
end


//Streaming registers to buffer video signal
STREAM_REG_EOP #(.DATA_WIDTH(26)) in_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Sink inputs
	.ready_out(sink_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop}),
	
	//Internal ports
	.ready_in(1'b1),
	.valid_out(valid),
	.data_out({data,sop,eop})
);

endmodule

