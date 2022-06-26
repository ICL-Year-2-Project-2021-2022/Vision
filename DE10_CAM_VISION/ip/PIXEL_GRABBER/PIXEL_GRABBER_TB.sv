module COM_COUNTER_TB ();
	logic [23:0] sink_data, source_data;
	logic clk, reset;
	logic sink_sop, sink_eop, sink_valid, sink_ready, source_sop, source_eop, source_ready, source_valid;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, COM_COUNTER_TB);
		
		clk = 0;
		
		repeat (1000000) begin
            #1 clk = ~clk;
         end
		 
    end
	
	
	
		 
	initial begin
	reset = 0;
	#2;
	reset = 1;
	
	//Avalon packet
	sink_data = 0;
	sink_valid = 0;
	sink_sop = 0;
	sink_eop = 0;
	source_ready = 1;
	
	#2;
	
	
	
	//Avalon packet
	sink_data = 0;
	sink_valid = 1;
	sink_sop = 1;
	sink_eop = 0;
	source_ready = 1;
	
	#2;
	
	for(int i = 2; i < 1000000; i = i+1) begin
		////Avalon packet
	sink_data = i;
	sink_valid = 1;
	sink_sop = 0;
	sink_eop = 0;
	source_ready = 1;
	
	#2;
	
	end
	
	
	////Avalon packet
	sink_data = 24'h102030;
	sink_valid = 1;
	sink_sop = 0;
	sink_eop = 1;
	source_ready = 1;
	
	#2;
	////Avalon packet
	sink_data = 0;
	sink_valid = 0;
	sink_sop = 0;
	sink_eop = 0;
	source_ready = 1;
		
	#8;
	
	
	//Control packet
	
	////Avalon packet
	sink_data = 1;
	sink_valid = 1;
	sink_sop = 1;
	sink_eop = 0;
	source_ready = 1;
	
	#2;
	
	////Avalon packet
	sink_data = 24'h1020ff;
	sink_valid = 1;
	sink_sop = 0;
	sink_eop = 0;
	source_ready = 1;
	
	#2;
	
	////Avalon packet
	sink_data = 24'h102030;
	sink_valid = 1;
	sink_sop = 0;
	sink_eop = 1;
	source_ready = 1;


	#2;
	
	sink_eop = 0;
	
	
	end
	

PIXEL_GRABBER dut(
	.clk(clk),
	.reset_n(reset),
	.sink_data(sink_data),
	.sink_valid(sink_valid),
	.sink_ready(sink_ready),
	.sink_sop(sink_sop),
	.sink_eop(sink_eop),
	
	
	.source_data(source_data),
	.source_valid(source_valid),
	.source_ready(source_ready),
	.source_sop(source_sop),
	.source_eop(source_eop)
	);

endmodule
	