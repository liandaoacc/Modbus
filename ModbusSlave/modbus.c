#include "port.h"
#include "mbrtu.h"
#include "mbascii.h"
#include "mbfunction.h"
#include "modbus.h"

#define FUNCTIONLISTSIZE 10

static unsigned char masterAddress; // This machine's address

static unsigned char slaveAddress;
static unsigned char lastSlaveAddress;
static unsigned char modbusPDU[253];
static unsigned char* pModbusPDUBuffer;
static unsigned char modbusPDUBufferSize = 0;

static eMBEventType eEvent = 0;
static sMBSlaveState slaveState = MB_SLAVE_IDLE;
static sMBMasterState masterState = MB_MASTER_IDLE;
static char isErrorInFunctionCode = 0;
static char isErrorInMaster = 0;

static fMBException fException = MB_EX_NONE;
static fMBFunctionHandler fFunctionHandler;
static fMBFunctionPair functionList[FUNCTIONLISTSIZE] = 
{
	{MB_REPORT_SLAVE_ID, fMBFuncReportSlaveID},
	{MB_READ_LED_STATE, fMBFuncReadLEDState},
	{MB_WRITE_LED_STATE, fMBFuncWriteLEDState},
	{MB_READ_DSP_REGISTER, fMBFuncReadDSPRegister},
	{MB_WRITE_DSP_REGISTER, fMBFuncWriteDSPRegister},
	{MB_ADD, fMBFuncAdd},
	{MB_SUBSTRACT, fMBFuncSubstract},
	{MB_MULTIPLY, fMBFuncMultiply},
	{MB_DIVIDE, fMBFuncDivide},
	{MB_MOD, fMBFuncMod}
};

void (*arMBReceiveMessageFrame) (void);
void (*arMBSendMessageFrame) (void);
void (*arMBTimer1Expire) (void);
void (*arMBTimer2Expire) (void);
char (*arMBModeInit)(unsigned long ulBaudRate, sMBParityMode eParity);

void (*modbusSend)(unsigned char slaveAddress, const unsigned char* modbusPDU, const unsigned char modbusPDUSize);
void (*modbusReceive)(unsigned char* slaveAddress, unsigned char** modbusPDU, unsigned char* modbusPDUSize);

char modbusInit(unsigned char machineAddress, unsigned char serialMode, unsigned long ulBaudRate, sMBParityMode eParity)
{
	if((machineAddress < 1) || (machineAddress > 247))
	{
		return 0;
	}
	masterAddress = machineAddress;
	
	switch(serialMode)
	{
		case MODE_RTU:
			arMBReceiveMessageFrame = rMBRTUReceiveMessageFrame;
			arMBSendMessageFrame = rMBRTUSendMessageFrame;
			arMBModeInit = rMBRTUInit;
			arMBTimer1Expire = rMBRTUTimerT15Expired;
			arMBTimer2Expire = rMBRTUTimerT35Expired;
			modbusSend = rMBRTUSend;
			modbusReceive = rMBRTUReceive;
			break;
		
		case MODE_ASCII:
			arMBReceiveMessageFrame = aMBASCIIReceiveMessageFrame;
			arMBSendMessageFrame = aMBASCIISendMessageFrame;
			arMBModeInit = aMBASCIIInit;
			arMBTimer1Expire = aMBASCIITimerT1sExpired;
			arMBTimer2Expire = 0;
			modbusSend = aMBASCIISend;
			modbusReceive = aMBASCIIReceive;
			break;
	}
	
	if(arMBModeInit(ulBaudRate, eParity) == 0)
	{
		// Fail
		return 0;
	}
	return 1;
}

char isMachineAddress(const unsigned char mAddress)
{
	return (mAddress == masterAddress);
}

char isLastSlaveAddress(const unsigned char lsAddress)
{
	return (lastSlaveAddress == lsAddress);
}

unsigned char getMachineAddress()
{
	return masterAddress;
}

void setSlaveState(sMBSlaveState sState)
{
	slaveState = sState;
}

sMBSlaveState getSlaveState()
{
	return slaveState;
}

void setMasterState(sMBMasterState mState)
{
	masterState = mState;
}

sMBMasterState getMasterState()
{
	return masterState;
}

static char checkRespond()
{
	if(lastSlaveAddress == slaveAddress)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}
