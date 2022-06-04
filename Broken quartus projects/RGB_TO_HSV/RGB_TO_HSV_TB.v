module RGB_TO_HSV_TB ();
    logic clk, reset_n, sink_sop;
	logic [23:0] source_data, sink_data;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, RGB_TO_HSV_TB);

        clk = 0;
		sink_data = 24'hC8193C;
		sink_sop = 0;
		
		repeat (100) begin
            #1 clk = ~clk;
         end

         
		 
		//Floating point implementation
    end
	
	
	
		 
	initial begin
		int count = 0;
		int sqerr = 0;
		int fix_hue, fix_saturation, fix_value;
		int step = 64;
		real red, green, blue, cmin, cmax, delta,  hue, value, saturation;
		int err;
		
		$display ("Mod: %0d %0d %0d %0d", $rtoi(0.2), $rtoi(0.6), $rtoi(-0.6), $rtoi(-0.2));
		
		reset_n = 0;
		#4;
		reset_n = 1;
		
		#2;
		
		for (int r = 1; r < 255;  r = r + step) begin
			for (int g = 2; g < 255; g = g + step) begin
				for (int b = 3; b < 255; b = b + step) begin
					
					
					red = real'(r)/255;
					green = real'(g)/255;
					blue = real'(b)/255;
					
					//$display ("Next measurement: %0h %0h %0h", r, g, b);
					
					//$display ("red %f, green %f, blue %f, ", red, green, blue);
	
					
					hue = 0;
					
					//Cmin calculation
					if (red <= blue && red <= green) begin 
						cmin = red;
					end	else if (green <= red && green <= blue) begin 
						cmin = green;
					end else begin 
						cmin = blue;
					end
					
					//$display ("Cmin %f:", cmin);
					
					//cmax 
					if (red >= blue && red >= green)begin 
						cmax = red;
					end else if (green >= red && green >= blue)begin					
						cmax = green;
					end else begin 
						cmax = blue;
						//$display ("Went blue");
					end
					
					//$display ("Cmax %f:", cmax);
					
					//Delta
					delta = cmax-cmin;
					value = int'(255*cmax);
					
					//Hue
					if (red >= blue & red >= green)begin
						if (green >= blue) hue = int'(30 * ((green - blue)/delta));
						else hue = int'(180 + 30 * ((green - blue)/delta));
				
					end else if (green > red & green >= blue) hue = int'(30 * ((blue - red)/delta + 2));
					else hue = int'(30 * ((red - green)/delta +4));
					
					
					
					//$display ("Delta: %f, cmax: %f", 255*delta, 255*cmax);
					//$display ("Delta/cmax: %f", 255*(delta/cmax));
					
					//saturation
					if(cmax == 0) saturation = int'(0);
					else saturation = int'(255*(delta/cmax));

					count = count + 1;
					
					#1;
					sink_data = r*(16**4)+ g*(16**2) + b;
					
					//$display ("sink data: %0h", sink_data);
					#3;
					
					fix_hue = source_data[23:16];
					fix_saturation = source_data[15:8];
					fix_value = source_data[7:0];
					
					err = int'( (hue - fix_hue)**2 + (saturation-fix_saturation)**2 + (value - fix_value)**2); //
					sqerr = sqerr + err;
					
					
					$display ("For RGB: %0d %0d %0d, HSV_ref: %0d %0d %0d, HSV_fix: %0d %0d %0d, Err %d", r, g, b, hue, saturation, value, fix_hue, fix_saturation, fix_value, err);
					//$display ("For RGB: %0h %0h %0h, HSV_ref: %0h %0h %0h, HSV_fix: %0h %0h %0h, Error: %0d ", r, g, b, hue, saturation, value, fix_hue, fix_saturation, fix_value, err);
				end
			end
		end
	end
	

RGB_TO_HSV dut( .source_data(source_data),.sink_data(sink_data), .sink_sop(sink_sop)); //.clk(clk), .reset_n(reset_n),
endmodule