module RGB_TO_HSV_COMB(
	input logic advance,
	input logic clk,
	input logic	[23:0] rgb,
	output logic [23:0] hsv
);


///////////////////////////////////////////////////////////////////////

//Division lookup table 
logic signed [14:0] div_lookup[255:0];
logic signed[8:0] gmb, bmr, rmg;

logic[7:0] hue, saturation, value, cmin, cmax, delta;
logic[8:0] red, green, blue;
logic[17:0] hue_step1, hue_step3;
logic signed[17:0] hue_step2_1;
logic signed[17:0] hue_step2_2;
logic signed [8:0] base;
logic signed[3:0] modifier;
logic signed [16:0] hue_debug;
logic base_sign;
logic [7:0] base_magnitude;
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

always @(*) begin
	//Step 1 -> Pre cmax, delta calculation, formatting
	
	//RGB splitting
	red = {1'b0, rgb[23:16]};
	green = {1'b0, rgb[15:8]};
	blue = {1'b0, rgb[7:0]};
	
	//Differences
	gmb = green - blue;
	bmr = blue - red;
	rmg = red - green;
	
	//Cmax calculation
	if (red >= blue & red >= green) begin 
		cmax = red;
	end else if (green >= red & green >= blue) begin
		cmax = green;
	end else begin
		cmax = blue;
	end
	
	//Cmin calculation
	if (red <= blue & red <= green) cmin = red;
	else if (green <= red & green <= blue) cmin = green;
	else cmin = blue;
	
	//Delta
	delta = cmax-cmin;
	hue_lookup_original = div_lookup[delta];
	hue_lookup = div_lookup[delta];
	

	//Hue calculation
	if (red >= blue & red >= green) begin 
		if (green >= blue) begin
			base = gmb;
			modifier = 0;
		end else begin
			base = gmb;
			modifier = 6;
		end
			
	end else if (green >= red & green >= blue) begin
		base = bmr;
		modifier = 2;
		
	end else begin
		base = rmg;
		modifier = 4;
	end
	
	base_sign = base[8];
	if (base_sign) base_magnitude = -base;
	else base_magnitude = base;
	
	//Step 2 -> Multiplication
	//hue_step1 = base*hue_lookup_original;
	
	hue_step2_1 = base_magnitude * hue_lookup[6:0];
	hue_step2_2 = base_magnitude * hue_lookup[14:7];
	
	
	//Step 3 -> Merge
	if (base_sign) begin
		hue_step1 = -(hue_step2_1 + (hue_step2_2 << 7));
	end else begin 
		hue_step1 = (hue_step2_1 + (hue_step2_2 << 7));
	end
	
	
	hue_step3 = hue_step1 + (modifier<<<12);
	hue = (30* (hue_step3))>>12;


	//Pipelined saturation
	
	//Step 1 - > cmax & lookup
	saturation_lookup = div_lookup[cmax];

	
	
	//Step 2 -> Multiplication
	saturation_step2_1 = delta * saturation_lookup[6:0];
	saturation_step2_2 = delta * saturation_lookup[14:7];
	
	//Step 3 -> Merge
	saturation_step3 = saturation_step2_1 + (saturation_step2_2<<7);
	saturation = ((saturation_step3<<8) - saturation_step3) >> 12;
	

	value = cmax;
		
	hsv = {hue, saturation, value}; 
		
end

endmodule