static char dealRespond()
{
	if(pModbusPDUBuffer[0] > 127)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static void dealRespondError()
{
	return ;
}

void modbusMasterRun()
{
	unsigned char i;
	for(i = 0; i < 253; i++)
	{
		modbusPDU[i] = i;
	}
	switch(masterState)
	{
		case MB_MASTER_IDLE:
			if(eMBPopEvent(&eEvent))
			{
				switch(eEvent)
				{
					case MB_EV_READY:
						// Here we send the message.
						modbusPDUBufferSize = 253;
						slaveAddress = 2;
						lastSlaveAddress = slaveAddress;
						modbusSend(slaveAddress, modbusPDU, modbusPDUBufferSize);
						break;
					case MB_EV_MSG_SENT:
						// If the message sent, the system will wait for respond.
						// So start the timer.
						masterState = MB_MASTER_WAIT_REP;
						tMBServerTimerInit();
						tMBServerTimerEnable();
						break;
					case MB_EV_MSG_RCV:
						// As the master would not receive message in the MB_MASTER_IDLE,
						// we do nothing here.
						break;
					default:
						break;
				}
			}
			break;
			
		case MB_MASTER_WAIT_REP:
			if(eMBPopEvent(&eEvent))
			{
				if(eEvent == MB_EV_MSG_RCV)
				{
					// If we receive message, we need to check if it is 
					// from the slave we sent message before.
					// If right, the system go to MB_MASTER_DEAL_REP,
					// other wise it remain in MB_MASTER_WAIT_REP.
					modbusReceive(&slaveAddress, &pModbusPDUBuffer, &modbusPDUBufferSize);
					tMBServerTimerDisable();
					if(checkRespond())
					{
						masterState = MB_MASTER_DEAL_REP;
					}
					else
					{
						masterState = MB_MASTER_WAIT_REP;
					}
				}
			}
			break;
			
		case MB_MASTER_DEAL_REP:
			// Deal with the respond
			if(dealRespond())
			{
				// Right
				masterState = MB_MASTER_IDLE;
				eMBPushEvent(MB_EV_READY);
			}
			else
			{
				// There is something error, so go to the MA_MASTER_DEAL_ERROR state.
				masterState = MB_MASTER_DEAL_ERROR;
				isErrorInMaster = 1;	
			}
			break;
			
		case MB_MASTER_DEAL_ERROR:
			if(isErrorInMaster)
			{
				// Error from dealRespond
				dealRespondError();
			}
			else
			{
				// Error because of timer expired
				dealRespondError();
			}
			masterState = MB_MASTER_IDLE;
			eMBPushEvent(MB_EV_READY);
			break;
		default:
			break;
	}		
}

static char checkRequire()
{
	char isRight = 0;
	int i;
	for(i = 0; i < FUNCTIONLISTSIZE; i++)
	{
		if(functionList[i].functionCode == pModbusPDUBuffer[0])
		{
			fFunctionHandler = functionList[i].functionHandler;
			isRight = 1;
		}
	}
	return isRight;
}

static void dealRequire()
{
	fException = fFunctionHandler(pModbusPDUBuffer, &modbusPDUBufferSize);
}

static void genErrorReq()
{
	pModbusPDUBuffer[0] += 128;
	modbusPDUBufferSize = 1;
}

void modbusSlaveRun()
{
	switch(slaveState)
	{
		case MB_SLAVE_IDLE:
			if(eMBPopEvent(&eEvent))
			{
				if(eEvent == MB_EV_MSG_RCV)
				{
					modbusReceive(&slaveAddress, &pModbusPDUBuffer, &modbusPDUBufferSize);
					slaveState = MB_SLAVE_CHECK_REQ;
				}
			}
			break;
			
		case MB_SLAVE_CHECK_REQ:
			if(checkRequire())
			{
				slaveState = MB_SLAVE_DEAL_REQ;
			}
			else
			{
				slaveState = MB_SLAVE_GEN_REP;
				isErrorInFunctionCode = 1;
			}
			break;
			
		case MB_SLAVE_DEAL_REQ:
			dealRequire();
			if(slaveAddress == 0)
			{
				slaveState = MB_SLAVE_IDLE;
			}
			else
			{
				slaveState = MB_SLAVE_GEN_REP;
			}
			break;
			
		case MB_SLAVE_GEN_REP:
			if(isErrorInFunctionCode)
			{
				genErrorReq();
			}
			modbusSend(slaveAddress, pModbusPDUBuffer, modbusPDUBufferSize);
			slaveState = MB_SLAVE_WAIT_SEND;
			break;
		case MB_SLAVE_WAIT_SEND:
			if(eMBPopEvent(&eEvent))
			{
				if(eEvent == MB_EV_MSG_SENT)
				{
					slaveState = MB_SLAVE_IDLE;
				}
			}
			break;
		default:
			break;
	}	
}