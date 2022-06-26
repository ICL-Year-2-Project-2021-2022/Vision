module EDGE_BINS_TB ();
	logic [23:0] sink_data, source_data;
	logic clk, reset;
	logic sink_sop, sink_eop, sink_valid, sink_ready, source_sop, source_eop, source_ready, source_valid;
	
	logic[31:0] data, address;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, EDGE_BINS_TB);
		
		clk = 0;
		
		repeat (614490) begin
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
	
	sink_valid = 1;
	sink_sop = 1;
	sink_data = 0;
	
	
	#2;
	
	sink_sop = 0;
	for (int i = 0; i < 640*480; i++) begin
		if (i%640 == 31) sink_data = 255;
		else if (i%640 == 32) sink_data = 255;
		else if (i%640 == 630) sink_data = 255;
		else sink_data = 0;
		
		#2;
	end
	
	#2;
	
	//Avalon packet
	sink_data = 0;
	sink_valid = 1;
	sink_sop = 0;
	sink_eop = 1;
	source_ready = 1;
	
	#2;
	
	#2;
	
	for (int i = 0; i < 20; i++) begin
		address = i;
		#2;
		$display("Reg %d: %x", i, data );
		#2;
		
	end
	
	end
	

EDGE_BINS dut(
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
	.source_eop(source_eop),
	
	.s_chipselect(1),
	.s_read(1),
	.s_write(0),
	.s_readdata(data),
	.s_address(address)
	
	);

endmodule
	