module COM_COUNTER_SECTION(
	//global clock & reset
	input logic	clk,
	input logic reset_n,
	
	// stream sink
	input logic	[23:0] sink_data,
	input logic sink_valid,
	input logic sink_sop,
	input logic sink_eop,
	
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
	end else if (sink_eop) begin
		mass <= current_mass;
		ex_raw <= current_ex_raw;
		ey_raw <= current_ey_raw;
	end else if (sink_sop) begin
		x_pixel <= 0;
		y_pixel <= 0;
		
		mass <= current_mass;
		ex_raw <= current_ex_raw;
		ey_raw <= current_ey_raw;
		
		current_ex_raw <= 0;
		current_ey_raw <= 0;
		current_mass <= 0;
		
	end else if (sink_valid) begin
		if(x_pixel >= width-1) begin
			x_pixel <= 0;
			y_pixel <= y_pixel + 1;
		end else begin
			x_pixel <= x_pixel + 1;
		end
			
		if (sink_data[23:16] == 8'hff) begin
			current_ex_raw = current_ex_raw + x_pixel;
			current_ey_raw = current_ey_raw + y_pixel;
			current_mass = current_mass + 1;
		end
		
	end
end

endmodule

