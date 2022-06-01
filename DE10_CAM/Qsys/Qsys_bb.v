
module Qsys (
	altpll_0_areset_conduit_export,
	altpll_0_locked_conduit_export,
	clk_clk,
	clk_sdram_clk,
	clk_vga_clk,
	d8m_xclkin_clk,
	i2c_0_external_connection_sda_in,
	i2c_0_external_connection_scl_in,
	i2c_0_external_connection_sda_oe,
	i2c_0_external_connection_scl_oe,
	key_external_connection_export,
	led_external_connection_export,
	mipi_reset_n_external_connection_export,
	reset_reset_n,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	sw_external_connection_export,
	uart_0_rx_tx_rxd,
	uart_0_rx_tx_txd);	

	input		altpll_0_areset_conduit_export;
	output		altpll_0_locked_conduit_export;
	input		clk_clk;
	output		clk_sdram_clk;
	output		clk_vga_clk;
	output		d8m_xclkin_clk;
	input		i2c_0_external_connection_sda_in;
	input		i2c_0_external_connection_scl_in;
	output		i2c_0_external_connection_sda_oe;
	output		i2c_0_external_connection_scl_oe;
	input	[1:0]	key_external_connection_export;
	output	[9:0]	led_external_connection_export;
	output		mipi_reset_n_external_connection_export;
	input		reset_reset_n;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[15:0]	sdram_wire_dq;
	output	[1:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	input	[9:0]	sw_external_connection_export;
	input		uart_0_rx_tx_rxd;
	output		uart_0_rx_tx_txd;
endmodule
