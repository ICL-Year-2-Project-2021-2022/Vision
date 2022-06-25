/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_gen2' in SOPC Builder design 'Qsys'
 * SOPC Builder design path: ../../Qsys.sopcinfo
 *
 * Generated: Fri Jun 24 19:14:26 BST 2022
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * COLOR_FILTER_0 configuration
 *
 */

#define ALT_MODULE_CLASS_COLOR_FILTER_0 COLOR_FILTER
#define COLOR_FILTER_0_BASE 0x33ac0
#define COLOR_FILTER_0_IRQ -1
#define COLOR_FILTER_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define COLOR_FILTER_0_NAME "/dev/COLOR_FILTER_0"
#define COLOR_FILTER_0_SPAN 64
#define COLOR_FILTER_0_TYPE "COLOR_FILTER"


/*
 * COLOR_FILTER_1 configuration
 *
 */

#define ALT_MODULE_CLASS_COLOR_FILTER_1 COLOR_FILTER
#define COLOR_FILTER_1_BASE 0x33900
#define COLOR_FILTER_1_IRQ -1
#define COLOR_FILTER_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define COLOR_FILTER_1_NAME "/dev/COLOR_FILTER_1"
#define COLOR_FILTER_1_SPAN 64
#define COLOR_FILTER_1_TYPE "COLOR_FILTER"


/*
 * COLOR_FILTER_2 configuration
 *
 */

#define ALT_MODULE_CLASS_COLOR_FILTER_2 COLOR_FILTER
#define COLOR_FILTER_2_BASE 0x338c0
#define COLOR_FILTER_2_IRQ -1
#define COLOR_FILTER_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define COLOR_FILTER_2_NAME "/dev/COLOR_FILTER_2"
#define COLOR_FILTER_2_SPAN 64
#define COLOR_FILTER_2_TYPE "COLOR_FILTER"


/*
 * COM_COUNTER_0 configuration
 *
 */

#define ALT_MODULE_CLASS_COM_COUNTER_0 COM_COUNTER
#define COM_COUNTER_0_BASE 0x33a80
#define COM_COUNTER_0_IRQ -1
#define COM_COUNTER_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define COM_COUNTER_0_NAME "/dev/COM_COUNTER_0"
#define COM_COUNTER_0_SPAN 64
#define COM_COUNTER_0_TYPE "COM_COUNTER"


/*
 * COM_COUNTER_1 configuration
 *
 */

#define ALT_MODULE_CLASS_COM_COUNTER_1 COM_COUNTER
#define COM_COUNTER_1_BASE 0x33940
#define COM_COUNTER_1_IRQ -1
#define COM_COUNTER_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define COM_COUNTER_1_NAME "/dev/COM_COUNTER_1"
#define COM_COUNTER_1_SPAN 64
#define COM_COUNTER_1_TYPE "COM_COUNTER"


/*
 * COM_COUNTER_2 configuration
 *
 */

#define ALT_MODULE_CLASS_COM_COUNTER_2 COM_COUNTER
#define COM_COUNTER_2_BASE 0x33880
#define COM_COUNTER_2_IRQ -1
#define COM_COUNTER_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define COM_COUNTER_2_NAME "/dev/COM_COUNTER_2"
#define COM_COUNTER_2_SPAN 64
#define COM_COUNTER_2_TYPE "COM_COUNTER"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x00031020
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 70000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x12
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 4096
#define ALT_CPU_EXCEPTION_ADDR 0x00020020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 70000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x12
#define ALT_CPU_NAME "nios2_gen2"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x00020000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x00031020
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 70000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x12
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 4096
#define NIOS2_EXCEPTION_ADDR 0x00020020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x12
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x00020000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_AVALON_UART
#define __ALTERA_NIOS2_GEN2
#define __ALTPLL
#define __ALT_VIP_CL_2DFIR
#define __COLOR_FILTER
#define __COM_COUNTER
#define __EDGE_BINS
#define __I2C_OPENCORES
#define __OBSTACLE_DIST
#define __PIXEL_BUFFER
#define __PIXEL_BUFFER_WB
#define __PIXEL_GRABBER
#define __RGB_TO_HSV
#define __TERASIC_AUTO_FOCUS


