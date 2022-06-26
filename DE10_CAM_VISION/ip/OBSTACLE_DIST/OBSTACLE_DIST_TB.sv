module OBSTACLE_DIST_TB ();
	logic [23:0] sink_data, source_data;
	logic clk, reset;
	logic sink_sop, sink_eop, sink_valid, sink_ready, source_sop, source_eop, source_ready, source_valid;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, OBSTACLE_DIST_TB);
		
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
	
	for(int i = 0; i < 1000; i = i+1) begin
		////Avalon packet
	
	if (i%640 == 20 | i%640 == 40 | i%640 == 70 | i%640 == 85)begin
		sink_data = 24'hffffff;
	end else begin
		sink_data = 0;
	end
	
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
	sink_data = 0;
	sink_valid = 1;
	sink_sop = 0;
	sink_eop = 1;
	source_ready = 1;


	#2;
	
	sink_eop = 0;
	
	
	end
	

OBSTACLE_DIST dut(
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
	