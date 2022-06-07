module RGB_TO_HSV_COMB_TOP(
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
logic[23:0] hsv_comb;

parameter  POINT  =    12;


always @(*) begin

	//Direct connections
	source_valid = sink_valid;
	source_sop = sink_sop;
	source_eop   = sink_eop;
	sink_ready  = source_ready;
	
	if (sink_sop) begin
		source_data = sink_data;
	end else begin
		source_data = hsv_comb;
	end
	
end


RGB_TO_HSV_COMB  #(.POINT(POINT)) rgbtohsv (.rgb(sink_data), .hsv(hsv_comb));
	


endmodule

