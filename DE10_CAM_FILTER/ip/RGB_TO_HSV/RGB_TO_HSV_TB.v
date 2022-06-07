module RGB_TO_HSV_TB ();
    logic clk;
	logic [23:0] source_data;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, RGB_TO_HSV_TB);

        clk = 0;

         repeat (100) begin
            #1 clk = ~clk;
         end
    end

RGB_TO_HSV dut(.source_data(source_data));
endmodule