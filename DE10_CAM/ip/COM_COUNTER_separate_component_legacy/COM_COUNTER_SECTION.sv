module COM_COUNTER_SECTION(
	//global clock & reset
	input logic	clk,
	input logic reset_n,

	// mm slave
	input logic s_chipselect,
	input logic s_read,
	input logic s_write,
	output logic[31:0] s_readdata,
	input logic[31:0] s_writedata,
	input logic[31:0] s_address,

	// stream sink
	input logic[23:0] sink_data,
	input sink_valid,
	output sink_ready,
	input sink_sop,
	input sink_eop,
	
	// streaming source
	output logic[23:0] source_data,
	output logic source_valid,
	input logic source_ready,
	output logic source_sop,
	output logic source_eop,
	
	//Outputs
	output logic[31:0] ex_raw,
	output logic[31:0] ey_raw,
	output logic[31:0] mass
);

///////////////////////////////////////////////////////
parameter width = 640;
parameter height = 480;
///////////////////////////////////////////////////////
logic[$clog2(width):0] x_pixel, y_pixel;
logic[31:0] current_ex_raw, current_ey_raw, current_mass;
logic[7:0] red_out, green_out, blue_out;

`define X_RAW 0
`define Y_RAW 1
`define MASS 2

always @(posedge clk) begin
	//last_ready <= source_ready;
	
	if (reset_n) begin
		x_pixel <= 0;
		y_pixel <= 0;
		
		mass <= 0;
		ex_raw <= 0;
		ey_raw <= 0;
		
		current_ex_raw <= 0;
		current_ey_raw <= 0;
		current_mass <= 0;
	end else if (source_eop) begin
		mass <= current_mass;
		ex_raw <= current_ex_raw;
		ey_raw <= current_ey_raw;
	end else if (source_sop) begin
		x_pixel <= 0;
		y_pixel <= 0;
		
		mass <= current_mass;
		ex_raw <= current_ex_raw;
		ey_raw <= current_ey_raw;
		
		current_ex_raw <= 0;
		current_ey_raw <= 0;
		current_mass <= 0;
	end else if (source_valid) begin
		if(x_pixel >= width-1) begin
			x_pixel <= 0;
			y_pixel <= y_pixel + 1;
		end else begin
			x_pixel <= x_pixel + 1;
		end
			
		if (source_data[23:16] == 8'hff) begin
			current_ex_raw = current_ex_raw + x_pixel;
			current_ey_raw = current_ey_raw + y_pixel;
			current_mass = current_mass + 1;
		end	
	end else if (s_chipselect & s_read) begin
		if   (s_address == `X_RAW) s_readdata <= ex_raw;
		if   (s_address == `Y_RAW) s_readdata <= ey_raw;
		if   (s_address == `MASS) s_readdata <= mass;
	end
end

//Streaming registers to buffer video signal
STREAM_REG #(.DATA_WIDTH(26)) in_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(sink_ready),
	.valid_out(in_valid),
	.data_out({red_out,green_out,blue_out,sop,eop}),
	.ready_in(out_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop})
);

STREAM_REG #(.DATA_WIDTH(26)) out_reg (
	.clk(clk),
	.rst_n(reset_n),
	.ready_out(out_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop}),
	.ready_in(source_ready),
	.valid_in(in_valid), // this is low (connected to in_reg.valid_out)
	.data_in({red_out, green_out, blue_out, sop, eop})
);

endmodule

