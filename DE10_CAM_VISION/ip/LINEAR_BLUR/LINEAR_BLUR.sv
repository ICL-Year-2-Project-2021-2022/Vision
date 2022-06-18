module LINEAR_BLUR(
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
	output logic source_eop,
	
	// mm slave
	input logic s_chipselect,
	input logic s_read,
	input logic s_write,
	output logic[31:0] s_readdata,
	input logic[31:0] s_writedata,
	input logic[3:0] s_address
	
);

parameter length = 4;
//Intermediate declarations
logic[23:0] data[length-1:0];
logic [length-1:0] sop;
logic [length-1:0] eop;
logic [length-1:0] valid;
logic [length-1:0] video_packet;


logic[23:0]data_out, data_in;
logic valid_in, sop_in, eop_in, ready, selected_blur;

logic advance, ready_d, blurselect;
logic video_packet_debug;
logic blur_enabled;

always @(posedge clk) begin
	//Control logic
	if(eop_in || ~reset_n) begin
		video_packet[0] <= 0;
	end
	if (sop_in) begin
		video_packet[0] <= (data_in[3:0] == 3'h0);
		
	end
	if (~reset_n)begin
		valid <= 0;
		sop <= 0;
		eop <= 0;
		video_packet[0] <= 0;
		
	end if (advance) begin
			data[0] <= data_in;
			valid[0] <= valid_in;
			sop[0] <= sop_in;
			eop[0] <= eop_in;
		
		for (int i = 0; i < length-1; i++) begin
			video_packet[i+1] <= video_packet[i];
			data[i+1] <= data[i];
			sop[i+1] <= sop[i];
			eop[i+1] <= eop[i];
			valid[i+1] <= valid [i];
		end
	end
	
	ready_d = ready;
end

always @(*) begin
	video_packet_debug = video_packet[length/2];
	selected_blur = video_packet[length/2] && (~sop[length/2]);
	if (selected_blur && blur_enabled) begin
		blurselect = 1;
		data_out = {3{((data[0][7:0]>>2)&({8{video_packet[0]}})) + ((data[1][7:0]>>2)&({8{video_packet[1]}})) + ((data[2][7:0]>>2)&({8{video_packet[2]}})) + ((data[3][7:0]>>2)&({8{video_packet[3]}}))}};
		
	end else begin
		blurselect = 0;
		data_out = data[length/2];
	end
	
	advance = ready_d | ~valid[length/2];
	
end


//Streaming registers to buffer video signal
STREAM_REG_BLUR #(.DATA_WIDTH(26)) in_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Sink inputs
	.ready_out(sink_ready),
	.valid_in(sink_valid),
	.data_in({sink_data,sink_sop,sink_eop}),
	
	//Internal ports
	.ready_in(ready),
	.valid_out(valid_in),
	.data_out({data_in,sop_in,eop_in})
);

STREAM_REG_BLUR #(.DATA_WIDTH(26)) out_reg (
	//Global clocks
	.clk(clk),
	.rst_n(reset_n),
	
	//Internal ports
	.ready_out(ready),
	.valid_in(valid[length/2]),
	.data_in({data_out, sop[length/2], eop[length/2]}),
	
	//source ports
	
	.ready_in(source_ready),
	.valid_out(source_valid),
	.data_out({source_data,source_sop,source_eop})
);


/////////////////////////////////
/// Memory-mapped port		 /////
/////////////////////////////////

// Addresses
`define BLUR_ENABLED				0



// Copy the requested word to the output port when there is a read.
always @ (posedge clk)
begin
   if (~reset_n) begin
	   s_readdata <= {32'b0};
	   blur_enabled <= 1;
	end
	
	else if (s_chipselect & s_read) begin
		//Enabled
		if	 (s_address == `BLUR_ENABLED) s_readdata <= blur_enabled;
	end

	else if (s_chipselect & s_write) begin
		if (s_address == `BLUR_ENABLED) blur_enabled <= s_writedata[0];
	end
end

endmodule

