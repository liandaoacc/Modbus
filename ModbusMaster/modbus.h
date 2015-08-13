#ifndef _MODBUS_H
#define _MODBUS_H

typedef enum
{
	MODE_RTU,
	MODE_ASCII
} sMode;

typedef enum
{
    MB_PAR_NONE,  // No parity.
    MB_PAR_ODD,   // Odd parity.
    MB_PAR_EVEN   // Even parity.
} sMBParityMode;

typedef enum
{
	MB_EV_READY,
	MB_EV_MSG_SENT,
	MB_EV_MSG_RCV
} eMBEventType;

typedef enum
{
	MB_MASTER_IDLE,
	MB_MASTER_WAIT_REP,
	MB_MASTER_DEAL_REP,
	MB_MASTER_DEAL_ERROR
} sMBMasterState;

typedef enum
{
	MB_SLAVE_IDLE,
	MB_SLAVE_CHECK_REQ,
	MB_SLAVE_DEAL_REQ,
	MB_SLAVE_GEN_REP,
	MB_SLAVE_WAIT_SEND
} sMBSlaveState;

extern void (*arMBReceiveMessageFrame) (void);
extern void (*arMBSendMessageFrame) (void);
extern void (*arMBTimer1Expire) (void);
extern void (*arMBTimer2Expire) (void);
extern char (*arMBModeInit)(unsigned long ulBaudRate, sMBParityMode eParity);

extern void (*modbusSend)(unsigned char slaveAddress, const unsigned char* modbusPDU, const unsigned char modbusPDUSize);
extern void (*modbusReceive)(unsigned char* slaveAddress, unsigned char** modbusPDU, unsigned char* modbusPDUSize);

char modbusInit(unsigned char machineAddress, unsigned char serialMode, unsigned long ulBaudRate, sMBParityMode eParity);
char isMachineAddress(const unsigned char mAddress);
char isLastSlaveAddress(const unsigned char lsAddress);
unsigned char getMachineAddress();

void setSlaveState(sMBSlaveState sState);
sMBSlaveState getSlaveState();
void setMasterState(sMBMasterState mState);
sMBMasterState getMasterState();

void modbusMasterRun();
void modbusSlaveRun();
#endif