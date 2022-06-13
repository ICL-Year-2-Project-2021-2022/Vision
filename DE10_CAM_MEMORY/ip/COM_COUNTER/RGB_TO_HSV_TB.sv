module RGB_TO_HSV_TB ();
    logic clk, reset_n;
	logic [23:0] source_data, sink_data;
	logic sink_valid, sink_ready, sink_sop, sink_eop;
	logic source_valid, source_ready, source_sop, source_eop;



    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, RGB_TO_HSV_TB);

        clk = 0;
		
		repeat (100) begin
            #1 clk = ~clk;
         end

    end
	
	
	
		 
	initial begin
		reset_n = 0;
		
		//Initialization
		sink_data = 24'h0;
		sink_valid = 0;
		sink_sop  = 0;
		sink_eop = 0;
		source_ready = 0;
		
		#2;
		reset_n = 1;
		
		//Packet block
		sink_data = 24'hffffff;
		sink_valid = 0;
		sink_sop  = 0;
		sink_eop = 0;
		source_ready = 1;

		
		
		#4;
		//Packet block
		sink_data = 24'h112233;
		sink_valid = 1;
		sink_sop  = 1;
		sink_eop = 0;
		source_ready = 1;
		
		#2;
		
		//Packet block
		sink_data = 24'h112233;
		sink_valid = 1;
		sink_sop  = 0;
		sink_eop = 0;
		source_ready = 1;
		
		#2;
		//Packet block
		sink_data = 24'h223344;
		sink_valid = 1;
		sink_sop  = 0;
		sink_eop = 0;
		source_ready = 1;
		
		
		
	end
	

RGB_TO_HSV dut(
	.clk(clk),
	.reset_n(reset_n),
	
	
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