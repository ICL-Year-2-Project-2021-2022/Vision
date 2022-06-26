module ST_TERMINATOR(
	//global clock & reset
	input logic	clk,
	input logic reset_n,
	
	// stream sink
	input logic	[23:0] sink_data,
	input logic sink_valid,
	output logic sink_ready,
	input logic sink_sop,
	input logic sink_eop
	
);

always @(*) begin	
	sink_ready = 1;
end

endmodule

