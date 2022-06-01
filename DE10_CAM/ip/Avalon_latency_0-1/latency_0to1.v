module avalon_latency(
		// global clock & reset
		input wire clk,
		input wire reset_n,

		// stream sink
		input wire[23:0] sink_data,
		input wire sink_valid,
		output wire sink_ready,
		input wire sink_sop,
		input wire sink_eop,
		
		// streaming source
		output wire[23:0] source_data,
		output wire source_valid,
		input wire source_ready,
		output wire source_sop,
		output wire source_eop
	);

	///////////////////////////////////////////////////////////////////////

		assign source_data = sink_data;
		assign source_valid = sink_valid;
		assign source_sop = sink_sop;
		assign source_eop = sink_eop;
		assign sink_ready = (~last_ready && source_ready) ? 0 : source_ready;
		
		reg last_ready;

	always @(posedge clk)begin
		if (reset_n == 0) last_ready <= 0;
		else last_ready <= source_ready;	
	end

endmodule
