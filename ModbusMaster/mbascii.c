#include "string.h"
#include "mbcheck.h"
#include "mbascii.h"

#define ASCII_TIMEROUT_SEC 1

typedef enum
{
	STATE_ASCII_RCVIDLE,
	STATE_ASCII_RECEIVING,
	STATE_ASCII_RCVEOF
}aMBASCIIRcvState;

typedef enum
{
	STATE_ASCII_SENDIDLE,	// Not send, in IDLE
	STATE_ASCII_SENDING,	// Sending the data
	STATE_ASCII_SENT		// Data was sent, so send the '\n'
}aMBASCIISndState;

static volatile char aIsByteHigh;
static volatile aMBASCIISndState aSendState = STATE_ASCII_SENDIDLE;
static volatile aMBASCIIRcvState aReceiveState = STATE_ASCII_RCVIDLE;

char aMBASCIIInit(unsigned long ulBaudRate, sMBParityMode eParity)
{
	char serialInitsuccess = 0;
	tMBPortTimerInit(ASCII_TIMEROUT_SEC * 20000UL, 0);
	serialInitsuccess =  sMBPortSerialInit(ulBaudRate, 7, eParity); // if 1, success, if 0 fail
	sMBPortSerialEnable(1, 0);
	eMBPushEvent(MB_EV_READY);
	return serialInitsuccess;
}

void aMBASCIISend(unsigned char slaveAddress, const unsigned char* modbusPDU, const unsigned char modbusPDUSize)
{
	unsigned char asciiLRC8;

	if((slaveAddress < 1) || (slaveAddress > 247))
		return;

	if((modbusPDUSize < 1) || (modbusPDUSize > 253))
		return;
	
	arADUBufferCount = 0;
	arADUBuffer[0] = slaveAddress;
	arADUBufferCount++;
	
	memcpy((void*)(arADUBuffer + 1), (void*)modbusPDU, modbusPDUSize);
	arADUBufferCount += modbusPDUSize;
	
	asciiLRC8 = aMBLRC8((unsigned char*)arADUBuffer, arADUBufferCount);
	arADUBuffer[arADUBufferCount++] = asciiLRC8;
	
	parSendBuffer = arADUBuffer;
	aIsByteHigh = 1;
	sMBPortSerialEnable(0, 1);
	// We can send data when in IDLE
	if((aSendState == STATE_ASCII_SENDIDLE) && (aReceiveState == STATE_ASCII_RCVIDLE))
	{
		aSendState = STATE_ASCII_SENDING;
		sMBPortSerialPutByte(0x3A);		
	}
}

void aMBASCIIReceive(unsigned char* slaveAddress, unsigned char** modbusPDU, unsigned char* modbusPDUSize)
{
	*slaveAddress = arADUBuffer[0];
	*modbusPDU = (unsigned char*)&arADUBuffer[1];
	*modbusPDUSize = parReceiveBuffer - 2;
}

void aMBASCIISendMessageFrame()
{
	unsigned char aSendByte;
	
	// If receiving data, we can't not send data.
	// In fact it should not happen.
	if(aReceiveState != STATE_ASCII_RCVIDLE)
	{
		return;
	}
	
	switch(aSendState)
	{
		case STATE_ASCII_SENDIDLE:
			delay(5);	// Make sure that the data was sent
			// After the data was sent, the system go into the state STATE_ASCII_SENDIDLE, 
			// so we need to disable the sender and enable the receiver 
			eMBPushEvent(MB_EV_MSG_SENT);
			sMBPortSerialEnable(1, 0);	
			break;
			
		case STATE_ASCII_SENDING:
			if(arADUBufferCount > 0)
			{
				if(aIsByteHigh)
				{
					aSendByte = aMBASCIIBin2Char(((*parSendBuffer) & 0x0F0) >> 4);
					aIsByteHigh = 0;
					sMBPortSerialPutByte(aSendByte);
				}
				else
				{
					aSendByte = aMBASCIIBin2Char((*parSendBuffer) & 0x0F);
					aIsByteHigh = 1;
					parSendBuffer++;
					arADUBufferCount--;
					sMBPortSerialPutByte(aSendByte);
				}
			}
			else
			{
				aSendState = STATE_ASCII_SENT;
				sMBPortSerialPutByte(0x0D);
				
			}
			break;
		
		case STATE_ASCII_SENT:
			aSendState = STATE_ASCII_SENDIDLE;
			sMBPortSerialPutByte(0x0A);
			break;
			
		default:
			break;
	}

}

