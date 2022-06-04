module RGB_TO_HSV_COMB(
	input logic	[23:0] rgb,
	output logic [23:0] hsv
	
);

////////////////////////////////////////////////////////////////////////
parameter POINT = 24;

///////////////////////////////////////////////////////////////////////

//Division lookup table 
logic signed [POINT+2:0] div_lookup[255:0];
logic[7:0] hue, saturation, value, cmin, cmax, delta;
logic[31:0] saturation_fixed;
logic signed[64:0] hue_debug, hue_step1_debug;
logic [7:0] hue_cond_debug;

logic[8:0] red, green, blue;
logic signed[8:0] gmb, bmr, rmg;
logic[POINT+10:0] hue_step1;

//Debug logic
logic[POINT+2 + 8:0] saturation_intermediate;

//Lookup table initialization

initial begin
	div_lookup[0] = 0;
	for (int i = 1; i < 256; i = i+1)  begin
		div_lookup[i] = (32'h1<<POINT)/i;
	end
end

always @(*) begin
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
	
	//Hue calculation
	if (red >= blue & red >= green) begin 
		if (green >= blue) begin
			hue_step1 = ((gmb)*div_lookup[delta]);
		end else begin
			hue_step1 = ((gmb)*div_lookup[delta] + (6 << POINT));
		end
			
	end else if (green >= red & green >= blue) begin
		hue_step1 = (((bmr)*div_lookup[delta]) + (2 << POINT));
		
	end else begin
		hue_step1 = (((rmg)*div_lookup[delta]) + (4 << POINT));
	end
	//Hue scaling
	hue = (30* hue_step1)>>POINT;

	//Saturation calulation
	saturation_intermediate = delta * div_lookup[cmax];
	saturation = (255 * saturation_intermediate) >>> POINT;
	
	value = cmax;
		
	hsv = {hue, saturation, value}; 
		
end

endmodule

