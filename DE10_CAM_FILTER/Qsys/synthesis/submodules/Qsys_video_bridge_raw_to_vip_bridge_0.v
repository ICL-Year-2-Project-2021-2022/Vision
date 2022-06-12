// (C) 2001-2018 Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files from any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License Subscription 
// Agreement, Intel FPGA IP License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Intel and sold by 
// Intel or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// THIS FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THIS FILE OR THE USE OR OTHER DEALINGS
// IN THIS FILE.

/******************************************************************************
 *                                                                            *
 * This module convert a RAW video packet stream into a VIP video packet      *
 *  stream.                                                                   *
 *                                                                            *
 ******************************************************************************/

//`define USE_1_SYMBOL_PER_BEAT
//`define USE_2_SYMBOLS_PER_BEAT
//`define USE_3_SYMBOLS_PER_BEAT

module Qsys_video_bridge_raw_to_vip_bridge_0 (
	// Inputs
	clk,
	reset,

	raw_data,
	raw_startofpacket,
	raw_endofpacket,
	raw_empty,
	raw_valid,

	vip_ready,

	// Bidirectional

	// Outputs
	raw_ready,

	vip_data,
	vip_startofpacket,
	vip_endofpacket,
	vip_empty,
	vip_valid
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/

parameter DW				= 23;
parameter EW				= 1;

parameter CTRL_PACKET_0	= 8'hF;
parameter CTRL_PACKET_1	= 8'd0;
parameter CTRL_PACKET_2	= 8'd2;
parameter CTRL_PACKET_3	= 8'd8;
parameter CTRL_PACKET_4	= 8'd0;
parameter CTRL_PACKET_5	= 8'd0;
parameter CTRL_PACKET_6	= 8'd1;
parameter CTRL_PACKET_7	= 8'd14;
parameter CTRL_PACKET_8	= 8'd0;
parameter CTRL_PACKET_9	= 8'b0000;

/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/

// Inputs
input						clk;
input						reset;

input			[DW: 0]	raw_data;
input						raw_startofpacket;
input						raw_endofpacket;
input			[EW: 0]	raw_empty;
input						raw_valid;

input						vip_ready;

// Bidirectional

// Outputs
output					raw_ready;

output reg	[DW: 0]	vip_data;
output reg				vip_startofpacket;
output reg				vip_endofpacket;
output reg	[EW: 0]	vip_empty;
output reg				vip_valid;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/

localparam	STATE_0_CREATE_CTRL_PACKET		= 2'h0,
				STATE_1_START_CONVERT_TO_VIP	= 2'h1,
				STATE_2_CONVERT_TO_VIP			= 2'h2;

/*****************************************************************************
 *                 Internal Wires and Registers Declarations                 *
 *****************************************************************************/

// Internal Wires
wire			[DW: 0]	ctrl_packet_data;

// Internal Registers
reg			[ 3: 0]	ctrl_packet_counter;

// State Machine Registers
reg			[ 1: 0]	s_raw_to_vip;
reg			[ 1: 0]	ns_raw_to_vip;

// Integers

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/

always @(posedge clk)
begin
	if (reset)
		s_raw_to_vip <= STATE_0_CREATE_CTRL_PACKET;
	else
		s_raw_to_vip <= ns_raw_to_vip;
end

always @(*)
begin
   case (s_raw_to_vip)
	STATE_0_CREATE_CTRL_PACKET:
		begin
			if ((~vip_valid | vip_ready) & (ctrl_packet_counter == 4'h3))
				ns_raw_to_vip = STATE_1_START_CONVERT_TO_VIP;
			else
				ns_raw_to_vip = STATE_0_CREATE_CTRL_PACKET;
		end
	STATE_1_START_CONVERT_TO_VIP:
		begin
			if (~vip_valid | vip_ready)
				ns_raw_to_vip = STATE_2_CONVERT_TO_VIP;
			else
				ns_raw_to_vip = STATE_1_START_CONVERT_TO_VIP;
		end
	STATE_2_CONVERT_TO_VIP:
		begin
			if ((~vip_valid | vip_ready) & raw_endofpacket & raw_valid)
				ns_raw_to_vip = STATE_0_CREATE_CTRL_PACKET;
			else
				ns_raw_to_vip = STATE_2_CONVERT_TO_VIP;
		end
	default:
		begin
			ns_raw_to_vip = STATE_0_CREATE_CTRL_PACKET;
		end
	endcase
end

/*****************************************************************************
 *                             Sequential Logic                              *
 *****************************************************************************/

// Output Registers
always @(posedge clk)
begin
	if (reset)
	begin
		vip_data					<= 'h0;
		vip_startofpacket		<= 1'b0;
		vip_endofpacket		<= 1'b0;
		vip_empty				<= 'h0;
		vip_valid				<= 1'b0;
	end
	else if ((s_raw_to_vip == STATE_0_CREATE_CTRL_PACKET) &
			(~vip_valid | vip_ready))
	begin
		vip_data					<= ctrl_packet_data;
		vip_startofpacket		<= (ctrl_packet_counter == 4'h0);
		vip_endofpacket		<= (ctrl_packet_counter == 4'h3);
		vip_empty				<= 'h0;
		vip_valid				<= 1'b1;
	end
	else if ((s_raw_to_vip == STATE_1_START_CONVERT_TO_VIP) &
			(~vip_valid | vip_ready))
	begin
		vip_data					<= 'h0;
		vip_startofpacket		<= 1'b1;
		vip_endofpacket		<= 1'b0;
		vip_empty				<= 'h0;
		vip_valid				<= 1'b1;
	end
	else if ((s_raw_to_vip == STATE_2_CONVERT_TO_VIP) &
			(~vip_valid | vip_ready))
	begin
		vip_data					<= raw_data;
		vip_startofpacket		<= 1'b0;
		vip_endofpacket		<= raw_endofpacket;
		vip_empty				<= raw_empty;
		vip_valid				<= raw_valid;
	end
	else if (vip_ready)
	begin
		vip_valid				<= 1'b0;
	end
end

// Internal Registers
always @(posedge clk)
begin
	if (reset)
		ctrl_packet_counter 	<= 4'h0;
	else if ((s_raw_to_vip == STATE_0_CREATE_CTRL_PACKET) &
			(~vip_valid | vip_ready))
		ctrl_packet_counter 	<= ctrl_packet_counter + 4'h1;
	else if (s_raw_to_vip != STATE_0_CREATE_CTRL_PACKET)
		ctrl_packet_counter 	<= 4'h0;
end

/*****************************************************************************
 *                            Combinational Logic                            *
 *****************************************************************************/

// Output Assignments
assign raw_ready		= (s_raw_to_vip == STATE_2_CONVERT_TO_VIP) ?
   							(~vip_valid | vip_ready) :	
								1'b0;

// Internal Assignments
assign ctrl_packet_data	=
		(ctrl_packet_counter == 4'h0) ?	CTRL_PACKET_0 : 
		(ctrl_packet_counter == 4'h1) ?	{CTRL_PACKET_3, CTRL_PACKET_2, CTRL_PACKET_1} : 
		(ctrl_packet_counter == 4'h2) ?	{CTRL_PACKET_6, CTRL_PACKET_5, CTRL_PACKET_4} : 
													{CTRL_PACKET_9, CTRL_PACKET_8, CTRL_PACKET_7}; 

/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/


endmodule

