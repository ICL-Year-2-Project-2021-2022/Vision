module RGB_TO_HSV_COMB_TB ();
	logic [23:0] rgb, hsv;


    initial begin
        $dumpfile("waves.vcd");
        $dumpvars(0, RGB_TO_HSV_COMB_TB);
    end
	
	
	
		 
	initial begin
		int count = 0;
		int sqerr = 0;
		int fix_hue, fix_saturation, fix_value;
		int step = 64;
		real red, green, blue, cmin, cmax, delta,  hue, value, saturation;
		int err;
			
		#2;
		
		for (int r = 0; r < 255;  r = r + step) begin
			for (int g = 0; g < 255; g = g + step) begin
				for (int b = 0; b < 255; b = b + step) begin
							
					red = real'(r)/255;
					green = real'(g)/255;
					blue = real'(b)/255;
									
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
					end

					//Delta
					delta = cmax-cmin;
					value = int'(255*cmax);
					
					//Hue
					if (red >= blue & red >= green)begin
						if (green >= blue) hue = int'(30 * ((green - blue)/delta));
						else hue = int'(180 + 30 * ((green - blue)/delta));
				
					end else if (green > red & green >= blue) hue = int'(30 * ((blue - red)/delta + 2));
					else hue = int'(30 * ((red - green)/delta +4));
					
					//saturation
					if(cmax == 0) saturation = int'(0);
					else saturation = int'(255*(delta/cmax));
					
					#1;
					rgb = r*(16**4)+ g*(16**2) + b;
					
					#3;
					
					fix_hue = hsv[23:16];
					fix_saturation = hsv[15:8];
					fix_value = hsv[7:0];
					
					err = int'( (hue - fix_hue)**2 + (saturation-fix_saturation)**2 + (value - fix_value)**2); //
					sqerr = sqerr + err;
					count = count + 1;
					
					$display ("For RGB: %0d %0d %0d, HSV_ref: %0d %0d %0d, HSV_fix: %0d %0d %0d, Err %d", r, g, b, hue, saturation, value, fix_hue, fix_saturation, fix_value, err);
					//$display ("For RGB: %0h %0h %0h, HSV_ref: %0h %0h %0h, HSV_fix: %0h %0h %0h, Error: %0d ", r, g, b, hue, saturation, value, fix_hue, fix_saturation, fix_value, err);
				end
			end
		end
		$display ("MSE: %f", real'(sqerr)/real'(count));
	end
	

RGB_TO_HSV_COMB dut( .hsv(hsv),.rgb(rgb));
endmodule