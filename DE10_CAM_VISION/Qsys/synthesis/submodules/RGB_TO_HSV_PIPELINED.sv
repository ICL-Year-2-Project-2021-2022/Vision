module RGB_TO_HSV_PIPELINED(
	input logic advance,
	input logic clk,
	input logic reset,
	input logic	[23:0] rgb,
	output logic [23:0] hsv
);


///////////////////////////////////////////////////////////////////////

//Division lookup table 
logic signed [14:0] div_lookup[255:0];
logic signed[8:0] gmb, bmr, rmg;

logic[7:0] hue, saturation, value, cmin, cmin_comb, cmax, cmax2, cmax_comb, delta, delta_comb;
logic[8:0] red, green, blue;
logic[17:0] hue_step3_1, hue_step3_2;
logic signed[17:0] hue_step2_1, hue_step2_2;
logic signed [8:0] base;
logic signed[3:0] modifier, modifier_2, modifier_comb;
logic signed [16:0] hue_debug;
logic base_sign, base_sign_1, base_sign_2;
logic [7:0] base_magnitude, base_magnitude_comb;
logic [14:0] saturation_lookup, saturation_step1, saturation_step2_1, saturation_step2_2;
logic [14:0] hue_lookup;
logic signed[14:0] hue_lookup_original;
logic [14:0] hue_lookup_unsigned;
logic [22:0] saturation_step3;





//Lookup table initialization

initial begin
	div_lookup[0] = 0;
	for (int i = 1; i < 256; i = i+1)  begin
		div_lookup[i] = (1<<12)/i;
	end
end

always @(posedge clk) begin

	if (~reset) begin
		gmb <= 0;
		bmr <= 0;
		rmg <= 0;
		delta <= 0;
		hue_lookup <= 0;
		hue_step2_1 <= 0;
		hue_step2_2 <= 0;
		saturation_lookup <= 0;
		saturation_step2_1 <= 0;
		saturation_step2_2 <= 0;
		cmax <= 0;
		cmax2 <= 0;
		modifier <= 0;
		modifier_2 <=0;
	
	end else begin
		//Hue
		//Step 1 ffs
		//Differences
		
		
		modifier <= modifier_comb;
		
		base_magnitude <= base_magnitude_comb;
		base_sign_1 <= base_sign;
		base_sign_2 <= base_sign_1;
		
		//Delta
		delta <= delta_comb;
		hue_lookup <= div_lookup[delta_comb];
		modifier <= modifier_comb;
		modifier_2 <= modifier;
		
		//Hue step 2 -> Multiplication	
		hue_step2_1 <= base_magnitude * hue_lookup[6:0];
		hue_step2_2 <= base_magnitude * hue_lookup[14:7];
		
		//Saturation
			//Step 1 - > cmax & lookup
		saturation_lookup <= div_lookup[cmax_comb];

		
		
		//Step 2 -> Multiplication
		saturation_step2_1 <= delta * saturation_lookup[6:0];
		saturation_step2_2 <= delta * saturation_lookup[14:7];
		
		
		//Value
		cmax  <= cmax_comb;
		cmax2 <=cmax;
		
	end

end

always @(*) begin
	//Step 1 -> Pre cmax, delta calculation, formatting
	
	//RGB splitting
	red = {1'b0, rgb[23:16]};
	green = {1'b0, rgb[15:8]};
	blue = {1'b0, rgb[7:0]};
	

	
	//Cmax calculation
	if (red >= blue & red >= green) begin 
		cmax_comb = red;
	end else if (green >= red & green >= blue) begin
		cmax_comb = green;
	end else begin
		cmax_comb = blue;
	end
	
	//Cmin calculation
	if (red <= blue & red <= green) cmin_comb = red;
	else if (green <= red & green <= blue) cmin_comb = green;
	else cmin_comb = blue;
	
	delta_comb = cmax_comb-cmin_comb;
	
	
	//Hue calculation
	if (red >= blue & red >= green) begin 
		if (green >= blue) begin
			base = green - blue;
			modifier_comb = 0;
		end else begin
			base = green - blue;
			modifier_comb = 6;
		end
			
	end else if (green >= red & green >= blue) begin
		base = blue - red;
		modifier_comb = 2;
		
	end else begin
		base = red - green;
		modifier_comb = 4;
	end
	
	 base_sign = base[8];
	 
	if (base_sign) base_magnitude_comb = -base;
	else base_magnitude_comb = base;
	

	
	//Step 3 -> Merge
	if (base_sign_2) begin
		hue_step3_1 = -(hue_step2_1 + (hue_step2_2 << 7));
	end else begin 
		hue_step3_1 = (hue_step2_1 + (hue_step2_2 << 7));
	end
	
	//Step4
	
	
	hue_step3_2 = hue_step3_1 + (modifier_2<<<12);
	hue = (30* (hue_step3_2))>>12;


	//Pipelined saturation
	

	//Step 3 -> Merge
	saturation_step3 = saturation_step2_1 + (saturation_step2_2<<7);
	saturation = ((saturation_step3<<8) - saturation_step3) >> 12;
	

	value = cmax2;
		
	hsv = {hue, saturation, value}; 
		
end

endmodule

