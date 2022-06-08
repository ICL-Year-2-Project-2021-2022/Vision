#!/bin/bash

iverilog -g 2012 -s COM_COUNTER_tb -o COM_COUNTER_tb COM_COUNTER_tb.sv ../COM_COUNTER.sv
./COM_COUNTER_tb