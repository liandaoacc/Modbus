#include "string.h"
#include "mbcheck.h"
#include "mbrtu.h"

typedef enum
{
	STATE_RTU_RCVINIT,
	STATE_RTU_RCVIDLE,
	STATE_RTU_RECEIVING,
	STATE_RTU_RCVCONANDWAIT
}rMBRTURcvState;

typedef enum
{
	STATE_RTU_SENDIDLE,	// Not send, in IDLE
	STATE_RTU_SENDING	// Sending the data	
}rMBRTUSndState;

static volatile rMBRTUSndState rSendState = STATE_RTU_SENDIDLE;
static volatile rMBRTURcvState rReceiveState = STATE_RTU_RCVINIT;
static volatile char isMsgWrong = 0;

char rMBRTUInit(unsigned long ulBaudRate, sMBParityMode eParity)
{
	unsigned int timerT15_50us;
	unsigned int timerT35_50us;
	char serialInitsuccess = 0;
	
	if(ulBaudRate > 19200)
	{
		timerT15_50us = 15;		// 750us
		timerT35_50us = 35;		// 1800us
	}
	else
	{
		timerT15_50us = (3UL * 220000UL) / (2UL * ulBaudRate);
		timerT35_50us = (7UL * 220000UL) / (2UL * ulBaudRate);
	}
	tMBPortTimerInit(timerT15_50us, timerT35_50us);
	serialInitsuccess = sMBPortSerialInit(ulBaudRate, 8, eParity); // if 1, success, if 0 fail
	sMBPortSerialEnable(1, 0);
	rReceiveState = STATE_RTU_RCVINIT;
	tMBPortTimerEnable(0, 1);
	return serialInitsuccess;
}

void rMBRTUSend(unsigned char slaveAddress, const unsigned char* modbusPDU, const unsigned char modbusPDUSize)
{
	unsigned short rtuCRC16;

	if((slaveAddress < 1) || (slaveAddress > 247))
		return;

	if((modbusPDUSize < 1) || (modbusPDUSize > 253))
		return;
	
	arADUBufferCount = 0;
	arADUBuffer[0] = slaveAddress;
	arADUBufferCount++;
	
	memcpy((void*)(arADUBuffer + 1), (void*)modbusPDU, modbusPDUSize);
	arADUBufferCount += modbusPDUSize;
	
	rtuCRC16 = rMBCRC16((unsigned char*)arADUBuffer, arADUBufferCount);
	arADUBuffer[arADUBufferCount++] = (unsigned char)(rtuCRC16 & 0x00FF);
	arADUBuffer[arADUBufferCount++] = (unsigned char)((rtuCRC16 & 0x0FF00) >> 8);
	
	parSendBuffer = arADUBuffer;
	sMBPortSerialEnable(0, 1);
	
	// We can send data when in IDLE
	if((rSendState == STATE_RTU_SENDIDLE) && (rReceiveState == STATE_RTU_RCVIDLE))
	{
		rSendState = STATE_RTU_SENDING;
		sMBPortSerialPutByte(*parSendBuffer);
	}
}

void rMBRTUReceive(unsigned char* slaveAddress, unsigned char** modbusPDU, unsigned char* modbusPDUSize)
{
	*slaveAddress = arADUBuffer[0];
	*modbusPDU = (unsigned char*)&arADUBuffer[1];
	*modbusPDUSize = parReceiveBuffer - 3;
}

void rMBRTUSendMessageFrame()
{
	// If receiving data, we can't not send data.
	// In fact it should not happen.
	if(rReceiveState != STATE_RTU_RCVIDLE)
	{
		return;
	}
	
	switch(rSendState)
	{
		case STATE_RTU_SENDIDLE:
			// Make sure that the receiver is opened and the 
			// sender is closed in the STATE_RTU_SENDIDLE.
			sMBPortSerialEnable(1, 0);	
			break;
		
		case STATE_RTU_SENDING:
			if(arADUBufferCount > 1)
			{
				parSendBuffer++;
				arADUBufferCount--;
				sMBPortSerialPutByte(*parSendBuffer);
			}
			else
			{
				// Sent the last character, so start the t3.5
				// and wait for the t3.5 expired
				tMBPortTimerEnable(0, 1);
			}
			break;
		default:
			break;
	}
	
}

