module COM_COUNTER(
	//global clock & reset
	input logic	clk,
	input logic reset_n,
	input logic[7:0] mask,

	// stream sink
	input logic	[23:0] sink_data,
	input logic sink_valid,
	input logic sink_sop,
	input logic sink_eop,
	input logic packet_video,
	
	//Outputs
	output logic[31:0] ex_raw,
	output logic[31:0] ey_raw,
	output logic[31:0] mass,
	output logic[31:0] current_ex_raw,
	output logic[31:0] current_ey_raw,
	output logic[31:0] current_mass,
	output logic[31:0] pixels_counter_frame,
	output logic[31:0] sop_counter,
	output logic[31:0] eop_counter,
	output logic[31:0] video_packet_counter
);

///////////////////////////////////////////////////////
parameter width = 640;
parameter height = 480;
///////////////////////////////////////////////////////
logic[$clog2(width):0] x_pixel, y_pixel;
logic[31:0] pixels_counter;

always @(posedge clk) begin
	//last_ready <= source_ready;

	if (~reset_n) begin
		x_pixel <= 0;
		y_pixel <= 0;
		
		mass <= 0;
		ex_raw <= 0;
		ey_raw <= 0;
		
		current_ex_raw <= 0;
		current_ey_raw <= 0;
		current_mass <= 0;
		pixels_counter <= 0;
		pixels_counter_frame <= 0;
		eop_counter <= 0;
		sop_counter <= 0;
		video_packet_counter <= 0;
	end else if (sink_sop) begin
			x_pixel <= 0;
			y_pixel <= 0;	
			current_ex_raw <= 0;
			current_ey_raw <= 0;
			current_mass <= 0;
			pixels_counter <= 0;
			sop_counter <= sop_counter + 1;
	end else if (packet_video) begin
		video_packet_counter <= video_packet_counter + 1;
		if (sink_valid) begin
			pixels_counter <= pixels_counter + 1;
			if (x_pixel < 4 & y_pixel < 4) begin
				$display("sink valid, x: %d, y: %d", x_pixel, y_pixel);
			end
				
			if (mask == 8'hff) begin
				$display("x_pixel: %d", x_pixel);
				$display("y_pixel: %d", y_pixel);
				current_ex_raw <= current_ex_raw + x_pixel;
				current_ey_raw <= current_ey_raw + y_pixel;
				current_mass <= current_mass + 1;
			end

			if(x_pixel >= width-1) begin
				x_pixel <= 0;
				y_pixel <= y_pixel + 1;
			end else begin
				x_pixel <= x_pixel + 1;
				//y_pixel <= 0;
			end
			
			/*if (pixels_counter >= width * height) begin
				mass <= current_mass;
				ex_raw <= current_ex_raw;
				ey_raw <= current_ey_raw;
				pixels_counter <= 0;
				current_ex_raw <= 0;
				current_ey_raw <= 0;
				current_mass <= 0;
				x_pixel <= 0;
				y_pixel <= 0;
			end*/
		end		
		if (sink_eop) begin
			mass <= current_mass;
			ex_raw <= current_ex_raw;
			ey_raw <= current_ey_raw;
			pixels_counter_frame <= pixels_counter;
			eop_counter <= eop_counter + 1;
		end
	end
end

endmodule