static char rMBASCIIADULRC()
{
	unsigned char asciiLRC8;
	char isRight = 0;
	// In ASCII mode, the maximum size of ADU is 255, 
	// the minimum size of ADU is 3
	if(parReceiveBuffer >= 3 && parReceiveBuffer <= 255)
	{
		// LRC
		asciiLRC8 = aMBLRC8((unsigned char*)arADUBuffer, parReceiveBuffer - 1);
		if(asciiLRC8 == arADUBuffer[parReceiveBuffer - 1])
		{
			// SLAVE MODE
			// if(isMachineAddress(arADUBuffer[0]))
			// {
				// isRight = 1;
			// }
			// MASTER MODE
			if(isLastSlaveAddress(arADUBuffer[0]))
			{
				isRight = 1;
			}
		}
	}
	return isRight;
}

void aMBASCIIReceiveMessageFrame()
{
	unsigned char aReceiveChar;
	unsigned char aReceiveByte;
	
	// If sending data, we can't not receive data.
	// In fact it should not happen.
	if(aSendState != STATE_ASCII_SENDIDLE)
	{
		return;
	}
	sMBPortSerialGetByte(&aReceiveChar);
	switch(aReceiveState)
	{
		case STATE_ASCII_RCVIDLE:
			if(aReceiveChar == 0x3A )
			{
				tMBPortTimerEnable(1, 0);
				parReceiveBuffer = 0;
				aIsByteHigh = 1;
				aReceiveState = STATE_ASCII_RECEIVING;
			}
			break;
		case STATE_ASCII_RECEIVING:
			tMBPortTimerEnable(1, 0);  // Receive character in 1 second, so restart the timer
			if(aReceiveChar == 0x3A)
			{
				parReceiveBuffer = 0;
				aIsByteHigh = 1;
			}
			else if(aReceiveChar == 0x0D)
			{
				aReceiveState = STATE_ASCII_RCVEOF;
			}
			else
			{
				aReceiveByte = aMBASCIIChar2Bin(aReceiveChar);
				if(aIsByteHigh)
				{
					if(parReceiveBuffer < (MB_ADU_MAX_SIZE - 1)) // In ASCII mode, the maximum size of ADU is 255
					{
						arADUBuffer[parReceiveBuffer] = aReceiveByte << 4;
						aIsByteHigh = 0;
					}
					else
					{
						// Error, disable the timer and go to STATE_ASCII_RCVIDLE
						aReceiveState = STATE_ASCII_RCVIDLE;
						tMBPortTimeDisable(1, 0);
					}
				}
				else
				{
					arADUBuffer[parReceiveBuffer++] |= aReceiveByte;
					aIsByteHigh = 1;
				}
			}
			break;
		case STATE_ASCII_RCVEOF:
			if(aReceiveChar == 0x0A)
			{
				tMBPortTimeDisable(1, 0); // Receive last character, so close the timer
				// If the message is right, we push the event MB_EV_MSG_RCV.
				if(rMBASCIIADULRC())
				{
					eMBPushEvent(MB_EV_MSG_RCV);
				}
				// Not matter how, we should go to STATE_ASCII_RCVIDLE.
				aReceiveState = STATE_ASCII_RCVIDLE;
			}
			else if(aReceiveChar == 0x3A)
			{
				parReceiveBuffer = 0;
				aIsByteHigh = 1;
				aReceiveState = STATE_ASCII_RECEIVING;
				tMBPortTimerEnable(1, 0);
			}
			else
			{
				aReceiveState = STATE_ASCII_RCVIDLE; // Receive wrong.
			}
			break;
		default:
			break;
	}
}

unsigned char aMBASCIIChar2Bin(unsigned char aCharacter)
{
	if((aCharacter >= 0x30) && (aCharacter <= 0x39))
	{
		return (aCharacter - 0x30);
	}
	else if((aCharacter >= 0x41) && (aCharacter <= 0x46))
	{
		return (aCharacter - 0x41 + 0x0A);
	}
	else
	{
		return 0xFF;
	}
}

unsigned char aMBASCIIBin2Char(unsigned char aByte)
{
	if(aByte <= 0x09)
	{
		return (0x30 + aByte);
	}
	else if((aByte >= 0x0A) && (aByte <= 0x0F))
	{
		return (aByte - 0x0A + 0x41);
	}
	else
	{
		return 0x30;
	}
}

void aMBASCIITimerT1sExpired()
{
	switch(aReceiveState)
	{
		case STATE_ASCII_RCVIDLE:
			break;
		case STATE_ASCII_RECEIVING:
		case STATE_ASCII_RCVEOF:
			aReceiveState = STATE_ASCII_RCVIDLE;
			break;
		default:
			break;
	}
	tMBPortTimeDisable(1, 0);
}