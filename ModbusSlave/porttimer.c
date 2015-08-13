#include "port.h"
#include "modbus.h"

extern volatile unsigned int* EVAIMRA;
extern volatile unsigned int* EVAIFRA;
extern volatile unsigned int* T1PER;
extern volatile unsigned int* T1CNT;
extern volatile unsigned int* T1CON;
extern volatile unsigned int* EVBIMRA;
extern volatile unsigned int* EVBIFRA;
extern volatile unsigned int* T3PER;
extern volatile unsigned int* T3CNT;
extern volatile unsigned int* T3CON;
extern volatile unsigned int* IFR;
extern volatile unsigned int* PIVR;

extern void disableINTM();
extern void enableINTM();

static volatile unsigned int tTimer1CINT;
static volatile unsigned int tTimer2CINT;

void tMBPortTimerInit(unsigned int tTimer1Out, unsigned int tTimer2Out)
{
	tTimer1CINT = (F_CLOCKIN * tTimer1Out) / F_50US_TICKS;
	tTimer2CINT = (F_CLOCKIN * tTimer2Out) / F_50US_TICKS;
	
	*EVAIMRA = *EVAIMRA | 0x0080;	// enable the interrupt
	*T1PER = tTimer1CINT;
	*T1CNT = 0x00;
	*T1CON = 0x170C; // 0001 0111 0000 1100
	
	*EVBIMRA = *EVBIMRA | 0x0080;	// enable the interrupt
	*T3PER = tTimer2CINT;
	*T3CNT = 0x00;
	*T3CON = 0x170C; // 0001 0111 0000 1100
}

static void tEnableTimer1()
{
	// enable timer
	*T1CNT = 0x00;
	*EVAIFRA = *EVAIFRA | 0x0080;	// Clear the interrupt flag 
	*T1CON = *T1CON | 0x0040;
}

static void tEnableTimer2()
{
	// enable timer
	*T3CNT = 0x00;
	*EVBIFRA = *EVBIFRA | 0x0080;	// Clear the interrupt flag 
	*T3CON = *T3CON | 0x0040;
}

static void tDisableTimer1()
{
	// Disable timer
	*T1CON = *T1CON & 0x0FFBF;
	*EVAIFRA = *EVAIFRA | 0x0080;	// Clear the interrupt flag 
	*T1CNT = 0x00;
}

static void tDisableTimer2()
{
	// Disable timer
	*T3CON = *T3CON & 0x0FFBF;
	*EVBIFRA = *EVBIFRA | 0x0080;	// Clear the interrupt flag
	*T3CNT = 0x00;

}

void tMBPortTimerEnable(char tTimer1Emable, char tTimer2Enable)
{
	if(tTimer1Emable)
	{
		tEnableTimer1();
	}
	if(tTimer2Enable)
	{
		tEnableTimer2();
	}
	enableINTM();
}

void tMBPortTimeDisable(char tTimer1Disable, char tTimer2Disable)
{
	if(tTimer1Disable)
	{
		tDisableTimer1();
	}
	if(tTimer2Disable)
	{
		tDisableTimer2();
	}
}

void interrupt tTIMERINT1()
{
	int flag;

	switch (*PIVR)
	{
		case 0x0027:	// Timer1
			flag = *EVAIFRA & 0x0080;
			if(flag != 0x0080)
			{
				enableINTM();
				return;
			}
			arMBTimer1Expire();
			tDisableTimer1();
			break;

		case 0x002F:	// Timer3
			flag = *EVBIFRA & 0x0080;
			if(flag != 0x0080)
			{
				enableINTM();
				return;
			}
			arMBTimer2Expire();
			tDisableTimer2();
			break;
		default:
			break;
	}
	*IFR = 0x0002;	
	enableINTM();
}