module COM_COUNTER_SECTION_tb ();
    integer i, j, x, y;

    logic clk, reset_n;

	logic[23:0] sink_data;
	logic sink_valid;
	logic sink_sop;
	logic sink_eop;
	
    logic[31:0] expected_x;
    logic[31:0] expected_y;
    logic[31:0] expected_mass;

	logic[31:0] ex_raw;
	logic[31:0] ey_raw;
	logic[31:0] mass;

    initial begin
        $dumpfile("COM_COUNTER_SECTION_tb.vcd");
        $dumpvars(0, COM_COUNTER_SECTION_tb);

        clk = 0;
        repeat (2 * 480 * 640 + 10) begin
            #1 clk = !clk;
        end
    end

    initial begin
        expected_mass = 0;
        expected_x = 0;
        expected_y = 0;
        for (x = 320; x <= 360; x++) begin
            for (y = 220; y <= 260; y++) begin
                expected_x = expected_x + x;
                expected_y = expected_y + y;
                expected_mass = expected_mass + 1;
            end
        end
    end

    initial begin
        sink_valid = 1;
        sink_sop = 0;
        sink_eop = 0;
        reset_n = 1;
        #2;
        reset_n = 0;
        #1;
        for (i = 0; i < 480; i = i + 1) begin
            for (j = 0; j < 640; j = j + 1) begin
                if (i >= 220 && i <= 260 && j >= 320 && j <= 360) begin
                    sink_data = 24'hffffff;
                end else begin
                    sink_data = 24'h0;
                end
                #2;
            end
        end
        sink_eop = 1;
        #4;
        sink_eop = 0;
        $display("expected X=%b, expected Y=%b, expected mass=%b, current X=%b, current Y=%b, current mass=%b", expected_x, expected_y, expected_mass, ex_raw, ey_raw, mass);
        assert (expected_x == ex_raw) else $fatal(2, "X is not correct!");
        assert (expected_y == ey_raw) else $fatal(2, "Y is not correct!");
        assert (expected_mass == mass) else $fatal(2, "Mass is not correct!");
    end

COM_COUNTER_SECTION dut(
    .clk(clk), .reset_n(reset_n), .sink_data(sink_data), .sink_valid(sink_valid), .sink_sop(sink_sop),
	.sink_eop(sink_eop), .ex_raw(ex_raw), .ey_raw(ey_raw), .mass(mass)
);

endmodule
