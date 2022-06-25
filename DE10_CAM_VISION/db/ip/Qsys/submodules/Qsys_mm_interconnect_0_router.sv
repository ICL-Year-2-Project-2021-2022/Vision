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



// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// $Id: //acds/rel/18.1std/ip/merlin/altera_merlin_router/altera_merlin_router.sv.terp#1 $
// $Revision: #1 $
// $Date: 2018/07/18 $
// $Author: psgswbuild $

// -------------------------------------------------------
// Merlin Router
//
// Asserts the appropriate one-hot encoded channel based on 
// either (a) the address or (b) the dest id. The DECODER_TYPE
// parameter controls this behaviour. 0 means address decoder,
// 1 means dest id decoder.
//
// In the case of (a), it also sets the destination id.
// -------------------------------------------------------

`timescale 1 ns / 1 ns

module Qsys_mm_interconnect_0_router_default_decode
  #(
     parameter DEFAULT_CHANNEL = 6,
               DEFAULT_WR_CHANNEL = -1,
               DEFAULT_RD_CHANNEL = -1,
               DEFAULT_DESTID = 8 
   )
  (output [86 - 81 : 0] default_destination_id,
   output [37-1 : 0] default_wr_channel,
   output [37-1 : 0] default_rd_channel,
   output [37-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[86 - 81 : 0];

  generate
    if (DEFAULT_CHANNEL == -1) begin : no_default_channel_assignment
      assign default_src_channel = '0;
    end
    else begin : default_channel_assignment
      assign default_src_channel = 37'b1 << DEFAULT_CHANNEL;
    end
  endgenerate

  generate
    if (DEFAULT_RD_CHANNEL == -1) begin : no_default_rw_channel_assignment
      assign default_wr_channel = '0;
      assign default_rd_channel = '0;
    end
    else begin : default_rw_channel_assignment
      assign default_wr_channel = 37'b1 << DEFAULT_WR_CHANNEL;
      assign default_rd_channel = 37'b1 << DEFAULT_RD_CHANNEL;
    end
  endgenerate

endmodule


module Qsys_mm_interconnect_0_router
(
    // -------------------
    // Clock & Reset
    // -------------------
    input clk,
    input reset,

    // -------------------
    // Command Sink (Input)
    // -------------------
    input                       sink_valid,
    input  [100-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [100-1    : 0] src_data,
    output reg [37-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 53;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 86;
    localparam PKT_DEST_ID_L = 81;
    localparam PKT_PROTECTION_H = 90;
    localparam PKT_PROTECTION_L = 88;
    localparam ST_DATA_W = 100;
    localparam ST_CHANNEL_W = 37;
    localparam DECODER_TYPE = 0;

    localparam PKT_TRANS_WRITE = 56;
    localparam PKT_TRANS_READ  = 57;

    localparam PKT_ADDR_W = PKT_ADDR_H-PKT_ADDR_L + 1;
    localparam PKT_DEST_ID_W = PKT_DEST_ID_H-PKT_DEST_ID_L + 1;



    // -------------------------------------------------------
    // Figure out the number of bits to mask off for each slave span
    // during address decoding
    // -------------------------------------------------------
    localparam PAD0 = log2ceil(64'h10000 - 64'h0); 
    localparam PAD1 = log2ceil(64'h30000 - 64'h20000); 
    localparam PAD2 = log2ceil(64'h31000 - 64'h30800); 
    localparam PAD3 = log2ceil(64'h31800 - 64'h31000); 
    localparam PAD4 = log2ceil(64'h32000 - 64'h31800); 
    localparam PAD5 = log2ceil(64'h32800 - 64'h32000); 
    localparam PAD6 = log2ceil(64'h33000 - 64'h32800); 
    localparam PAD7 = log2ceil(64'h33800 - 64'h33000); 
    localparam PAD8 = log2ceil(64'h33880 - 64'h33800); 
    localparam PAD9 = log2ceil(64'h338c0 - 64'h33880); 
    localparam PAD10 = log2ceil(64'h33900 - 64'h338c0); 
    localparam PAD11 = log2ceil(64'h33940 - 64'h33900); 
    localparam PAD12 = log2ceil(64'h33980 - 64'h33940); 
    localparam PAD13 = log2ceil(64'h339c0 - 64'h33980); 
    localparam PAD14 = log2ceil(64'h33a00 - 64'h339c0); 
    localparam PAD15 = log2ceil(64'h33a40 - 64'h33a00); 
    localparam PAD16 = log2ceil(64'h33a80 - 64'h33a40); 
    localparam PAD17 = log2ceil(64'h33ac0 - 64'h33a80); 
    localparam PAD18 = log2ceil(64'h33b00 - 64'h33ac0); 
    localparam PAD19 = log2ceil(64'h33b20 - 64'h33b00); 
    localparam PAD20 = log2ceil(64'h33b40 - 64'h33b20); 
    localparam PAD21 = log2ceil(64'h33b60 - 64'h33b40); 
    localparam PAD22 = log2ceil(64'h33b80 - 64'h33b60); 
    localparam PAD23 = log2ceil(64'h33ba0 - 64'h33b80); 
    localparam PAD24 = log2ceil(64'h33bc0 - 64'h33ba0); 
    localparam PAD25 = log2ceil(64'h33be0 - 64'h33bc0); 
    localparam PAD26 = log2ceil(64'h33c00 - 64'h33bf0); 
    localparam PAD27 = log2ceil(64'h33c10 - 64'h33c00); 
    localparam PAD28 = log2ceil(64'h33c20 - 64'h33c10); 
    localparam PAD29 = log2ceil(64'h33c30 - 64'h33c20); 
    localparam PAD30 = log2ceil(64'h33c40 - 64'h33c30); 
    localparam PAD31 = log2ceil(64'h33c50 - 64'h33c40); 
    localparam PAD32 = log2ceil(64'h33c60 - 64'h33c50); 
    localparam PAD33 = log2ceil(64'h33c70 - 64'h33c60); 
    localparam PAD34 = log2ceil(64'h33c80 - 64'h33c70); 
    localparam PAD35 = log2ceil(64'h33c88 - 64'h33c80); 
    localparam PAD36 = log2ceil(64'h33c90 - 64'h33c88); 
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'h33c90;
    localparam RANGE_ADDR_WIDTH = log2ceil(ADDR_RANGE);
    localparam OPTIMIZED_ADDR_H = (RANGE_ADDR_WIDTH > PKT_ADDR_W) ||
                                  (RANGE_ADDR_WIDTH == 0) ?
                                        PKT_ADDR_H :
                                        PKT_ADDR_L + RANGE_ADDR_WIDTH - 1;

    localparam RG = RANGE_ADDR_WIDTH-1;
    localparam REAL_ADDRESS_RANGE = OPTIMIZED_ADDR_H - PKT_ADDR_L;

      reg [PKT_ADDR_W-1 : 0] address;
      always @* begin
        address = {PKT_ADDR_W{1'b0}};
        address [REAL_ADDRESS_RANGE:0] = sink_data[OPTIMIZED_ADDR_H : PKT_ADDR_L];
      end   

    // -------------------------------------------------------
    // Pass almost everything through, untouched
    // -------------------------------------------------------
    assign sink_ready        = src_ready;
    assign src_valid         = sink_valid;
    assign src_startofpacket = sink_startofpacket;
    assign src_endofpacket   = sink_endofpacket;
    wire [PKT_DEST_ID_W-1:0] default_destid;
    wire [37-1 : 0] default_src_channel;




    // -------------------------------------------------------
    // Write and read transaction signals
    // -------------------------------------------------------
    wire read_transaction;
    assign read_transaction  = sink_data[PKT_TRANS_READ];


    Qsys_mm_interconnect_0_router_default_decode the_default_decode(
      .default_destination_id (default_destid),
      .default_wr_channel   (),
      .default_rd_channel   (),
      .default_src_channel  (default_src_channel)
    );

    always @* begin
        src_data    = sink_data;
        src_channel = default_src_channel;
        src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = default_destid;

        // --------------------------------------------------
        // Address Decoder
        // Sets the channel and destination ID based on the address
        // --------------------------------------------------

    // ( 0x0 .. 0x10000 )
    if ( {address[RG:PAD0],{PAD0{1'b0}}} == 18'h0   ) begin
            src_channel = 37'b0000000000000000000000000000001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 8;
    end

    // ( 0x20000 .. 0x30000 )
    if ( {address[RG:PAD1],{PAD1{1'b0}}} == 18'h20000   ) begin
            src_channel = 37'b0000000000100000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 29;
    end

    // ( 0x30800 .. 0x31000 )
    if ( {address[RG:PAD2],{PAD2{1'b0}}} == 18'h30800   ) begin
            src_channel = 37'b0000000000000000010000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 20;
    end

    // ( 0x31000 .. 0x31800 )
    if ( {address[RG:PAD3],{PAD3{1'b0}}} == 18'h31000   ) begin
            src_channel = 37'b0000000000000001000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 28;
    end

    // ( 0x31800 .. 0x32000 )
    if ( {address[RG:PAD4],{PAD4{1'b0}}} == 18'h31800   ) begin
            src_channel = 37'b0000000000000000001000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 18;
    end

    // ( 0x32000 .. 0x32800 )
    if ( {address[RG:PAD5],{PAD5{1'b0}}} == 18'h32000   ) begin
            src_channel = 37'b0000000000000000000100000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 19;
    end

    // ( 0x32800 .. 0x33000 )
    if ( {address[RG:PAD6],{PAD6{1'b0}}} == 18'h32800   ) begin
            src_channel = 37'b0000000000000000000010000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 17;
    end

    // ( 0x33000 .. 0x33800 )
    if ( {address[RG:PAD7],{PAD7{1'b0}}} == 18'h33000   ) begin
            src_channel = 37'b0000000000000000000000000001000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 9;
    end

    // ( 0x33800 .. 0x33880 )
    if ( {address[RG:PAD8],{PAD8{1'b0}}} == 18'h33800   ) begin
            src_channel = 37'b0000000000000000000000000000100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
    end

    // ( 0x33880 .. 0x338c0 )
    if ( {address[RG:PAD9],{PAD9{1'b0}}} == 18'h33880   ) begin
            src_channel = 37'b0000000000000000000000010000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 5;
    end

    // ( 0x338c0 .. 0x33900 )
    if ( {address[RG:PAD10],{PAD10{1'b0}}} == 18'h338c0   ) begin
            src_channel = 37'b0000000000000000000000001000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
    end

    // ( 0x33900 .. 0x33940 )
    if ( {address[RG:PAD11],{PAD11{1'b0}}} == 18'h33900   ) begin
            src_channel = 37'b0000000000000000000000000100000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
    end

    // ( 0x33940 .. 0x33980 )
    if ( {address[RG:PAD12],{PAD12{1'b0}}} == 18'h33940   ) begin
            src_channel = 37'b0000000000000000000000000010000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
    end

    // ( 0x33980 .. 0x339c0 )
    if ( {address[RG:PAD13],{PAD13{1'b0}}} == 18'h33980   ) begin
            src_channel = 37'b0000000000000000000000000000010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
    end

    // ( 0x339c0 .. 0x33a00 )
    if ( {address[RG:PAD14],{PAD14{1'b0}}} == 18'h339c0   ) begin
            src_channel = 37'b0000000000000000000000000000000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 10;
    end

    // ( 0x33a00 .. 0x33a40 )
    if ( {address[RG:PAD15],{PAD15{1'b0}}} == 18'h33a00   ) begin
            src_channel = 37'b0000000000000000000000000000000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 11;
    end

    // ( 0x33a40 .. 0x33a80 )
    if ( {address[RG:PAD16],{PAD16{1'b0}}} == 18'h33a40   ) begin
            src_channel = 37'b0000000000000000000000000000000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 12;
    end

    // ( 0x33a80 .. 0x33ac0 )
    if ( {address[RG:PAD17],{PAD17{1'b0}}} == 18'h33a80   ) begin
            src_channel = 37'b0000000000000000000000000000000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
    end

    // ( 0x33ac0 .. 0x33b00 )
    if ( {address[RG:PAD18],{PAD18{1'b0}}} == 18'h33ac0   ) begin
            src_channel = 37'b0000000000000000000000000000000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
    end

    // ( 0x33b00 .. 0x33b20 )
    if ( {address[RG:PAD19],{PAD19{1'b0}}} == 18'h33b00   ) begin
            src_channel = 37'b1000000000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 34;
    end

    // ( 0x33b20 .. 0x33b40 )
    if ( {address[RG:PAD20],{PAD20{1'b0}}} == 18'h33b20   ) begin
            src_channel = 37'b0010000000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 33;
    end

    // ( 0x33b40 .. 0x33b60 )
    if ( {address[RG:PAD21],{PAD21{1'b0}}} == 18'h33b40   ) begin
            src_channel = 37'b0001000000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 36;
    end

    // ( 0x33b60 .. 0x33b80 )
    if ( {address[RG:PAD22],{PAD22{1'b0}}} == 18'h33b60   ) begin
            src_channel = 37'b0000000001000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 35;
    end

    // ( 0x33b80 .. 0x33ba0 )
    if ( {address[RG:PAD23],{PAD23{1'b0}}} == 18'h33b80   ) begin
            src_channel = 37'b0000000000000010000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 13;
    end

    // ( 0x33ba0 .. 0x33bc0 )
    if ( {address[RG:PAD24],{PAD24{1'b0}}} == 18'h33ba0   ) begin
            src_channel = 37'b0000000000000000000001000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 21;
    end

    // ( 0x33bc0 .. 0x33be0 )
    if ( {address[RG:PAD25],{PAD25{1'b0}}} == 18'h33bc0   ) begin
            src_channel = 37'b0000000000000000000000100000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 22;
    end

    // ( 0x33bf0 .. 0x33c00 )
    if ( {address[RG:PAD26],{PAD26{1'b0}}} == 18'h33bf0   ) begin
            src_channel = 37'b0100000000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 30;
    end

    // ( 0x33c00 .. 0x33c10 )
    if ( {address[RG:PAD27],{PAD27{1'b0}}} == 18'h33c00   ) begin
            src_channel = 37'b0000100000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 26;
    end

    // ( 0x33c10 .. 0x33c20 )
    if ( {address[RG:PAD28],{PAD28{1'b0}}} == 18'h33c10   ) begin
            src_channel = 37'b0000010000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 27;
    end

    // ( 0x33c20 .. 0x33c30 )
    if ( {address[RG:PAD29],{PAD29{1'b0}}} == 18'h33c20  && read_transaction  ) begin
            src_channel = 37'b0000001000000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 24;
    end

    // ( 0x33c30 .. 0x33c40 )
    if ( {address[RG:PAD30],{PAD30{1'b0}}} == 18'h33c30  && read_transaction  ) begin
            src_channel = 37'b0000000100000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 31;
    end

    // ( 0x33c40 .. 0x33c50 )
    if ( {address[RG:PAD31],{PAD31{1'b0}}} == 18'h33c40   ) begin
            src_channel = 37'b0000000010000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 25;
    end

    // ( 0x33c50 .. 0x33c60 )
    if ( {address[RG:PAD32],{PAD32{1'b0}}} == 18'h33c50   ) begin
            src_channel = 37'b0000000000010000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 16;
    end

    // ( 0x33c60 .. 0x33c70 )
    if ( {address[RG:PAD33],{PAD33{1'b0}}} == 18'h33c60   ) begin
            src_channel = 37'b0000000000001000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 15;
    end

    // ( 0x33c70 .. 0x33c80 )
    if ( {address[RG:PAD34],{PAD34{1'b0}}} == 18'h33c70   ) begin
            src_channel = 37'b0000000000000100000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 14;
    end

    // ( 0x33c80 .. 0x33c88 )
    if ( {address[RG:PAD35],{PAD35{1'b0}}} == 18'h33c80  && read_transaction  ) begin
            src_channel = 37'b0000000000000000100000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 32;
    end

    // ( 0x33c88 .. 0x33c90 )
    if ( {address[RG:PAD36],{PAD36{1'b0}}} == 18'h33c88   ) begin
            src_channel = 37'b0000000000000000000000000000000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 23;
    end

end


    // --------------------------------------------------
    // Ceil(log2()) function
    // --------------------------------------------------
    function integer log2ceil;
        input reg[65:0] val;
        reg [65:0] i;

        begin
            i = 1;
            log2ceil = 0;

            while (i < val) begin
                log2ceil = log2ceil + 1;
                i = i << 1;
            end
        end
    endfunction

endmodule


