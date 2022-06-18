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
     parameter DEFAULT_CHANNEL = 16,
               DEFAULT_WR_CHANNEL = -1,
               DEFAULT_RD_CHANNEL = -1,
               DEFAULT_DESTID = 19 
   )
  (output [85 - 81 : 0] default_destination_id,
   output [27-1 : 0] default_wr_channel,
   output [27-1 : 0] default_rd_channel,
   output [27-1 : 0] default_src_channel
  );

  assign default_destination_id = 
    DEFAULT_DESTID[85 - 81 : 0];

  generate
    if (DEFAULT_CHANNEL == -1) begin : no_default_channel_assignment
      assign default_src_channel = '0;
    end
    else begin : default_channel_assignment
      assign default_src_channel = 27'b1 << DEFAULT_CHANNEL;
    end
  endgenerate

  generate
    if (DEFAULT_RD_CHANNEL == -1) begin : no_default_rw_channel_assignment
      assign default_wr_channel = '0;
      assign default_rd_channel = '0;
    end
    else begin : default_rw_channel_assignment
      assign default_wr_channel = 27'b1 << DEFAULT_WR_CHANNEL;
      assign default_rd_channel = 27'b1 << DEFAULT_RD_CHANNEL;
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
    input  [99-1 : 0]    sink_data,
    input                       sink_startofpacket,
    input                       sink_endofpacket,
    output                      sink_ready,

    // -------------------
    // Command Source (Output)
    // -------------------
    output                          src_valid,
    output reg [99-1    : 0] src_data,
    output reg [27-1 : 0] src_channel,
    output                          src_startofpacket,
    output                          src_endofpacket,
    input                           src_ready
);

    // -------------------------------------------------------
    // Local parameters and variables
    // -------------------------------------------------------
    localparam PKT_ADDR_H = 54;
    localparam PKT_ADDR_L = 36;
    localparam PKT_DEST_ID_H = 85;
    localparam PKT_DEST_ID_L = 81;
    localparam PKT_PROTECTION_H = 89;
    localparam PKT_PROTECTION_L = 87;
    localparam ST_DATA_W = 99;
    localparam ST_CHANNEL_W = 27;
    localparam DECODER_TYPE = 0;

    localparam PKT_TRANS_WRITE = 57;
    localparam PKT_TRANS_READ  = 58;

    localparam PKT_ADDR_W = PKT_ADDR_H-PKT_ADDR_L + 1;
    localparam PKT_DEST_ID_W = PKT_DEST_ID_H-PKT_DEST_ID_L + 1;



    // -------------------------------------------------------
    // Figure out the number of bits to mask off for each slave span
    // during address decoding
    // -------------------------------------------------------
    localparam PAD0 = log2ceil(64'h40000 - 64'h20000); 
    localparam PAD1 = log2ceil(64'h50000 - 64'h40000); 
    localparam PAD2 = log2ceil(64'h51000 - 64'h50800); 
    localparam PAD3 = log2ceil(64'h51800 - 64'h51000); 
    localparam PAD4 = log2ceil(64'h51880 - 64'h51800); 
    localparam PAD5 = log2ceil(64'h518c0 - 64'h51880); 
    localparam PAD6 = log2ceil(64'h51900 - 64'h518c0); 
    localparam PAD7 = log2ceil(64'h51940 - 64'h51900); 
    localparam PAD8 = log2ceil(64'h51980 - 64'h51940); 
    localparam PAD9 = log2ceil(64'h519c0 - 64'h51980); 
    localparam PAD10 = log2ceil(64'h519e0 - 64'h519c0); 
    localparam PAD11 = log2ceil(64'h51a00 - 64'h519e0); 
    localparam PAD12 = log2ceil(64'h51a20 - 64'h51a00); 
    localparam PAD13 = log2ceil(64'h51a40 - 64'h51a20); 
    localparam PAD14 = log2ceil(64'h51a60 - 64'h51a40); 
    localparam PAD15 = log2ceil(64'h51a80 - 64'h51a60); 
    localparam PAD16 = log2ceil(64'h51aa0 - 64'h51a90); 
    localparam PAD17 = log2ceil(64'h51ab0 - 64'h51aa0); 
    localparam PAD18 = log2ceil(64'h51ac0 - 64'h51ab0); 
    localparam PAD19 = log2ceil(64'h51ad0 - 64'h51ac0); 
    localparam PAD20 = log2ceil(64'h51ae0 - 64'h51ad0); 
    localparam PAD21 = log2ceil(64'h51af0 - 64'h51ae0); 
    localparam PAD22 = log2ceil(64'h51b00 - 64'h51af0); 
    localparam PAD23 = log2ceil(64'h51b10 - 64'h51b00); 
    localparam PAD24 = log2ceil(64'h51b20 - 64'h51b10); 
    localparam PAD25 = log2ceil(64'h51b28 - 64'h51b20); 
    localparam PAD26 = log2ceil(64'h51b30 - 64'h51b28); 
    // -------------------------------------------------------
    // Work out which address bits are significant based on the
    // address range of the slaves. If the required width is too
    // large or too small, we use the address field width instead.
    // -------------------------------------------------------
    localparam ADDR_RANGE = 64'h51b30;
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
    wire [27-1 : 0] default_src_channel;




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

    // ( 0x20000 .. 0x40000 )
    if ( {address[RG:PAD0],{PAD0{1'b0}}} == 19'h20000   ) begin
            src_channel = 27'b000000000010000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 19;
    end

    // ( 0x40000 .. 0x50000 )
    if ( {address[RG:PAD1],{PAD1{1'b0}}} == 19'h40000   ) begin
            src_channel = 27'b000000000000000000010000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 3;
    end

    // ( 0x50800 .. 0x51000 )
    if ( {address[RG:PAD2],{PAD2{1'b0}}} == 19'h50800   ) begin
            src_channel = 27'b000000000000001000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 18;
    end

    // ( 0x51000 .. 0x51800 )
    if ( {address[RG:PAD3],{PAD3{1'b0}}} == 19'h51000   ) begin
            src_channel = 27'b000000000000000010000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 10;
    end

    // ( 0x51800 .. 0x51880 )
    if ( {address[RG:PAD4],{PAD4{1'b0}}} == 19'h51800   ) begin
            src_channel = 27'b000000000000000000001000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 2;
    end

    // ( 0x51880 .. 0x518c0 )
    if ( {address[RG:PAD5],{PAD5{1'b0}}} == 19'h51880   ) begin
            src_channel = 27'b000000000000000000000100000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 4;
    end

    // ( 0x518c0 .. 0x51900 )
    if ( {address[RG:PAD6],{PAD6{1'b0}}} == 19'h518c0   ) begin
            src_channel = 27'b000000000000000000000010000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 5;
    end

    // ( 0x51900 .. 0x51940 )
    if ( {address[RG:PAD7],{PAD7{1'b0}}} == 19'h51900   ) begin
            src_channel = 27'b000000000000000000000001000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 6;
    end

    // ( 0x51940 .. 0x51980 )
    if ( {address[RG:PAD8],{PAD8{1'b0}}} == 19'h51940   ) begin
            src_channel = 27'b000000000000000000000000100;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 1;
    end

    // ( 0x51980 .. 0x519c0 )
    if ( {address[RG:PAD9],{PAD9{1'b0}}} == 19'h51980   ) begin
            src_channel = 27'b000000000000000000000000010;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 0;
    end

    // ( 0x519c0 .. 0x519e0 )
    if ( {address[RG:PAD10],{PAD10{1'b0}}} == 19'h519c0   ) begin
            src_channel = 27'b001000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 24;
    end

    // ( 0x519e0 .. 0x51a00 )
    if ( {address[RG:PAD11],{PAD11{1'b0}}} == 19'h519e0   ) begin
            src_channel = 27'b000100000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 26;
    end

    // ( 0x51a00 .. 0x51a20 )
    if ( {address[RG:PAD12],{PAD12{1'b0}}} == 19'h51a00   ) begin
            src_channel = 27'b000000000100000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 25;
    end

    // ( 0x51a20 .. 0x51a40 )
    if ( {address[RG:PAD13],{PAD13{1'b0}}} == 19'h51a20   ) begin
            src_channel = 27'b000000000000010000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 7;
    end

    // ( 0x51a40 .. 0x51a60 )
    if ( {address[RG:PAD14],{PAD14{1'b0}}} == 19'h51a40   ) begin
            src_channel = 27'b000000000000000001000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 11;
    end

    // ( 0x51a60 .. 0x51a80 )
    if ( {address[RG:PAD15],{PAD15{1'b0}}} == 19'h51a60   ) begin
            src_channel = 27'b000000000000000000100000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 12;
    end

    // ( 0x51a90 .. 0x51aa0 )
    if ( {address[RG:PAD16],{PAD16{1'b0}}} == 19'h51a90   ) begin
            src_channel = 27'b100000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 21;
    end

    // ( 0x51aa0 .. 0x51ab0 )
    if ( {address[RG:PAD17],{PAD17{1'b0}}} == 19'h51aa0   ) begin
            src_channel = 27'b010000000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 20;
    end

    // ( 0x51ab0 .. 0x51ac0 )
    if ( {address[RG:PAD18],{PAD18{1'b0}}} == 19'h51ab0   ) begin
            src_channel = 27'b000010000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 16;
    end

    // ( 0x51ac0 .. 0x51ad0 )
    if ( {address[RG:PAD19],{PAD19{1'b0}}} == 19'h51ac0   ) begin
            src_channel = 27'b000001000000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 17;
    end

    // ( 0x51ad0 .. 0x51ae0 )
    if ( {address[RG:PAD20],{PAD20{1'b0}}} == 19'h51ad0  && read_transaction  ) begin
            src_channel = 27'b000000100000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 14;
    end

    // ( 0x51ae0 .. 0x51af0 )
    if ( {address[RG:PAD21],{PAD21{1'b0}}} == 19'h51ae0  && read_transaction  ) begin
            src_channel = 27'b000000010000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 22;
    end

    // ( 0x51af0 .. 0x51b00 )
    if ( {address[RG:PAD22],{PAD22{1'b0}}} == 19'h51af0   ) begin
            src_channel = 27'b000000001000000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 15;
    end

    // ( 0x51b00 .. 0x51b10 )
    if ( {address[RG:PAD23],{PAD23{1'b0}}} == 19'h51b00   ) begin
            src_channel = 27'b000000000001000000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 9;
    end

    // ( 0x51b10 .. 0x51b20 )
    if ( {address[RG:PAD24],{PAD24{1'b0}}} == 19'h51b10   ) begin
            src_channel = 27'b000000000000100000000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 8;
    end

    // ( 0x51b20 .. 0x51b28 )
    if ( {address[RG:PAD25],{PAD25{1'b0}}} == 19'h51b20  && read_transaction  ) begin
            src_channel = 27'b000000000000000100000000000;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 23;
    end

    // ( 0x51b28 .. 0x51b30 )
    if ( {address[RG:PAD26],{PAD26{1'b0}}} == 19'h51b28   ) begin
            src_channel = 27'b000000000000000000000000001;
            src_data[PKT_DEST_ID_H:PKT_DEST_ID_L] = 13;
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


