#ifndef _PORT_H
#define _PORT_H

#include "modbus.h"

#define F_CPU 15000000
#define TIMER_PRESCALER	(128UL) //TPS
#define F_CLOCKIN (F_CPU / TIMER_PRESCALER)
#define F_50US_TICKS (20000UL)

// Serial Line System
char sMBPortSerialInit(unsigned long ulBaudRate, unsigned char ucDataBits, sMBParityMode eParity);
void sMBPortSerialEnable(char sRxEnable, char sTxEnable);
void sMBPortSerialPutByte(char ucByte);
void sMBPortSerialGetByte(unsigned char* pucByte);
void delay(int count);

// Timer for Serial Line System
void tMBPortTimerInit(unsigned int tTimer1Out, unsigned int tTimer2Out);
void tMBPortTimerEnable(char tTimer1Emable, char tTimer2Enable);
void tMBPortTimeDisable(char tTimer1Disable, char tTimer2Disable);

// Event System
void eMBPushEvent(eMBEventType eEvent);
char eMBPopEvent(eMBEventType* eEvent);

// Timer for the server
void tMBServerTimerInit();
void tMBServerTimerEnable();
void tMBServerTimerDisable();
#endif
