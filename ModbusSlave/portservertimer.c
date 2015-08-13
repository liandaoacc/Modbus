#include "port.h"
#include "modbus.h"

extern volatile unsigned int* EVAIMRB;
extern volatile unsigned int* T2PER;
extern volatile unsigned int* T2CNT;
extern volatile unsigned int* T2CON;
extern volatile unsigned int* EVAIFRB;
extern volatile unsigned int* PIVR;
extern volatile unsigned int* IFR;

static volatile unsigned int tTimer3CINT;
static volatile int tTimer3Count = 0;

void tMBServerTimerInit()
{
	tTimer3CINT = 0xFFFF;	// 0.559232s
	*EVAIMRB = *EVAIMRB | 0x0001;	// enable the interrupt
	*T2PER = tTimer3CINT;
	*T2CNT = 0x00;
	*T2CON = 0x170C; // 0001 0111 0000 1100
}

void tMBServerTimerEnable()
{
	// enable timer
	*T2CNT = 0x00;
	*EVAIFRB = *EVAIFRB | 0x0001;	// Clear the interrupt flag 
	*T2CON = *T2CON | 0x0040;
}

void tMBServerTimerDisable()
{
	// Disable timer
	*T2CON = *T2CON & 0x0FFBF;
	*EVAIFRB = *EVAIFRB | 0x0001;	// Clear the interrupt flag 
	*T2CNT = 0x00;
}

void interrupt tTIMERINT2()
{
	int flag;

	switch (*PIVR)
	{
		case 0x002B:	// Timer2
			flag = *EVAIFRB & 0x0001;
			if(flag != 0x0001)
			{
				enableINTM();
				return;
			}

			tTimer3Count ++;
			if(tTimer3Count == 4)	// Wait (4 * 0.559232)s
			{
				tMBServerTimerDisable();
				setMasterState(MB_MASTER_DEAL_ERROR);
				tTimer3Count = 0;
			}
			*EVAIFRB = *EVAIFRB | 0x0001;	// Clear the interrupt flag 
			break;
			
		default:
			break;
	}
	*IFR = 0x0002;	
	enableINTM();
}