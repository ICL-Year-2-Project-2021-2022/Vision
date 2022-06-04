module GRAY_GATE (
	input logic[23:0] graygraygray,
	input logic[7:0] threshold,
	output logic[23:0] mask
);

always @(*) begin
	if (graygraygray[7:0] >= threshold) mask = 24'hffffff;
	else mask = 0;
end
endmodule