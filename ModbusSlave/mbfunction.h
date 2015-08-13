#ifndef _MBFUNCTION_H
#define _MBFUNCTION_H

typedef enum
{
	MB_EX_NONE = 0x00,
    MB_EX_ILLEGAL_FUNCTION = 0x01,
    MB_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    MB_EX_ILLEGAL_DATA_VALUE = 0x03,
    MB_EX_SLAVE_DEVICE_FAILURE = 0x04,
    MB_EX_ACKNOWLEDGE = 0x05,
    MB_EX_SLAVE_BUSY = 0x06,
    MB_EX_MEMORY_PARITY_ERROR = 0x08,
    MB_EX_GATEWAY_PATH_FAILED = 0x0A,
    MB_EX_GATEWAY_TGT_FAILED = 0x0B
} fMBException;

typedef fMBException (*fMBFunctionHandler) (unsigned char* pModbusPDU, unsigned char* modbusPDUSize);

typedef struct
{
	unsigned char functionCode;
	fMBFunctionHandler functionHandler;
} fMBFunctionPair;

#define MB_REPORT_SLAVE_ID 66
#define MB_READ_LED_STATE 67
#define MB_WRITE_LED_STATE 68
#define MB_READ_DSP_REGISTER 69
#define MB_WRITE_DSP_REGISTER 70
#define MB_ADD 71
#define MB_SUBSTRACT 72
#define MB_MULTIPLY 73
#define MB_DIVIDE 74
#define MB_MOD 75

fMBException fMBFuncReportSlaveID(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncReadLEDState(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncWriteLEDState(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncReadDSPRegister(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncWriteDSPRegister(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncAdd(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncSubstract(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncMultiply(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncDivide(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
fMBException fMBFuncMod(unsigned char* pModbusPDU, unsigned char* modbusPDUSize);
#endif