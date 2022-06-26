#include "filters_buffers.h"

void fir_load_unit(int base){
	//Clear filter coefficients
	for (int i = 7; i < 7+9; i++){
		IOWR(base, i, 0);
	}
	IOWR(base, 11, 128);
	IOWR(base, 6, 1);
	IOWR(base, 0, 1);
}

void fir_load_avg(int base){
	for (int i = 7; i < 7+9; i++){
		IOWR(base, i, 14);
	}
	IOWR(base, 6, 1);
	IOWR(base, 0, 1);
}


void fir_load_sobel(int base){
	IOWR(base, 7, -128);
	IOWR(base, 8, 0);
	IOWR(base, 9, 128);
	IOWR(base, 10, -128);
	IOWR(base, 11, 0);
	IOWR(base, 12, 128);
	IOWR(base, 13, -128);
	IOWR(base, 14, 0);
	IOWR(base, 15, 128);

	IOWR(base, 6, 1);
	IOWR(base, 0, 1);

}

void fir_load_inv_sobel(int base){
	IOWR(base, 7, 128);
	IOWR(base, 8, 0);
	IOWR(base, 9, -128);
	IOWR(base, 10, 128);
	IOWR(base, 11, 0);
	IOWR(base, 12, -128);
	IOWR(base, 13, 128);
	IOWR(base, 14, 0);
	IOWR(base, 15, -128);

	IOWR(base, 6, 1);
	IOWR(base, 0, 1);

}

void fir_load_vertical(int base){
	IOWR(base, 7, 0);
	IOWR(base, 8, 43);
	IOWR(base, 9, 0);
	IOWR(base, 10, 0);
	IOWR(base, 11, 43);
	IOWR(base, 12, 0);
	IOWR(base, 13, 0);
	IOWR(base, 14, 43);
	IOWR(base, 15, 0);

	IOWR(base, 6, 1);
	IOWR(base, 0, 1);

};


void print_image(){
	IOWR(PIXEL_BUFFER_0_BASE, 5000, 1);
	usleep(10);

	int count = 0;

	for(int y = 0; y <60; y++){
		for (int x = 0; x < 80; x++){
			alt_u32 pixel =(IORD(PIXEL_BUFFER_0_BASE, 64*x + y));
			alt_u8 chr = (alt_u8) ((pixel)& 0x000000ff);

			if (chr == '\n') chr--;
			if(chr == ',') chr--;

			alt_putchar(chr);

			count++;
			if (count == 60) {
				count = 0;
				if (y == 59 && x == 79){
					alt_putchar('\n');
				} else {
					alt_putchar(',');
				}
				while(alt_getchar() != 'A'){
						usleep(1);
					}
			}
		}
	}

	IOWR(PIXEL_BUFFER_0_BASE, 5000, 0);

	return;
}

void print_wb_buffer(){
	IOWR(PIXEL_BUFFER_WB_0_BASE, 5000, 1);
		usleep(10);

		for(int y = 0; y <15; y++){
			for (int x = 0; x < 20; x++){
				alt_u32 pixel =(IORD(PIXEL_BUFFER_WB_0_BASE, 16*x + y));
				printf("%x,", pixel);
			}

			alt_putchar('\n');
		}

		IOWR(PIXEL_BUFFER_WB_0_BASE, 5000, 0);

}

int green_average(){
	int sum = 0;
	for(int y = 0; y <15; y++){
		for (int x = 0; x < 20; x++){
			alt_u32 pixel =(IORD(PIXEL_BUFFER_WB_0_BASE, 16*x + y));
			alt_u32 green = (pixel & 0x0000ff00) >> 8;
			sum += green;
			}
		}
	return sum / (15*20);
}

int red_average(){
	int sum = 0;
	for(int y = 0; y <15; y++){
		for (int x = 0; x < 20; x++){
			alt_u32 pixel =(IORD(PIXEL_BUFFER_WB_0_BASE, 16*x + y));
			alt_u32 red = (pixel & 0x00ff0000)>>16;
			sum += red;
			}
		}
	return sum / (15*20);
}

int blue_average(){
	int sum = 0;
	for(int y = 0; y <15; y++){
		for (int x = 0; x < 20; x++){
			alt_u32 pixel =(IORD(PIXEL_BUFFER_WB_0_BASE, 16*x + y));
			alt_u32 blue = pixel & 0x000000ff;
			sum += blue;
			}
		}
	return sum / (15*20);
}