/*
 * EDGE_BINS_0 configuration
 *
 */

#define ALT_MODULE_CLASS_EDGE_BINS_0 EDGE_BINS
#define EDGE_BINS_0_BASE 0x33800
#define EDGE_BINS_0_IRQ -1
#define EDGE_BINS_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define EDGE_BINS_0_NAME "/dev/EDGE_BINS_0"
#define EDGE_BINS_0_SPAN 128
#define EDGE_BINS_0_TYPE "EDGE_BINS"


/*
 * OBSTACLE_DIST_0 configuration
 *
 */

#define ALT_MODULE_CLASS_OBSTACLE_DIST_0 OBSTACLE_DIST
#define OBSTACLE_DIST_0_BASE 0x33980
#define OBSTACLE_DIST_0_IRQ -1
#define OBSTACLE_DIST_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define OBSTACLE_DIST_0_NAME "/dev/OBSTACLE_DIST_0"
#define OBSTACLE_DIST_0_SPAN 64
#define OBSTACLE_DIST_0_TYPE "OBSTACLE_DIST"


/*
 * PIXEL_BUFFER_0 configuration
 *
 */

#define ALT_MODULE_CLASS_PIXEL_BUFFER_0 PIXEL_BUFFER
#define PIXEL_BUFFER_0_BASE 0x0
#define PIXEL_BUFFER_0_IRQ -1
#define PIXEL_BUFFER_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_BUFFER_0_NAME "/dev/PIXEL_BUFFER_0"
#define PIXEL_BUFFER_0_SPAN 65536
#define PIXEL_BUFFER_0_TYPE "PIXEL_BUFFER"


/*
 * PIXEL_BUFFER_WB_0 configuration
 *
 */

#define ALT_MODULE_CLASS_PIXEL_BUFFER_WB_0 PIXEL_BUFFER_WB
#define PIXEL_BUFFER_WB_0_BASE 0x33000
#define PIXEL_BUFFER_WB_0_IRQ -1
#define PIXEL_BUFFER_WB_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_BUFFER_WB_0_NAME "/dev/PIXEL_BUFFER_WB_0"
#define PIXEL_BUFFER_WB_0_SPAN 2048
#define PIXEL_BUFFER_WB_0_TYPE "PIXEL_BUFFER_WB"


/*
 * PIXEL_GRABBER_HSV configuration
 *
 */

#define ALT_MODULE_CLASS_PIXEL_GRABBER_HSV PIXEL_GRABBER
#define PIXEL_GRABBER_HSV_BASE 0x339c0
#define PIXEL_GRABBER_HSV_IRQ -1
#define PIXEL_GRABBER_HSV_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_GRABBER_HSV_NAME "/dev/PIXEL_GRABBER_HSV"
#define PIXEL_GRABBER_HSV_SPAN 64
#define PIXEL_GRABBER_HSV_TYPE "PIXEL_GRABBER"


/*
 * PIXEL_GRABBER_RGB configuration
 *
 */

#define ALT_MODULE_CLASS_PIXEL_GRABBER_RGB PIXEL_GRABBER
#define PIXEL_GRABBER_RGB_BASE 0x33a00
#define PIXEL_GRABBER_RGB_IRQ -1
#define PIXEL_GRABBER_RGB_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_GRABBER_RGB_NAME "/dev/PIXEL_GRABBER_RGB"
#define PIXEL_GRABBER_RGB_SPAN 64
#define PIXEL_GRABBER_RGB_TYPE "PIXEL_GRABBER"


/*
 * RGB_TO_HSV configuration
 *
 */

#define ALT_MODULE_CLASS_RGB_TO_HSV RGB_TO_HSV
#define RGB_TO_HSV_BASE 0x33a40
#define RGB_TO_HSV_IRQ -1
#define RGB_TO_HSV_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RGB_TO_HSV_NAME "/dev/RGB_TO_HSV"
#define RGB_TO_HSV_SPAN 64
#define RGB_TO_HSV_TYPE "RGB_TO_HSV"


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "MAX 10"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x33c88
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x33c88
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x33c88
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "Qsys"


