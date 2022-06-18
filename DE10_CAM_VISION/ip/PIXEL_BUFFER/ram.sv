module pixel_buffer_ram (
	input logic clock,
	input logic [7:0] data,
	input logic [12:0]  rdaddress,
	input logic	[12:0]  wraddress,
	input logic wren,
	output logic[7:0] q
	);
	
	logic[31:0] ram [5119:0];
	
	always @(posedge clock) begin
		if(wren) ram[wraddress] <= data;
		
		q <= ram[rdaddress];
	end

endmodule