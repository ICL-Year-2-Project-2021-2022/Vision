module CONTROL_DEMUX_TB ();
	logic [23:0] sink_data, source_data;
	logic clk, reset;
	logic sink_sop, sink_eop, sink_valid, sink_ready, source_sop, source_eop, source_ready, source_valid, irq;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, CONTROL_DEMUX_TB);
		
		clk = 0;
		
		repeat (100) begin
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
	
	////Avalon packet
	sink_data = 24'h010203;
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
	////Avalon packet
	sink_data = 0;
	sink_valid = 0;
	sink_sop = 0;
	sink_eop = 0;
	source_ready = 0;
	
	#2;
	
	source_ready = 0;
	
	#4;
	
	
	//Control packet
	
	////Avalon packet
	sink_data = 1;
	sink_valid = 1;
	sink_sop = 1;
	sink_eop = 0;
	source_ready = 1;
	
	#2;
	
	////Avalon packet
	sink_data = 24'h102030;
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
	sink_valid = 0;
	
	
	end
	

CONTROL_DEMUX dut(
	.clk(clk),
	.reset_n(reset),
	
	.sink_data(sink_data),
	.sink_valid(sink_valid),
	.sink_ready(sink_ready),
	.sink_sop(sink_sop),
	.sink_eop(sink_eop),
	
	.source_ready(1),
	.source_ready2(1)
	);

endmodule
	