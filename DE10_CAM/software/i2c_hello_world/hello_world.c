#include "system.h"
#include "altera_avalon_i2c.h"
#include "altera_avalon_i2c_regs.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include "sys/alt_irq.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


int main()
{
	ALT_AVALON_I2C_DEV_t *i2c_dev; //pointer to instance structure
    alt_u8 txbuffer[4];
	alt_u8 rxbuffer[4];
	alt_u32 *speed_in_hz;
	int i;
	ALT_AVALON_I2C_STATUS_CODE status;

	//get a pointer to the avalon i2c instance
	i2c_dev = alt_avalon_i2c_open("/dev/i2c_0");
	if (NULL==i2c_dev) {
		printf("Error: Cannot find /dev/i2c_0\n");
		return 1;
	} else {
		printf("I2C driver found: /dev/i2c_0\n");
    }


	ALT_AVALON_I2C_MASTER_CONFIG_t *i2c_dev_config;
	alt_avalon_i2c_master_config_get(i2c_dev, i2c_dev_config);
	alt_avalon_i2c_master_config_speed_set(i2c_dev, i2c_dev_config, 100000);
	alt_avalon_i2c_master_config_set(i2c_dev, i2c_dev_config);
	alt_avalon_i2c_master_config_speed_get(i2c_dev, i2c_dev_config, speed_in_hz);
	printf("Speed: %d \n", *speed_in_hz);
	/*alt_avalon_i2c_master_config_set(i2c_dev, i2c_dev_config);*/

 	//set the address of the device using
	alt_avalon_i2c_master_target_set(i2c_dev,0x20);
	//write data to an eeprom at address 0x020

	txbuffer[0] = 2;
	txbuffer[1] = 0;
	txbuffer[2] = 1;
	txbuffer[2] = 3;

	while(1) {

		status = alt_avalon_i2c_master_tx(i2c_dev,txbuffer, 0x4, ALT_AVALON_I2C_NO_INTERRUPTS);
		//printf("Status TX: %d\n", status);
		//usleep(1000000);
	}
}
