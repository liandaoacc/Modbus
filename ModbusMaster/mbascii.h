#ifndef _MBASCII_H
#define _MBASCII_H

#include "port.h"

char aMBASCIIInit(unsigned long ulBaudRate, sMBParityMode eParity);
void aMBASCIISend(unsigned char slaveAddress, const unsigned char* modbusPDU, const unsigned char modbusPDUSize);
void aMBASCIIReceive(unsigned char* slaveAddress, unsigned char** modbusPDU, unsigned char* modbusPDUSize);
void aMBASCIISendMessageFrame();
void aMBASCIIReceiveMessageFrame();
unsigned char aMBASCIIChar2Bin(unsigned char aCharacter);
unsigned char aMBASCIIBin2Char(unsigned char aByte);
void aMBASCIITimerT1sExpired();


#endif