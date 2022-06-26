#include "input.h"


void process_input(){
	static alt_u8  manual_focus_step = 50;
	static alt_u16  current_focus = 300;
	alt_u32 exposureTime = EXPOSURE_INIT;
	alt_u16 gain = GAIN_INIT;

	// touch KEY0 to trigger Auto focus
		   if((IORD(KEY_BASE,0)&0x03) == 0x02){
			   usleep(100000);
	    	   current_focus = Focus_Window(320,240);
	    	   usleep(100000);
	       }
		   // touch KEY1 with selected routine to run
		   else if ((IORD(KEY_BASE,0)&0x03) == 0x01){
	    	   int sw = (IORD(SW_BASE, 0)& 0x03FF);
	    	   IOWR(LED_BASE,0,511);
	    	   usleep(500000);
	    	   IOWR(LED_BASE,0,0);
	    	   usleep(500000);

	    	   switch(sw){
				   case 1 : {
					   auto_gain(120 , 2);
					   break;
				   }case 2: {
					   auto_wb(120 , 3);
					   break;
				   }case 4: {
					   IOWR(RGB_TO_HSV_BASE, HSV_ENABLED, ~(IORD(RGB_TO_HSV_BASE, HSV_ENABLED)&0x1));
					   if(print){
						   printf("HSV: %x\n", IORD(RGB_TO_HSV_BASE, HSV_ENABLED));
					   }
					   break;
				   }case 8: {
					   IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, ~(IORD(COLOR_FILTER_0_BASE, APPLY_MASK)&0x1));
					   if(print){
						   printf("MASK:  %x\n", IORD(COLOR_FILTER_0_BASE, APPLY_MASK));
					   }
					   break;
				   }case 16: {
					    IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, ~(IORD(COM_COUNTER_0_BASE, THRESH_ENABLED)&0x1));

					   if(print){
						   printf("Thresh:  %x\n", IORD(COLOR_FILTER_0_BASE, THRESH_ENABLED));
					   }
					   break;
				   }case 256: {
					   halt = !halt;
					   break;
				   }case 512: {
					   slow = !slow;
					   break;
				   }case 3: {
					   fir_load_unit(FIR_0_0_BASE);
					   fir_load_unit(FIR_0_1_BASE);
					   break;
				   }case 5: {
					   fir_load_avg(FIR_0_0_BASE);
					   fir_load_unit(FIR_0_1_BASE);
					   break;
				   }case 6: {
					   fir_load_sobel(FIR_0_0_BASE);
					   fir_load_unit(FIR_0_1_BASE);
					   break;
				   } case 768: {
					   one = !one;
					   break;
				   }
	    	   }

		   usleep(100000);

	       }

		   //Process input commands
		  int in = alt_getchar();
		  switch (in) {
			   case 'R': {
				   //Process request
				   print_observations(observations);
				   break;}
			   case 'P': {
				   //Process request
				   print_image();
				   break;}

			   case 'H': {
					   for(int i = 0; i<20; i++){
						   alt_printf("%x ", histogram[i]);
						   if( i == 19 ){
							   alt_putchar('\n');
						   }else{
							   alt_putchar(' ');
						   }
					   }
				   break;}
			   case 'D': {
				   	   IOWR(COLOR_FILTER_0_BASE, APPLY_MASK, 0);
					   fir_load_sobel(FIR_0_0_BASE);
					   fir_load_vertical(FIR_0_1_BASE);
					   //COM counter setup
					   IOWR(COM_COUNTER_0_BASE, THRESH_ENABLED, 1);
					   IOWR(COM_COUNTER_0_BASE, THRESH_Y, 200);
					   IOWR(COM_COUNTER_0_BASE, THRESH_X, 10);
					   IOWR(COM_COUNTER_0_BASE, THRESHOLD_GATE, 80);

					   usleep(delay);
					   //printf("D\n");

					   printf("%x %x %x %x", IORD(OBSTACLE_DIST_0_BASE, STRIPE_COUNT),
							   IORD(OBSTACLE_DIST_0_BASE, STRIPE_DIST), IORD(OBSTACLE_DIST_0_BASE, STRIPE_POS_MIN),
							   IORD(OBSTACLE_DIST_0_BASE, STRIPE_POS_MAX));
				   break;}
			   case 'S': {
				   halt = !halt;
				   break;}
			   case 'C': {
				   //Autocalibration
				   	Focus_Init();
				   	auto_gain(160,2);
				   	auto_wb(128 ,2);
				   	auto_gain(160,2);
				   	Focus_Init();
					break;}

			   case 'e': {
				   exposureTime += EXPOSURE_STEP;
				   OV8865SetExposure(exposureTime);
				   //printf("\nExposure = %x ", exposureTime);
				   break;}
			   case 'd': {
				   exposureTime -= EXPOSURE_STEP;
				   OV8865SetExposure(exposureTime);
				   //printf("\nExposure = %x ", exposureTime);
				   break;}
			   case 't': {
				   gain += GAIN_STEP;
				   OV8865SetGain(gain);
				   //printf("\nGain = %x ", gain);
				   break;}
			   case 'g': {
				   gain -= GAIN_STEP;
				   OV8865SetGain(gain);
				   //printf("\nGain = %x ", gain);
				   break;}
			   case 'r': {
				   current_focus += manual_focus_step;
				   if(current_focus >1023) current_focus = 1023;
				   OV8865_FOCUS_Move_to(current_focus);
				   //printf("\nFocus = %x ",current_focus);
				   break;}
			   case 'f': {
				   if(current_focus > manual_focus_step) current_focus -= manual_focus_step;
				   OV8865_FOCUS_Move_to(current_focus);
				   //printf("\nFocus = %x ",current_focus);
				   break;}
		  	  };

}
