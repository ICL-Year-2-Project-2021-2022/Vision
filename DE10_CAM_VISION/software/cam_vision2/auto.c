#include "auto.h"

void auto_gain(int target, int epsilon){
	int currentgain = 0x280;
	OV8865SetGain(currentgain);
	usleep(100000);

	int value = OV8865ReadAvg();
	int delta = target - value;
	int kp = 1;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta*kp;
		if (currentgain < 0) currentgain = 0;
		if (currentgain > 2047){
			IOWR(LED_BASE, 0, 511);
			usleep(1000000);
			IOWR(LED_BASE, 0, 0);
			OV8865SetGain(2047);
			return;
		}
		OV8865SetGain(currentgain);
		usleep(100000);

		value = OV8865ReadAvg();
		delta = target - value;

		if (print){
			printf("Delta: %d, gain: %d\n", delta, currentgain);
		}
		IOWR(LED_BASE, 0, delta);
	}

	IOWR(LED_BASE, 0, 0);

}

void auto_wb(int target, int epsilon){
	int delay = 100000;
	int kp = 1;


	//Red Balance
	int currentgain = 0x400;
	OV8865SetRedGain(currentgain);
	usleep(delay);
	 alt_u32 value = red_average();
	int delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		if (currentgain > 2047){
			IOWR(LED_BASE, 0, 511);
			usleep(1000000);
			IOWR(LED_BASE, 0, 0);
			OV8865SetRedGain(2047);
			break;
		}

		OV8865SetRedGain(currentgain);
		usleep(delay);

		value = red_average();

		delta = target - value;
		if(print){
			printf("Red Delta: %d, gain: %d\n", delta, currentgain);
		}
		IOWR(LED_BASE, 0, delta);
	}

	//Green Balance
	currentgain = 0x400;
	OV8865SetGreenGain(currentgain);
	usleep(delay);
	value = green_average();
	delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		if (currentgain > 2047){
			IOWR(LED_BASE, 0, 511);
			usleep(1000000);
			IOWR(LED_BASE, 0, 0);
			OV8865SetGreenGain(2047);
			break;
		}
		OV8865SetGreenGain(currentgain);
		usleep(delay);

		value = green_average();

		delta = target - value;
		if (print){
			printf("Green Delta: %d, gain: %d\n", delta, currentgain);
		}
		IOWR(LED_BASE, 0, delta);
		}

	//Blue Balance
	currentgain = 0x400;
	OV8865SetBlueGain(currentgain);
	usleep(delay);
	value = blue_average();
	delta = target - value;

	while ( delta > epsilon || delta < -epsilon){
		currentgain = currentgain + delta/2;
		if (currentgain < 0) currentgain = 0;
		if (currentgain > 2047){
			IOWR(LED_BASE, 0, 511);
			usleep(1000000);
			IOWR(LED_BASE, 0, 0);
			OV8865SetBlueGain(2047);
			break;
		}
		OV8865SetBlueGain(currentgain);
		usleep(delay);

		value = blue_average();

		delta = target - value;
		if (print){
		printf("Blue Delta: %d, gain: %d\n", delta, currentgain);
		}
		IOWR(LED_BASE, 0, delta);
	}
	IOWR(LED_BASE, 0, 0);

}
