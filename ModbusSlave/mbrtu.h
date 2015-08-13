#ifndef _MBRTU_H
#define _MBRTU_H

#include "port.h"

char rMBRTUInit(unsigned long ulBaudRate, sMBParityMode eParity);
void rMBRTUSend(unsigned char slaveAddress, const unsigned char* modbusPDU, const unsigned char modbusPDUSize);
void rMBRTUReceive(unsigned char* slaveAddress, unsigned char** modbusPDU, unsigned char* modbusPDUSize);
void rMBRTUSendMessageFrame();
void rMBRTUReceiveMessageFrame();
void rMBRTUTimerT15Expired();
void rMBRTUTimerT35Expired();
#endif