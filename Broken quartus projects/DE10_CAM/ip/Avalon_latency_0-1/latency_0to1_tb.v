module avalon_latency_tb ();
    logic clk, reset;
	logic source_ready, sink_ready, source_valid, sink_valid;
	logic[23:0] data_in, data_out;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, avalon_latency_tb);

        clk = 0;

         repeat (10) begin
            #1 clk = ~clk;
         end
    end
	
	initial begin
		reset = 0;
		source_ready =0;
		data_in = 24'hf;
		sink_valid = 1;
		
		#1;
		sink_data =1;
		reset=1;
		sink_valid = 0;
		
		#2;
		sink_data = 2;
		source_ready = 1;
		
		#2;
		sink_data = 3;
		source_ready = 0;
		
		
	end

avalon_latency dut(.clk(clk), .reset_n(reset), .source_ready(source_ready), .sink_ready(sink_ready), .source_valid(source_valid), .sink_valid(sink_valid), .source_data(data_out), .sink_data(data_in));
endmodule