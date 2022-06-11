# TCL File Generated by Component Editor 18.1
# Sat Jun 11 16:06:41 BST 2022
# DO NOT MODIFY


# 
# COM_COUNTER "COM_COUNTER" v1.1
#  2022.06.11.16:06:41
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module COM_COUNTER
# 
set_module_property DESCRIPTION ""
set_module_property NAME COM_COUNTER
set_module_property VERSION 1.1
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP Rover
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME COM_COUNTER
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL COM_COUNTER_TOP
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file COM_COUNTER.sv SYSTEM_VERILOG PATH COM_COUNTER.sv
add_fileset_file COM_COUNTER_TOP.sv SYSTEM_VERILOG PATH COM_COUNTER_TOP.sv TOP_LEVEL_FILE
add_fileset_file STREAM_REG_COM.v VERILOG PATH STREAM_REG_COM.v


# 
# parameters
# 
add_parameter IMAGE_W STD_LOGIC_VECTOR 640
set_parameter_property IMAGE_W DEFAULT_VALUE 640
set_parameter_property IMAGE_W DISPLAY_NAME IMAGE_W
set_parameter_property IMAGE_W TYPE STD_LOGIC_VECTOR
set_parameter_property IMAGE_W ENABLED false
set_parameter_property IMAGE_W UNITS None
set_parameter_property IMAGE_W ALLOWED_RANGES 0:8191
set_parameter_property IMAGE_W HDL_PARAMETER true
add_parameter IMAGE_H STD_LOGIC_VECTOR 480
set_parameter_property IMAGE_H DEFAULT_VALUE 480
set_parameter_property IMAGE_H DISPLAY_NAME IMAGE_H
set_parameter_property IMAGE_H TYPE STD_LOGIC_VECTOR
set_parameter_property IMAGE_H ENABLED false
set_parameter_property IMAGE_H UNITS None
set_parameter_property IMAGE_H ALLOWED_RANGES 0:8191
set_parameter_property IMAGE_H HDL_PARAMETER true


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset reset_n reset_n Input 1


# 
# connection point avalon_streaming_sink
# 
add_interface avalon_streaming_sink avalon_streaming end
set_interface_property avalon_streaming_sink associatedClock clock
set_interface_property avalon_streaming_sink associatedReset reset
set_interface_property avalon_streaming_sink dataBitsPerSymbol 8
set_interface_property avalon_streaming_sink errorDescriptor ""
set_interface_property avalon_streaming_sink firstSymbolInHighOrderBits true
set_interface_property avalon_streaming_sink maxChannel 0
set_interface_property avalon_streaming_sink readyLatency 1
set_interface_property avalon_streaming_sink ENABLED true
set_interface_property avalon_streaming_sink EXPORT_OF ""
set_interface_property avalon_streaming_sink PORT_NAME_MAP ""
set_interface_property avalon_streaming_sink CMSIS_SVD_VARIABLES ""
set_interface_property avalon_streaming_sink SVD_ADDRESS_GROUP ""

add_interface_port avalon_streaming_sink sink_data data Input 24
add_interface_port avalon_streaming_sink sink_eop endofpacket Input 1
add_interface_port avalon_streaming_sink sink_ready ready Output 1
add_interface_port avalon_streaming_sink sink_sop startofpacket Input 1
add_interface_port avalon_streaming_sink sink_valid valid Input 1


# 
# connection point avalon_streaming_source
# 
add_interface avalon_streaming_source avalon_streaming start
set_interface_property avalon_streaming_source associatedClock clock
set_interface_property avalon_streaming_source associatedReset reset
set_interface_property avalon_streaming_source dataBitsPerSymbol 8
set_interface_property avalon_streaming_source errorDescriptor ""
set_interface_property avalon_streaming_source firstSymbolInHighOrderBits true
set_interface_property avalon_streaming_source maxChannel 0
set_interface_property avalon_streaming_source readyLatency 1
set_interface_property avalon_streaming_source ENABLED true
set_interface_property avalon_streaming_source EXPORT_OF ""
set_interface_property avalon_streaming_source PORT_NAME_MAP ""
set_interface_property avalon_streaming_source CMSIS_SVD_VARIABLES ""
set_interface_property avalon_streaming_source SVD_ADDRESS_GROUP ""

add_interface_port avalon_streaming_source source_data data Output 24
add_interface_port avalon_streaming_source source_eop endofpacket Output 1
add_interface_port avalon_streaming_source source_ready ready Input 1
add_interface_port avalon_streaming_source source_sop startofpacket Output 1
add_interface_port avalon_streaming_source source_valid valid Output 1


# 
# connection point avalon_mm_slave
# 
add_interface avalon_mm_slave avalon end
set_interface_property avalon_mm_slave addressUnits WORDS
set_interface_property avalon_mm_slave associatedClock clock
set_interface_property avalon_mm_slave associatedReset reset
set_interface_property avalon_mm_slave bitsPerSymbol 8
set_interface_property avalon_mm_slave burstOnBurstBoundariesOnly false
set_interface_property avalon_mm_slave burstcountUnits WORDS
set_interface_property avalon_mm_slave explicitAddressSpan 0
set_interface_property avalon_mm_slave holdTime 0
set_interface_property avalon_mm_slave linewrapBursts false
set_interface_property avalon_mm_slave maximumPendingReadTransactions 0
set_interface_property avalon_mm_slave maximumPendingWriteTransactions 0
set_interface_property avalon_mm_slave readLatency 0
set_interface_property avalon_mm_slave readWaitTime 1
set_interface_property avalon_mm_slave setupTime 0
set_interface_property avalon_mm_slave timingUnits Cycles
set_interface_property avalon_mm_slave writeWaitTime 0
set_interface_property avalon_mm_slave ENABLED true
set_interface_property avalon_mm_slave EXPORT_OF ""
set_interface_property avalon_mm_slave PORT_NAME_MAP ""
set_interface_property avalon_mm_slave CMSIS_SVD_VARIABLES ""
set_interface_property avalon_mm_slave SVD_ADDRESS_GROUP ""

add_interface_port avalon_mm_slave s_chipselect chipselect Input 1
add_interface_port avalon_mm_slave s_read read Input 1
add_interface_port avalon_mm_slave s_write write Input 1
add_interface_port avalon_mm_slave s_readdata readdata Output 32
add_interface_port avalon_mm_slave s_writedata writedata Input 32
add_interface_port avalon_mm_slave s_address address Input 4
set_interface_assignment avalon_mm_slave embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_mm_slave embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_mm_slave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_mm_slave embeddedsw.configuration.isPrintableDevice 0

