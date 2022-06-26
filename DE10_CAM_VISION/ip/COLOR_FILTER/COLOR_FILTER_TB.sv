module COLOR_FILTER_TB ();
    integer i, j, x, y;

    logic clk, reset_n;

    // mm slave
	logic s_chipselect;
	logic s_read;
	logic s_write;
	logic[31:0] s_readdata;
	logic[31:0] s_writedata;
	logic[31:0] s_address;

    // streaming sink
	logic[23:0] sink_data;
	logic in_valid;
	logic sop;
	logic eop;
    logic packet_video;
	
	// streaming source
	logic[23:0] source_data;
	logic source_ready;
    logic source_valid;
	logic source_sop;
	logic source_eop;

    logic[31:0] expected_x;
    logic[31:0] expected_y;
    logic[31:0] expected_mass;

	logic[31:0] ex_raw;
	logic[31:0] ey_raw;
	logic[31:0] mass;
	logic[31:0] current_ex_raw;
	logic[31:0] current_ey_raw;
	logic[31:0] current_mass;
	logic[31:0] pixels_counter_frame;
	logic[31:0] sop_counter;
	logic[31:0] eop_counter;
	logic[31:0] video_packet_counter;
    logic[7:0] mask;
    
    initial begin
        $dumpfile("COM_COUNTER_tb.vcd");
        $dumpvars(0, COM_COUNTER_tb);

        clk = 0;
        repeat (10 * 480 * 640 + 14) begin
            #1 clk = !clk;
        end
    end

    initial begin
        expected_mass = 0;
        expected_x = 0;
        expected_y = 0;
        for (y = 220; y <= 260; y++) begin
            for (x = 320; x <= 360; x++) begin
                expected_x = expected_x + x;
                expected_y = expected_y + y;
                expected_mass = expected_mass + 1;
            end
        end
    end

    initial begin
        in_valid = 1;
        sop = 0;
        eop = 0;
        reset_n = 1;
        source_ready = 1;
        packet_video = 1;
        #2;
        reset_n = 0;
        #2;
        reset_n = 1;
        sop = 1;
        #2;
        sop = 0;
        #2;
        for (i = 0; i < 479; i = i + 1) begin
            for (j = 0; j < 640; j = j + 1) begin
                if (i >= 220 && i <= 260 && j >= 320 && j <= 360) begin
                    mask = 8'hff;
                end else begin
                    mask = 8'h0;
                end
                #2;
            end
        end
        for (j = 0; j < 639; j = j + 1) begin
            mask = 8'h0;
            #2;
        end
        mask = 8'h0;
        eop = 1;
        #2;
        eop = 0;
        #2;
        sop = 1;
        #2;
        sop = 0;
        #2;
        for (i = 0; i < 479; i = i + 1) begin
            for (j = 0; j < 640; j = j + 1) begin
                if (i >= 0 && i <= 1 && j >= 0 && j <= 1) begin
                    $display("HIGH for i: %d, j: %d", i, j);
                    mask = 8'hff;
                end else begin
                    mask = 8'h0;
                end
                #2;
            end
        end
        for (j = 0; j < 639; j = j + 1) begin
            mask = 8'h0;
            #2;
        end
        mask = 8'h0;
        eop = 1;
        #2;
        eop = 0;
        $display("expected X=%d, expected Y=%d, expected mass=%d, current X=%d, current Y=%d, current mass=%d", 2, 2, 4, ex_raw, ey_raw, mass);
        $display("pixel counter: %d", pixels_counter_frame);
        $display("sop counter: %d", sop_counter);
        $display("eop counter: %d", eop_counter);
        assert (2 == ex_raw) else $fatal(2, "X is not correct!");
        assert (2 == ey_raw) else $fatal(2, "Y is not correct!");
        assert (4 == mass) else $fatal(2, "Mass is not correct!");
    end

COLOR_FILTER dut(
	.clk(clk), .reset_n(reset_n),
	.sink_valid(in_valid), .sink_sop(sop), .sink_eop(eop), .packet_video(packet_video),
	.ex_raw(ex_raw), .ey_raw(ey_raw), .mass(mass), .current_ex_raw(current_ex_raw), .current_ey_raw(current_ey_raw), .current_mass(current_mass),
	.pixels_counter_frame(pixels_counter_frame), .sop_counter(sop_counter), .eop_counter(eop_counter), .mask(mask), .video_packet_counter(video_packet_counter)
);

endmodule
