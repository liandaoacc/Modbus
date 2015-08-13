#include "lf2407.h"
#include "lf2407regs.h"
#include "modbus.h"

void initialSystem()
{
	asm(" setc SXM ");
	asm(" clrc OVM ");
	asm(" clrc CNF ");
	*SCSR1 = 0x87FE;	// F_CPU = 15MHz
	*WDCR = 0x0E8;
	*IMR = 0x0016;		// 0000 0000 0001 0110
	*IFR = 0xFFFF;
	*WSGR = 0x00;
}

void main()
{	
	unsigned char machineAddress = 0x02;
	char isModbusInit = 0;
	
	disableINTM();
	initialSystem();
	 isModbusInit = modbusInit(machineAddress, MODE_RTU, 9600, MB_PAR_EVEN);
	// isModbusInit = modbusInit(machineAddress, MODE_RTU, 9600, MB_PAR_ODD);
	// isModbusInit = modbusInit(machineAddress, MODE_RTU, 9600, MB_PAR_NONE);
	// isModbusInit = modbusInit(machineAddress, MODE_ASCII, 9600, MB_PAR_EVEN);
	// isModbusInit = modbusInit(machineAddress, MODE_ASCII, 9600, MB_PAR_ODD);
	// isModbusInit = modbusInit(machineAddress, MODE_ASCII, 9600, MB_PAR_NONE);
	enableINTM();

	while(1)
	{		
		modbusSlaveRun();
	}
}

void interrupt nothing()
{
	enableINTM();
	return;
}