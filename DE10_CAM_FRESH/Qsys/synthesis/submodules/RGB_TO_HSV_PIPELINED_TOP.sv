module RGB_TO_HSV_PIPELINED_TOP(
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
logic[23:0] rgb, rgb_1, rgb_2, hsv, hsv_comb;
logic sop, eop, ready, valid, selected_hsv;

logic video_packet;
logic valid_1, valid_2;
logic sop_1, sop_2;
logic eop_1, eop_2;
logic advance, ready_d;
logic video_packet_1, video_packet_2, video_packet_3;

always @(posedge clk) begin
	//Control logic
	if(eop || ~reset_n) begin
		video_packet <= 0;
	end
	if (sop) begin
		video_packet <= (rgb[3:0] == 3'h0);
	end
	if (~reset_n)begin
		valid_1 <= 0;
		valid_2 <= 0;
		sop_1 <= 0;
		sop_2 <= 0;
		eop_1 <= 0;
		eop_2 <= 0;
		
	end if (advance) begin
		valid_1 <= valid;
		valid_2 <= valid_1;
		sop_1 <= sop;
		sop_2 <= sop_1;
		eop_1 <= eop;
		eop_2 <= eop_1;
		
		rgb_1 <= rgb;
		rgb_2 <= rgb_1;
		
		video_packet_1 <= video_packet;
		video_packet_2 <= video_packet_1;
		video_packet_3 <= video_packet_2;
	end
	
	ready_d = ready;
end

always @(*) begin
	selected_hsv = video_packet_2 && (~sop_2);
	if (selected_hsv) begin
		hsv = hsv_comb;
	end else begin
		hsv = rgb_2;
	end
	
	advance = ready_d | ~valid_2;
	
end


//Streaming registers to buffer video signal
STREAM_REG #(.DATA_WIDTH(26)) in_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Sink inputs
	.ready_out(sink_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop}),
	
	//Internal ports
	.ready_in(ready),
	.valid_out(valid),
	.data_out({rgb,sop,eop})
);

STREAM_REG #(.DATA_WIDTH(26)) out_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Internal ports
	.ready_out(ready),
	.valid_in(valid_2),
	.data_in({hsv, sop_2, eop_2}),
	
	//source ports
	
	.ready_in(source_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop})
);

RGB_TO_HSV_PIPELINED  rgbtohsv (.rgb(rgb), .hsv(hsv_comb), .advance(advance), .clk(clk), .reset(reset_n));

endmodule
