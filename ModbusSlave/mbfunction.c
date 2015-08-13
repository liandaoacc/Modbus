#include "string.h"
#include "limits.h"
#include "mbfunction.h"
#include "modbus.h"
#include "led.h"

static unsigned char registerList[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// In
// function code
// pModbusPDU = {0x42}
// modbusPDUSize = 1
// Out
// function code | slave ID
// pModbusPDU = {0x42, 0x02}
// modbusPDUSize = 2
// Error
// pModbusPDU = {0x0C2}
// modbusPDUSize = 1
fMBException fMBFuncReportSlaveID(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char aAddress;
	
	if(*modbusPDUSize != 1)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	aAddress = getMachineAddress();
	if(aAddress >= 1 && aAddress <= 247)
	{
		pModbusPDU[1] = aAddress;
		*modbusPDUSize = 2;
	}
	else
	{
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		aException = MB_EX_SLAVE_DEVICE_FAILURE;
	}
	return aException;
}

// In
// function code
// pModbusPDU = {0x43}
// modbusPDUSize = 1
// Out
// function code | led1 | state | led2 | state | led3 | state | led4 | state
// pModbusPDU = {0x43, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x04, 0x01}
// modbusPDUSize = 9
// Error
// pModbusPDU = {0x0C3}
// modbusPDUSize = 1
fMBException fMBFuncReadLEDState(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char ledState;
	unsigned char i;
	unsigned char pattern = 1;
	
	if(*modbusPDUSize != 1)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	ledState = ti_led_state();
	for(i = 0; i < 4; i++)
	{
		pModbusPDU[2 * i + 1] = i + 1;
		if(pattern & ledState)
		{
			pModbusPDU[2 * i + 2] = 1;
		}
		else
		{
			pModbusPDU[2 * i + 2] = 0;
		}
		pattern <<= 1;
	}
	*modbusPDUSize = 9;
	return aException;
}

// In
// function code | led1 | state | led2 | state | led3 | state | led4 | state
// pModbusPDU = {0x44, 0x01, 0x00, 0x02, 0x01, 0x03, 0x00, 0x04, 0x01}
// modbusPDUSize = 9
// Out
// function code
// pModbusPDU = {0x44}
// modbusPDUSize = 1
// Error
// pModbusPDU = {0x0C4}
// modbusPDUSize = 1
fMBException fMBFuncWriteLEDState(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char i;
	unsigned char pattern = 1;
	if(*modbusPDUSize != 9)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	for(i = 0; i < 4; i++)
	{
		if(pModbusPDU[2 * i + 2])
		{
			ti_led_on(i + 1);
		}
		else
		{
			ti_led_off(i + 1);
		}
	}
	*modbusPDUSize = 1;
	return aException;
}

// In
// function code | begin | size
// pModbusPDU = {0x45, 0x00, 0x0A}
// modbusPDUSize = 3
// Out
// function code | begin | size | data
// pModbusPDU = {0x45, 0x00, 0x0A, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09}
// modbusPDUSize = 13
// Error
// pModbusPDU = {0x0C5}
// modbusPDUSize = 1
fMBException fMBFuncReadDSPRegister(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char begin;
	unsigned char size = 0;
	if(*modbusPDUSize != 3)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	begin = pModbusPDU[1];
	size = pModbusPDU[2];
	if((begin < 0) || (begin > 9))
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	if(begin + size > 10)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	memcpy((void*)(pModbusPDU + 3), (void*)(&registerList[begin]), size);
	*modbusPDUSize = size + 3;
	return aException;
}

// In
// function code | begin | size | data
// pModbusPDU = {0x46, 0x00, 0x0A, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09}
// modbusPDUSize = 13
// Out
// function code | begin | size
// pModbusPDU = {0x46, 0x00, 0x0A}
// modbusPDUSize = 3
// Error
// pModbusPDU = {0x0C6}
// modbusPDUSize = 1
fMBException fMBFuncWriteDSPRegister(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char begin;
	unsigned char size = 0;
	// First we need to make sure that the PDU contains begin and size.
	// So *modbusPDUSize >= 3
	if(*modbusPDUSize < 3)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	begin = pModbusPDU[1];
	size = pModbusPDU[2];
	if((begin < 0) || (begin > 9))
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	if(begin + size > 10)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	if(*modbusPDUSize != (size + 3))
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	memcpy((void*)(&registerList[begin]), (void*)(pModbusPDU + 3), size);
	*modbusPDUSize = 3;
	return aException;
}

// In
// function code | data1 | data2
// pModbusPDU = {0x47, 0x00, 0x0A}
// modbusPDUSize = 3
// Out
// function code | result
// pModbusPDU = {0x47, 0x0A}
// modbusPDUSize = 2
// Error
// pModbusPDU = {0x0C7}
// modbusPDUSize = 1
fMBException fMBFuncAdd(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char result;
	if(*modbusPDUSize != 3)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	// Over float
	if((UCHAR_MAX - pModbusPDU[1]) < pModbusPDU[2])
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	result = pModbusPDU[1] + pModbusPDU[2];
	pModbusPDU[1] = result;
	*modbusPDUSize = 2;
	return aException;
	
}

// In
// function code | data1 | data2
// pModbusPDU = {0x48, 0x0A, 0x00}
// modbusPDUSize = 3
// Out
// function code | result
// pModbusPDU = {0x48, 0x0A}
// modbusPDUSize = 2
// Error
// pModbusPDU = {0x0C8}
// modbusPDUSize = 1
fMBException fMBFuncSubstract(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char result;
	if(*modbusPDUSize != 3)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	// Here we need to make sure that pModbusPDU[1] > pModbusPDU[2]
	if(pModbusPDU[1] < pModbusPDU[2])
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	result = pModbusPDU[1] - pModbusPDU[2];
	pModbusPDU[1] = result;
	*modbusPDUSize = 2;
	return aException;
}

// In
// function code | data1 | data2
// pModbusPDU = {0x49, 0x0A, 0x00}
// modbusPDUSize = 3
// Out
// function code | result
// pModbusPDU = {0x49, 0x00}
// modbusPDUSize = 2
// Error
// pModbusPDU = {0x0C9}
// modbusPDUSize = 1
fMBException fMBFuncMultiply(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char result;
	unsigned char temp;
	if(*modbusPDUSize != 3)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	// Over float
	temp = UCHAR_MAX / pModbusPDU[1];
	if(temp < pModbusPDU[2])
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	result = pModbusPDU[1] * pModbusPDU[2];
	pModbusPDU[1] = result;
	*modbusPDUSize = 2;
	return aException;
}

// In
// function code | data1 | data2
// pModbusPDU = {0x4A, 0x00, 0x0A}
// modbusPDUSize = 3
// Out
// function code | result
// pModbusPDU = {0x4A, 0x00}
// modbusPDUSize = 2
// Error
// pModbusPDU = {0x0CA}
// modbusPDUSize = 1
fMBException fMBFuncDivide(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char result;
	if(*modbusPDUSize != 3)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	if(pModbusPDU[2] == 0)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	result = pModbusPDU[1] / pModbusPDU[2];
	pModbusPDU[1] = result;
	*modbusPDUSize = 2;
	return aException;
}

// In
// function code | data1 | data2
// pModbusPDU = {0x4B, 0x0B, 0x0A}
// modbusPDUSize = 3
// Out
// function code | result
// pModbusPDU = {0x4B, 0x01}
// modbusPDUSize = 2
// Error
// pModbusPDU = {0x0CB}
// modbusPDUSize = 1
fMBException fMBFuncMod(unsigned char* pModbusPDU, unsigned char* modbusPDUSize)
{
	fMBException aException = MB_EX_NONE;
	unsigned char result;
	if(*modbusPDUSize != 3)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	if(pModbusPDU[2] == 0)
	{
		aException = MB_EX_ILLEGAL_DATA_VALUE;
		pModbusPDU[0] += 128;
		*modbusPDUSize = 1;
		return aException;
	}
	
	result = pModbusPDU[1] % pModbusPDU[2];
	pModbusPDU[1] = result;
	*modbusPDUSize = 2;
	return aException;
}