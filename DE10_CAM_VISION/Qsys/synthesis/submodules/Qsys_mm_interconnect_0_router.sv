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
               DEFAULT_DESTID = 4 
   )
  (output [84 - 80 : 0] default_destination_id,
   output [30-1 : 0] default_wr_channel,
   output [30-1 : 0] default_rd_channel,
   output [30-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[84 - 80 : 0];

  generate
    if (DEFAULT_CHANNEL == -1) begin : no_default_channel_assignment
      assign default_src_channel = '0;
    end
    else begin : default_channel_assignment
      assign default_src_channel = 30'b1 << DEFAULT_CHANNEL;
    end
  endgenerate

  generate
    if (DEFAULT_RD_CHANNEL == -1) begin : no_default_rw_channel_assignment
      assign default_wr_channel = '0;
      assign default_rd_channel = '0;
    end
    else begin : default_rw_channel_assignment
      assign default_wr_channel = 30'b1 << DEFAULT_WR_CHANNEL;
      assign default_rd_channel = 30'b1 << DEFAULT_RD_CHANNEL;
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
    input  [98-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [98-1    : 0] src_data,
    output reg [30-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 53;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 84;
    localparam PKT_DEST_ID_L = 80;
    localparam PKT_PROTECTION_H = 88;
    localparam PKT_PROTECTION_L = 86;
    localparam ST_DATA_W = 98;
    localparam ST_CHANNEL_W = 30;
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
    localparam PAD5 = log2ceil(64'h32080 - 64'h32000); 
    localparam PAD6 = log2ceil(64'h320c0 - 64'h32080); 
    localparam PAD7 = log2ceil(64'h32100 - 64'h320c0); 
    localparam PAD8 = log2ceil(64'h32140 - 64'h32100); 
    localparam PAD9 = log2ceil(64'h32180 - 64'h32140); 
    localparam PAD10 = log2ceil(64'h321c0 - 64'h32180); 
    localparam PAD11 = log2ceil(64'h32200 - 64'h321c0); 
    localparam PAD12 = log2ceil(64'h32220 - 64'h32200); 
    localparam PAD13 = log2ceil(64'h32240 - 64'h32220); 
    localparam PAD14 = log2ceil(64'h32260 - 64'h32240); 
    localparam PAD15 = log2ceil(64'h32280 - 64'h32260); 
    localparam PAD16 = log2ceil(64'h322a0 - 64'h32280); 
    localparam PAD17 = log2ceil(64'h322c0 - 64'h322a0); 
    localparam PAD18 = log2ceil(64'h322e0 - 64'h322c0); 
    localparam PAD19 = log2ceil(64'h32300 - 64'h322f0); 
    localparam PAD20 = log2ceil(64'h32310 - 64'h32300); 
    localparam PAD21 = log2ceil(64'h32320 - 64'h32310); 
    localparam PAD22 = log2ceil(64'h32330 - 64'h32320); 
    localparam PAD23 = log2ceil(64'h32340 - 64'h32330); 
    localparam PAD24 = log2ceil(64'h32350 - 64'h32340); 
    localparam PAD25 = log2ceil(64'h32360 - 64'h32350); 
    localparam PAD26 = log2ceil(64'h32370 - 64'h32360); 
    localparam PAD27 = log2ceil(64'h32380 - 64'h32370); 
    localparam PAD28 = log2ceil(64'h32388 - 64'h32380); 
    localparam PAD29 = log2ceil(64'h32390 - 64'h32388); 
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'h32390;
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
    wire [30-1 : 0] default_src_channel;




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
            src_channel = 30'b000000000000000000000001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
    end

    // ( 0x20000 .. 0x30000 )
    if ( {address[RG:PAD1],{PAD1{1'b0}}} == 18'h20000   ) begin
            src_channel = 30'b000000000010000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 22;
    end

    // ( 0x30800 .. 0x31000 )
    if ( {address[RG:PAD2],{PAD2{1'b0}}} == 18'h30800   ) begin
            src_channel = 30'b000000000000000000001000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 5;
    end

    // ( 0x31000 .. 0x31800 )
    if ( {address[RG:PAD3],{PAD3{1'b0}}} == 18'h31000   ) begin
            src_channel = 30'b000000000000000100000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 21;
    end

    // ( 0x31800 .. 0x32000 )
    if ( {address[RG:PAD4],{PAD4{1'b0}}} == 18'h31800   ) begin
            src_channel = 30'b000000000000000001000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 13;
    end

    // ( 0x32000 .. 0x32080 )
    if ( {address[RG:PAD5],{PAD5{1'b0}}} == 18'h32000   ) begin
            src_channel = 30'b000000000000000000000100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
    end

    // ( 0x32080 .. 0x320c0 )
    if ( {address[RG:PAD6],{PAD6{1'b0}}} == 18'h32080   ) begin
            src_channel = 30'b000000000000000000000010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
    end

    // ( 0x320c0 .. 0x32100 )
    if ( {address[RG:PAD7],{PAD7{1'b0}}} == 18'h320c0   ) begin
            src_channel = 30'b000000000000000000000000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
    end

    // ( 0x32100 .. 0x32140 )
    if ( {address[RG:PAD8],{PAD8{1'b0}}} == 18'h32100   ) begin
            src_channel = 30'b000000000000000000000000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
    end

    // ( 0x32140 .. 0x32180 )
    if ( {address[RG:PAD9],{PAD9{1'b0}}} == 18'h32140   ) begin
            src_channel = 30'b000000000000000000000000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 8;
    end

    // ( 0x32180 .. 0x321c0 )
    if ( {address[RG:PAD10],{PAD10{1'b0}}} == 18'h32180   ) begin
            src_channel = 30'b000000000000000000000000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
    end

    // ( 0x321c0 .. 0x32200 )
    if ( {address[RG:PAD11],{PAD11{1'b0}}} == 18'h321c0   ) begin
            src_channel = 30'b000000000000000000000000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
    end

    // ( 0x32200 .. 0x32220 )
    if ( {address[RG:PAD12],{PAD12{1'b0}}} == 18'h32200   ) begin
            src_channel = 30'b100000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 27;
    end

    // ( 0x32220 .. 0x32240 )
    if ( {address[RG:PAD13],{PAD13{1'b0}}} == 18'h32220   ) begin
            src_channel = 30'b001000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 26;
    end

    // ( 0x32240 .. 0x32260 )
    if ( {address[RG:PAD14],{PAD14{1'b0}}} == 18'h32240   ) begin
            src_channel = 30'b000100000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 29;
    end

    // ( 0x32260 .. 0x32280 )
    if ( {address[RG:PAD15],{PAD15{1'b0}}} == 18'h32260   ) begin
            src_channel = 30'b000000000100000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 28;
    end

    // ( 0x32280 .. 0x322a0 )
    if ( {address[RG:PAD16],{PAD16{1'b0}}} == 18'h32280   ) begin
            src_channel = 30'b000000000000001000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 9;
    end

    // ( 0x322a0 .. 0x322c0 )
    if ( {address[RG:PAD17],{PAD17{1'b0}}} == 18'h322a0   ) begin
            src_channel = 30'b000000000000000000100000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 14;
    end

    // ( 0x322c0 .. 0x322e0 )
    if ( {address[RG:PAD18],{PAD18{1'b0}}} == 18'h322c0   ) begin
            src_channel = 30'b000000000000000000010000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 15;
    end

    // ( 0x322f0 .. 0x32300 )
    if ( {address[RG:PAD19],{PAD19{1'b0}}} == 18'h322f0   ) begin
            src_channel = 30'b010000000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 23;
    end

    // ( 0x32300 .. 0x32310 )
    if ( {address[RG:PAD20],{PAD20{1'b0}}} == 18'h32300   ) begin
            src_channel = 30'b000010000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 19;
    end

    // ( 0x32310 .. 0x32320 )
    if ( {address[RG:PAD21],{PAD21{1'b0}}} == 18'h32310   ) begin
            src_channel = 30'b000001000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 20;
    end

    // ( 0x32320 .. 0x32330 )
    if ( {address[RG:PAD22],{PAD22{1'b0}}} == 18'h32320  && read_transaction  ) begin
            src_channel = 30'b000000100000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 17;
    end

    // ( 0x32330 .. 0x32340 )
    if ( {address[RG:PAD23],{PAD23{1'b0}}} == 18'h32330  && read_transaction  ) begin
            src_channel = 30'b000000010000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 24;
    end

    // ( 0x32340 .. 0x32350 )
    if ( {address[RG:PAD24],{PAD24{1'b0}}} == 18'h32340   ) begin
            src_channel = 30'b000000001000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 18;
    end

    // ( 0x32350 .. 0x32360 )
    if ( {address[RG:PAD25],{PAD25{1'b0}}} == 18'h32350   ) begin
            src_channel = 30'b000000000001000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 12;
    end

    // ( 0x32360 .. 0x32370 )
    if ( {address[RG:PAD26],{PAD26{1'b0}}} == 18'h32360   ) begin
            src_channel = 30'b000000000000100000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 11;
    end

    // ( 0x32370 .. 0x32380 )
    if ( {address[RG:PAD27],{PAD27{1'b0}}} == 18'h32370   ) begin
            src_channel = 30'b000000000000010000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 10;
    end

    // ( 0x32380 .. 0x32388 )
    if ( {address[RG:PAD28],{PAD28{1'b0}}} == 18'h32380  && read_transaction  ) begin
            src_channel = 30'b000000000000000010000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 25;
    end

    // ( 0x32388 .. 0x32390 )
    if ( {address[RG:PAD29],{PAD29{1'b0}}} == 18'h32388   ) begin
            src_channel = 30'b000000000000000000000000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 16;
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