/*
 * TERASIC_AUTO_FOCUS_0 configuration
 *
 */

#define ALT_MODULE_CLASS_TERASIC_AUTO_FOCUS_0 TERASIC_AUTO_FOCUS
#define TERASIC_AUTO_FOCUS_0_BASE 0x33b80
#define TERASIC_AUTO_FOCUS_0_IRQ -1
#define TERASIC_AUTO_FOCUS_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define TERASIC_AUTO_FOCUS_0_NAME "/dev/TERASIC_AUTO_FOCUS_0"
#define TERASIC_AUTO_FOCUS_0_SPAN 32
#define TERASIC_AUTO_FOCUS_0_TYPE "TERASIC_AUTO_FOCUS"


/*
 * altpll_0 configuration
 *
 */

#define ALTPLL_0_BASE 0x33c70
#define ALTPLL_0_IRQ -1
#define ALTPLL_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ALTPLL_0_NAME "/dev/altpll_0"
#define ALTPLL_0_SPAN 16
#define ALTPLL_0_TYPE "altpll"
#define ALT_MODULE_CLASS_altpll_0 altpll


/*
 * altpll_1 configuration
 *
 */

#define ALTPLL_1_BASE 0x33c60
#define ALTPLL_1_IRQ -1
#define ALTPLL_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ALTPLL_1_NAME "/dev/altpll_1"
#define ALTPLL_1_SPAN 16
#define ALTPLL_1_TYPE "altpll"
#define ALT_MODULE_CLASS_altpll_1 altpll


/*
 * altpll_2 configuration
 *
 */

#define ALTPLL_2_BASE 0x33c50
#define ALTPLL_2_IRQ -1
#define ALTPLL_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ALTPLL_2_NAME "/dev/altpll_2"
#define ALTPLL_2_SPAN 16
#define ALTPLL_2_TYPE "altpll"
#define ALT_MODULE_CLASS_altpll_2 altpll


/*
 * fir_0_0 configuration
 *
 */

#define ALT_MODULE_CLASS_fir_0_0 alt_vip_cl_2dfir
#define FIR_0_0_BASE 0x32800
#define FIR_0_0_IRQ -1
#define FIR_0_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FIR_0_0_NAME "/dev/fir_0_0"
#define FIR_0_0_SPAN 2048
#define FIR_0_0_TYPE "alt_vip_cl_2dfir"


/*
 * fir_0_1 configuration
 *
 */

#define ALT_MODULE_CLASS_fir_0_1 alt_vip_cl_2dfir
#define FIR_0_1_BASE 0x31800
#define FIR_0_1_IRQ -1
#define FIR_0_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FIR_0_1_NAME "/dev/fir_0_1"
#define FIR_0_1_SPAN 2048
#define FIR_0_1_TYPE "alt_vip_cl_2dfir"


/*
 * fir_1 configuration
 *
 */

#define ALT_MODULE_CLASS_fir_1 alt_vip_cl_2dfir
#define FIR_1_BASE 0x32000
#define FIR_1_IRQ -1
#define FIR_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FIR_1_NAME "/dev/fir_1"
#define FIR_1_SPAN 2048
#define FIR_1_TYPE "alt_vip_cl_2dfir"


/*
 * fir_2 configuration
 *
 */

#define ALT_MODULE_CLASS_fir_2 alt_vip_cl_2dfir
#define FIR_2_BASE 0x30800
#define FIR_2_IRQ -1
#define FIR_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FIR_2_NAME "/dev/fir_2"
#define FIR_2_SPAN 2048
#define FIR_2_TYPE "alt_vip_cl_2dfir"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 4
#define ALT_SYS_CLK none
#define ALT_TIMESTAMP_CLK none


/*
 * i2c_opencores_camera configuration
 *
 */

#define ALT_MODULE_CLASS_i2c_opencores_camera i2c_opencores
#define I2C_OPENCORES_CAMERA_BASE 0x33ba0
#define I2C_OPENCORES_CAMERA_IRQ 1
#define I2C_OPENCORES_CAMERA_IRQ_INTERRUPT_CONTROLLER_ID 0
#define I2C_OPENCORES_CAMERA_NAME "/dev/i2c_opencores_camera"
#define I2C_OPENCORES_CAMERA_SPAN 32
#define I2C_OPENCORES_CAMERA_TYPE "i2c_opencores"


