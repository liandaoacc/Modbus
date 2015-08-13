#ifndef _MBCHECK_H
#define _MBCHECK_H

#define MB_ADU_MAX_SIZE 256		// Maximum size of ADU

extern volatile unsigned short arADUBufferCount;
extern volatile unsigned char arADUBuffer[MB_ADU_MAX_SIZE]; // RTU and ASCII share the same ADU buffer
extern volatile unsigned char* parSendBuffer;				// Point to the ADU buffer that will be sent
extern volatile unsigned short parReceiveBuffer; 			// Position in the ADU buffer to put the last received data

unsigned short rMBCRC16(unsigned char* pucFrame, unsigned short usLen);
unsigned char aMBLRC8(unsigned char* pucFrame, unsigned short usLen);

#endif
