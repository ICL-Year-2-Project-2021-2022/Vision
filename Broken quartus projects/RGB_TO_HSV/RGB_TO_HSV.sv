module RGB_TO_HSV(
	//global clock & reset
	clk,
	reset_n,
	
	// stream sink
	sink_data,
	sink_valid,
	sink_ready,
	sink_sop,
	sink_eop,
	
	// streaming source
	source_data,
	source_valid,
	source_ready,
	source_sop,
	source_eop
	
);

// global clock & reset
input logic	clk;
input logic reset_n;

// streaming sink
input logic	[23:0]            	sink_data;
input logic						sink_valid;
output logic					sink_ready;
input logic						sink_sop;
input logic						sink_eop;

// streaming source
output logic [23:0]			  	   	source_data;
output logic						source_valid;
input logic								source_ready;
output logic							source_sop;
output logic							source_eop;


////////////////////////////////////////////////////////////////////////
//
parameter IMAGE_W = 11'd640;
parameter IMAGE_H = 11'd480;
parameter POINT = 12;


///////////////////////////////////////////////////////////////////////

//Division lookup table 
logic [POINT+4:0] div_lookup[255:0];
logic[7:0]hue, saturation, value, cmin, cmax, delta;
logic[31:0] saturation_fixed, hue_lookup;
//logic[24:0] hue;
logic[23:0] hue_debug;
logic[7:0] hue_cond_debug;

logic[8:0] red, green, blue;
logic[8:0] gmb, bmr, rmg;
logic[POINT+10:0] hue_step1;

//Debug logics
logic[23:0] saturation_lookup;
logic[31:0] saturation_intermediate_a, saturation_intermediate_b;


//Lookup table initialization

initial begin
	div_lookup[0] = 0;
	for (int i = 1; i < 256; i = i+1)  begin
		div_lookup[i] = (32'h1<<POINT)/i;
	end
end





always @(*) begin
	//Mapping i to o
	source_valid = sink_valid;
	sink_ready = source_ready;
	source_sop = sink_sop;
	source_eop = sink_eop;
	
	
	

	//RGB splitting
	red = {1'b0, sink_data[23:16]};
	green = {1'b0, sink_data[15:8]};
	blue = {1'b0, sink_data[7:0]};
	
	//Differences
	gmb = green - blue;
	bmr = blue - red;
	rmg = red - green;
	

	


	
	if (sink_sop)begin
		//source_data = sink_data;
	end else begin
		//Debug stuff
		hue_debug = (delta*div_lookup[delta]);
		hue_lookup = div_lookup[delta];
			
		//Value/cmax calculation
		if (red >= blue & red >= green) begin 
			cmax = red;
		
			if (green >= blue) begin
				hue_step1 = ((gmb)*div_lookup[delta]);
				hue_cond_debug = 1;
			end else begin
				hue_step1 = ((gmb)*div_lookup[delta] + (2 << POINT));
				hue_cond_debug = 2;
			end
				
		end else if (green >= red & green >= blue) begin
			cmax = green;
			hue_step1 = (((bmr)*div_lookup[delta]) + (2 << POINT));
			hue_cond_debug =3;
			
		end else begin
			cmax = blue;
			hue_step1 = (((rmg)*div_lookup[delta]) + (4 << POINT));
			hue_cond_debug = 4;
		end
		//Hue scaling
		hue = (30* hue_step1)>>POINT;
		value = cmax;
		
		//Cmin calculation
		if (red <= blue & red <= green) cmin = red;
		else if (green <= red & green <= blue) cmin = green;
		else cmin = blue;
		
		//Delta
		delta = cmax-cmin;
		
		//Saturation calulation
		saturation_intermediate_a = delta * div_lookup[cmax];
		saturation_intermediate_b = (255 * saturation_intermediate_a) >>> POINT;
		saturation_lookup = div_lookup[cmax];
		saturation = saturation_intermediate_b;
		
		source_data = {hue, saturation, value}; 
		
	end
	
	
	
end

endmodule