/*
 * i2c_opencores_mipi configuration
 *
 */

#define ALT_MODULE_CLASS_i2c_opencores_mipi i2c_opencores
#define I2C_OPENCORES_MIPI_BASE 0x33bc0
#define I2C_OPENCORES_MIPI_IRQ 0
#define I2C_OPENCORES_MIPI_IRQ_INTERRUPT_CONTROLLER_ID 0
#define I2C_OPENCORES_MIPI_NAME "/dev/i2c_opencores_mipi"
#define I2C_OPENCORES_MIPI_SPAN 32
#define I2C_OPENCORES_MIPI_TYPE "i2c_opencores"


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x33c88
#define JTAG_UART_IRQ 2
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * key configuration
 *
 */

#define ALT_MODULE_CLASS_key altera_avalon_pio
#define KEY_BASE 0x33c20
#define KEY_BIT_CLEARING_EDGE_REGISTER 0
#define KEY_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEY_CAPTURE 0
#define KEY_DATA_WIDTH 2
#define KEY_DO_TEST_BENCH_WIRING 0
#define KEY_DRIVEN_SIM_VALUE 0
#define KEY_EDGE_TYPE "NONE"
#define KEY_FREQ 50000000
#define KEY_HAS_IN 1
#define KEY_HAS_OUT 0
#define KEY_HAS_TRI 0
#define KEY_IRQ -1
#define KEY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define KEY_IRQ_TYPE "NONE"
#define KEY_NAME "/dev/key"
#define KEY_RESET_VALUE 0
#define KEY_SPAN 16
#define KEY_TYPE "altera_avalon_pio"


/*
 * led configuration
 *
 */

#define ALT_MODULE_CLASS_led altera_avalon_pio
#define LED_BASE 0x33c40
#define LED_BIT_CLEARING_EDGE_REGISTER 0
#define LED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_CAPTURE 0
#define LED_DATA_WIDTH 10
#define LED_DO_TEST_BENCH_WIRING 0
#define LED_DRIVEN_SIM_VALUE 0
#define LED_EDGE_TYPE "NONE"
#define LED_FREQ 50000000
#define LED_HAS_IN 0
#define LED_HAS_OUT 1
#define LED_HAS_TRI 0
#define LED_IRQ -1
#define LED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LED_IRQ_TYPE "NONE"
#define LED_NAME "/dev/led"
#define LED_RESET_VALUE 0
#define LED_SPAN 16
#define LED_TYPE "altera_avalon_pio"


/*
 * mipi_pwdn_n configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_pwdn_n altera_avalon_pio
#define MIPI_PWDN_N_BASE 0x33c00
#define MIPI_PWDN_N_BIT_CLEARING_EDGE_REGISTER 0
#define MIPI_PWDN_N_BIT_MODIFYING_OUTPUT_REGISTER 0
#define MIPI_PWDN_N_CAPTURE 0
#define MIPI_PWDN_N_DATA_WIDTH 1
#define MIPI_PWDN_N_DO_TEST_BENCH_WIRING 0
#define MIPI_PWDN_N_DRIVEN_SIM_VALUE 0
#define MIPI_PWDN_N_EDGE_TYPE "NONE"
#define MIPI_PWDN_N_FREQ 50000000
#define MIPI_PWDN_N_HAS_IN 0
#define MIPI_PWDN_N_HAS_OUT 1
#define MIPI_PWDN_N_HAS_TRI 0
#define MIPI_PWDN_N_IRQ -1
#define MIPI_PWDN_N_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_PWDN_N_IRQ_TYPE "NONE"
#define MIPI_PWDN_N_NAME "/dev/mipi_pwdn_n"
#define MIPI_PWDN_N_RESET_VALUE 0
#define MIPI_PWDN_N_SPAN 16
#define MIPI_PWDN_N_TYPE "altera_avalon_pio"


/*
 * mipi_reset_n configuration
 *
 */

