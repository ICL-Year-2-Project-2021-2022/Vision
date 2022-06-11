module COLOR_FILTER(
	// global clock & reset
	input logic clk,
	input logic reset_n,
	
	// mm slave
	input logic s_chipselect,
	input logic s_read,
	input logic s_write,
	output logic[31:0] s_readdata,
	input logic[31:0] s_writedata,
	input logic[3:0] s_address,

	// stream sink
	input logic[23:0] sink_data,
	input logic sink_valid,
	output logic sink_ready,
	input logic sink_sop,
	input logic sink_eop,
	
	// streaming source
	output logic[23:0] source_data,
	output logic source_valid,
	input logic source_ready,
	output logic source_sop,
	output logic source_eop

);

logic [23:0]	threshold_low;
logic [23:0]	threshold_high;
logic [7:0] mask;
logic apply_mask;



//////////////////////////////////////////////////////////////////////////
logic [7:0]   red, green, blue, grey;
logic [7:0]   red_out, green_out, blue_out;

logic         sop, eop, in_valid, out_ready;
////////////////////////////////////////////////////////////////////////

always @(*) begin
	mask = (red <= threshold_high[23:16] && red >= threshold_low[23:16]
	&& green <= threshold_high[15:8] && green >= threshold_low[15:8]
	&& blue <= threshold_high[7:0] && blue >= threshold_low[7:0]) ?  8'b11111111 : 0;
	
	{red_out, green_out, blue_out} = (~sop & packet_video & apply_mask) ? {mask, mask, mask} : {red, green, blue};
end

logic packet_video;
always@(posedge clk) begin
	if (sop) begin
		packet_video <= (blue[3:0] == 3'h0);
	end else if (eop) begin
		packet_video <= 0;
	end
	
end

//Streaming registers to buffer video signal
STREAM_REG_FILTER #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red, green, blue, sop, eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG_FILTER #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid),
	.data_in({red_out, green_out, blue_out, sop, eop})
);


/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define THRESHOLD_LOW			0
`define THRESHOLD_HIGH				1
`define APPLY_MASK				2




// Copy the requested word to the output port when there is a read.
always @ (posedge clk)
begin
   if (~reset_n) begin
	   s_readdata <= {32'b0};
	   apply_mask <= 1;
	end
	
	else if (s_chipselect & s_read) begin
		if   (s_address == `THRESHOLD_LOW) s_readdata <= {8'h0, threshold_low[23:0]};
		if   (s_address == `THRESHOLD_HIGH) s_readdata <= {8'h0, threshold_high[23:0]};
		if   (s_address == `APPLY_MASK) s_readdata <= {31'h0, apply_mask};
	end

	else if (s_chipselect & s_write) begin
		if (s_address == `THRESHOLD_LOW) threshold_low <= s_writedata[23:0];
		if (s_address == `THRESHOLD_HIGH) threshold_high <= s_writedata[23:0];
		if (s_address == `APPLY_MASK) apply_mask <= s_writedata[0];
	end
end
endmodule