void rMBRTUReceiveMessageFrame()
{
	unsigned char rReceiveChar;
	// If sending data, we can't not receive data.
	// In fact it should not happen.
	if(rSendState != STATE_RTU_SENDIDLE)
	{
		return;
	}
	
	switch(rReceiveState)
	{
		case STATE_RTU_RCVINIT:
			// If we have received a character in STATE_RTU_RCVINIT state,
			// we have to wait until the frame is finished.
			tMBPortTimerEnable(0, 1);
			isMsgWrong = 1;
			break;
		
		case STATE_RTU_RCVIDLE:
			// When we receive the first character,
			// we need to clear the flag isMsgWrong.
			isMsgWrong = 0;
			parReceiveBuffer = 0;
			sMBPortSerialGetByte(&rReceiveChar);
			arADUBuffer[parReceiveBuffer++] = rReceiveChar;
			rReceiveState = STATE_RTU_RECEIVING;
			tMBPortTimerEnable(1, 1);
			break;
			
		case STATE_RTU_RECEIVING:
			tMBPortTimerEnable(1, 1);  // Receive character, so restart the timer
			
			// In RTU mode, the maximum size of ADU is 256
			// If we receive more than 256 characters, we do nothing.
			// And wait for all the characters were received.
			// After all the characters were received, we wait for the 
			// t1.5 expired to change the state to STATE_RTU_RCVCONANDWAIT.
			if(parReceiveBuffer < MB_ADU_MAX_SIZE) 
			{
				sMBPortSerialGetByte(&rReceiveChar);
				arADUBuffer[parReceiveBuffer++] = rReceiveChar;
			}
			break;
			
		case STATE_RTU_RCVCONANDWAIT:
			// If we receive the character in STATE_RTU_RCVCONANDWAIT, 
			// we set the flag isMsgWrong because the message is wrong.
			isMsgWrong = 1;
			break;
		default:
			break;
	}
	
}

static char rMBRTUADUCRC()
{
	unsigned short rtuCRC16;
	char isRight = 0;
	// In RTU mode, the maximum size of ADU is 256, 
	// the minimum size of ADU is 4
	if(parReceiveBuffer >= 4 && parReceiveBuffer <= 256)
	{
		// CRC
		rtuCRC16 = rMBCRC16((unsigned char*)arADUBuffer, parReceiveBuffer - 2);
		if((arADUBuffer[parReceiveBuffer - 2] == ((unsigned char)(rtuCRC16 & 0x00FF))) &&
		   (arADUBuffer[parReceiveBuffer - 1] == ((unsigned char)((rtuCRC16 & 0x0FF00) >> 8))))
		{
			// SLAVE MODE
			if(isMachineAddress(arADUBuffer[0]))
			{
				isRight = 1;
			}
			// MASTER MODE
			// if(isLastSlaveAddress(arADUBuffer[0]))
			// {
				// isRight = 1;
			// }
		}
	}
	return isRight;
}

void rMBRTUTimerT15Expired()
{
	switch(rReceiveState)
	{
		case STATE_RTU_RCVINIT:
			break;
		
		case STATE_RTU_RCVIDLE:
			break;
		
		case STATE_RTU_RECEIVING:
			tMBPortTimeDisable(1, 1);
			rReceiveState = STATE_RTU_RCVCONANDWAIT;
			// If the message is right, we push the event MB_EV_MSG_RCV,
			// other wise we wait for the t3.5 expired and go to the STATE_RTU_RCVIDLE
			if(!rMBRTUADUCRC())
			{
				isMsgWrong = 1;
			}
			// Need to restart the t3.5.
			tMBPortTimerEnable(0, 1);
			break;
			
		case STATE_RTU_RCVCONANDWAIT:
			break;
		
		default:
			break;
	}
	tMBPortTimeDisable(1, 0);
}

void rMBRTUTimerT35Expired()
{
	switch(rReceiveState)
	{
		case STATE_RTU_RCVINIT:
			rReceiveState = STATE_RTU_RCVIDLE;
			eMBPushEvent(MB_EV_READY);
		break;
		case STATE_RTU_RCVIDLE:
		break;
		case STATE_RTU_RECEIVING:
		break;
		case STATE_RTU_RCVCONANDWAIT:
			// Before push the event, we need to check the flag isMsgWrong.
			// If we have received character in STATE_RTU_RCVCONANDWAIT,
			// that means that this message is wrong.
			// We should not push the event.
			if(!isMsgWrong)
			{
				eMBPushEvent(MB_EV_MSG_RCV);
			}
			rReceiveState = STATE_RTU_RCVIDLE;
		break;
		default:
		break;
	}
	
	switch(rSendState)
	{
		case STATE_RTU_SENDIDLE:
			break;
		case STATE_RTU_SENDING:
			// End sending, go to STATE_RTU_SENDIDLE.
			// Push the MB_EV_MSG_SENT event.
			// Disable the sender and enable the receiver.
			rSendState = STATE_RTU_SENDIDLE;
			eMBPushEvent(MB_EV_MSG_SENT);
			sMBPortSerialEnable(1, 0);
			break;
		default:
			break;
	}
	
	tMBPortTimeDisable(0, 1);
}