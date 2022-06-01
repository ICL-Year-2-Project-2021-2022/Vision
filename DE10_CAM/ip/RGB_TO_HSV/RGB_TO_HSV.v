module RGB_TO_HSV(
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

////////////////////////////////////////////////////////////////////////
//
parameter IMAGE_W = 11'd640;
parameter IMAGE_H = 11'd480;
parameter POINT = 8;


///////////////////////////////////////////////////////////////////////

//Division lookup table 
reg [15:0] div_lookup[255:0];
wire[15:0] check_div1, check_div2,check_div16;


initial begin
	for (int i = 0; i < 255; i = i+1)begin
		div_lookup[i] <= (32'h1<<POINT)/i;
	end
end


assign check_div2 = div_lookup[2];
assign check_div1 = div_lookup[1];
assign check_div16 = div_lookup[16];

assign source_data = ((32'h512<<POINT)*div_lookup[2]);




endmodule