#define ALT_MODULE_CLASS_mipi_reset_n altera_avalon_pio
#define MIPI_RESET_N_BASE 0x33c10
#define MIPI_RESET_N_BIT_CLEARING_EDGE_REGISTER 0
#define MIPI_RESET_N_BIT_MODIFYING_OUTPUT_REGISTER 0
#define MIPI_RESET_N_CAPTURE 0
#define MIPI_RESET_N_DATA_WIDTH 1
#define MIPI_RESET_N_DO_TEST_BENCH_WIRING 0
#define MIPI_RESET_N_DRIVEN_SIM_VALUE 0
#define MIPI_RESET_N_EDGE_TYPE "NONE"
#define MIPI_RESET_N_FREQ 50000000
#define MIPI_RESET_N_HAS_IN 0
#define MIPI_RESET_N_HAS_OUT 1
#define MIPI_RESET_N_HAS_TRI 0
#define MIPI_RESET_N_IRQ -1
#define MIPI_RESET_N_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MIPI_RESET_N_IRQ_TYPE "NONE"
#define MIPI_RESET_N_NAME "/dev/mipi_reset_n"
#define MIPI_RESET_N_RESET_VALUE 0
#define MIPI_RESET_N_SPAN 16
#define MIPI_RESET_N_TYPE "altera_avalon_pio"


/*
 * onchip_memory2_0 configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory2_0 altera_avalon_onchip_memory2
#define ONCHIP_MEMORY2_0_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY2_0_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY2_0_BASE 0x20000
#define ONCHIP_MEMORY2_0_CONTENTS_INFO ""
#define ONCHIP_MEMORY2_0_DUAL_PORT 0
#define ONCHIP_MEMORY2_0_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_INIT_CONTENTS_FILE "Qsys_onchip_memory2_0"
#define ONCHIP_MEMORY2_0_INIT_MEM_CONTENT 0
#define ONCHIP_MEMORY2_0_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY2_0_IRQ -1
#define ONCHIP_MEMORY2_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY2_0_NAME "/dev/onchip_memory2_0"
#define ONCHIP_MEMORY2_0_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY2_0_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY2_0_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY2_0_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY2_0_SIZE_VALUE 60000
#define ONCHIP_MEMORY2_0_SPAN 60000
#define ONCHIP_MEMORY2_0_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY2_0_WRITABLE 1


/*
 * pio_0 configuration
 *
 */

#define ALT_MODULE_CLASS_pio_0 altera_avalon_pio
#define PIO_0_BASE 0x33bf0
#define PIO_0_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_0_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_0_CAPTURE 1
#define PIO_0_DATA_WIDTH 1
#define PIO_0_DO_TEST_BENCH_WIRING 0
#define PIO_0_DRIVEN_SIM_VALUE 0
#define PIO_0_EDGE_TYPE "RISING"
#define PIO_0_FREQ 70000000
#define PIO_0_HAS_IN 1
#define PIO_0_HAS_OUT 0
#define PIO_0_HAS_TRI 0
#define PIO_0_IRQ 6
#define PIO_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PIO_0_IRQ_TYPE "EDGE"
#define PIO_0_NAME "/dev/pio_0"
#define PIO_0_RESET_VALUE 0
#define PIO_0_SPAN 16
#define PIO_0_TYPE "altera_avalon_pio"


/*
 * sw configuration
 *
 */

#define ALT_MODULE_CLASS_sw altera_avalon_pio
#define SW_BASE 0x33c30
#define SW_BIT_CLEARING_EDGE_REGISTER 0
#define SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SW_CAPTURE 0
#define SW_DATA_WIDTH 10
#define SW_DO_TEST_BENCH_WIRING 0
#define SW_DRIVEN_SIM_VALUE 0
#define SW_EDGE_TYPE "NONE"
#define SW_FREQ 50000000
#define SW_HAS_IN 1
#define SW_HAS_OUT 0
#define SW_HAS_TRI 0
#define SW_IRQ -1
#define SW_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SW_IRQ_TYPE "NONE"
#define SW_NAME "/dev/sw"
#define SW_RESET_VALUE 0
#define SW_SPAN 16
#define SW_TYPE "altera_avalon_pio"


