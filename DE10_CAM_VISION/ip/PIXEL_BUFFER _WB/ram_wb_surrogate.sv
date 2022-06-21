module ram_wb (
	input logic clock,
	input logic [23:0] data,
	input logic [8:0]  rdaddress,
	input logic	[8:0]  wraddress,
	input logic wren,
	output logic[23:0] q
	);
	
	logic[31:0] ram [511:0];
	
	always @(posedge clock) begin
		if(wren) ram[wraddress] <= data;
		
		q <= ram[rdaddress];
	end

endmodule