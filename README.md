## Introduction
- This is a course project which transplanting FreeMODBUS 
library to the TMS320 LF2407 DSP demoboard.
- FreeMODBUS is a free implementation of the popular Modbus 
protocol specially targeted for embedded systems. Website 
is http://www.freemodbus.org/.

## Folder description
- ModbusMaster: The demo shows how to use the transplanted 
FreeMODBUS library to setup the Modbus master.
- ModbusSlave: The demo shows how to use the transplanted 
FreeMODBUS library to setup the Modbus slave.
- redundancychecking.exe: A simple program I wrote for 
redundancy check.

## Require
- Need two TMS320 LF2407 DSP demoboard, one runs as master,
the other one runs as slave.
- To compile the program, Code Composer Studio 3.0 was needed. 
Since it is just a course project, I only test it on this tool.