/*
 * sysid_qsys configuration
 *
 */

#define ALT_MODULE_CLASS_sysid_qsys altera_avalon_sysid_qsys
#define SYSID_QSYS_BASE 0x33c80
#define SYSID_QSYS_ID 0
#define SYSID_QSYS_IRQ -1
#define SYSID_QSYS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_QSYS_NAME "/dev/sysid_qsys"
#define SYSID_QSYS_SPAN 8
#define SYSID_QSYS_TIMESTAMP 1656092876
#define SYSID_QSYS_TYPE "altera_avalon_sysid_qsys"


/*
 * timer configuration
 *
 */

#define ALT_MODULE_CLASS_timer altera_avalon_timer
#define TIMER_ALWAYS_RUN 0
#define TIMER_BASE 0x33b60
#define TIMER_COUNTER_SIZE 32
#define TIMER_FIXED_PERIOD 0
#define TIMER_FREQ 70000000
#define TIMER_IRQ 3
#define TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_LOAD_VALUE 69999
#define TIMER_MULT 0.001
#define TIMER_NAME "/dev/timer"
#define TIMER_PERIOD 1
#define TIMER_PERIOD_UNITS "ms"
#define TIMER_RESET_OUTPUT 0
#define TIMER_SNAPSHOT 1
#define TIMER_SPAN 32
#define TIMER_TICKS_PER_SEC 1000
#define TIMER_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TYPE "altera_avalon_timer"


/*
 * timer_0 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_0 altera_avalon_timer
#define TIMER_0_ALWAYS_RUN 1
#define TIMER_0_BASE 0x33b20
#define TIMER_0_COUNTER_SIZE 32
#define TIMER_0_FIXED_PERIOD 1
#define TIMER_0_FREQ 70000000
#define TIMER_0_IRQ 5
#define TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_0_LOAD_VALUE 69999999
#define TIMER_0_MULT 1.0
#define TIMER_0_NAME "/dev/timer_0"
#define TIMER_0_PERIOD 1
#define TIMER_0_PERIOD_UNITS "s"
#define TIMER_0_RESET_OUTPUT 0
#define TIMER_0_SNAPSHOT 0
#define TIMER_0_SPAN 32
#define TIMER_0_TICKS_PER_SEC 1
#define TIMER_0_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_0_TYPE "altera_avalon_timer"


/*
 * timer_1 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_1 altera_avalon_timer
#define TIMER_1_ALWAYS_RUN 1
#define TIMER_1_BASE 0x33b00
#define TIMER_1_COUNTER_SIZE 32
#define TIMER_1_FIXED_PERIOD 1
#define TIMER_1_FREQ 70000000
#define TIMER_1_IRQ 7
#define TIMER_1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_1_LOAD_VALUE 69999999
#define TIMER_1_MULT 1.0
#define TIMER_1_NAME "/dev/timer_1"
#define TIMER_1_PERIOD 1
#define TIMER_1_PERIOD_UNITS "s"
#define TIMER_1_RESET_OUTPUT 0
#define TIMER_1_SNAPSHOT 0
#define TIMER_1_SPAN 32
#define TIMER_1_TICKS_PER_SEC 1
#define TIMER_1_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_1_TYPE "altera_avalon_timer"


/*
 * uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_uart_0 altera_avalon_uart
#define UART_0_BASE 0x33b40
#define UART_0_BAUD 115200
#define UART_0_DATA_BITS 8
#define UART_0_FIXED_BAUD 1
#define UART_0_FREQ 50000000
#define UART_0_IRQ 4
#define UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define UART_0_NAME "/dev/uart_0"
#define UART_0_PARITY 'N'
#define UART_0_SIM_CHAR_STREAM ""
#define UART_0_SIM_TRUE_BAUD 0
#define UART_0_SPAN 32
#define UART_0_STOP_BITS 1
#define UART_0_SYNC_REG_DEPTH 2
#define UART_0_TYPE "altera_avalon_uart"
#define UART_0_USE_CTS_RTS 0
#define UART_0_USE_EOP_REGISTER 0

#endif /* __SYSTEM_H_ */
