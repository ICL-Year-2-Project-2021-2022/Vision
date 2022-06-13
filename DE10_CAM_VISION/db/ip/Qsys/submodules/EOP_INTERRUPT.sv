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
logic [5:0] irq_d;


always @(posedge clk) begin
	//Control logic
	if(eop || ~reset_n) begin
		video_packet <= 0;
	end
	else if (sop) begin
		video_packet <= (data[3:0] == 3'h0);
	end
	
	if (video_packet && eop && valid) begin
		irq_d[0] <= 1;
	end else begin
		irq_d[0] <= 0;
	end
	
	//Shift irq
	for (int i = 0; i  < 5; i= i + 1) begin
		irq_d[i+1] <= irq_d[i];
	end
end

always @(*) begin	
	if (irq_d > 0) irq = 1;
	else irq = 0;
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

