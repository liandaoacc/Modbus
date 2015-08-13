#include "port.h"
#include "modbus.h"

#define UART_BAUD_RATE 9600
#define UART_BAUD_CALC(UART_BAUD_RATE,F_OSC) ((F_OSC)/((UART_BAUD_RATE)*8l)-1)

extern volatile unsigned int* SCICTL1;
extern volatile unsigned int* SCICTL2; 
extern volatile unsigned int* SCICCR; 
extern volatile unsigned int* SCIHBAUD; 
extern volatile unsigned int* SCILBAUD; 
extern volatile unsigned int* SCIPRI; 
extern volatile unsigned int* SCITXBUF;
extern volatile unsigned int* SCIRXBUF;
extern volatile unsigned int* MCRA; 
extern volatile unsigned int* IMR;
extern volatile unsigned int* PIVR;
extern volatile unsigned int* IFR;

extern void disableINTM();
extern void enableINTM();

void sMBPortSerialEnable(char sRxEnable, char sTxEnable)
{
	*SCICTL1 = 0x00;
	*SCICTL2 = 0x00;
    if( sRxEnable )
	{
        *SCICTL1 = (*SCICTL1) | 0x11; // 00010001
		*SCICTL2 = (*SCICTL2) | 0x2;  // 00000010
	}
    else
    {
        *SCICTL1 = (*SCICTL1) & 0x0FE; // 11111110
		*SCICTL2 = (*SCICTL2) & 0x0FD; // 11111101
	}

	if( sTxEnable )
	{
        *SCICTL1 = (*SCICTL1) | 0x12; // 00010010
		*SCICTL2 = (*SCICTL2) | 0x1;  // 00000001
	}
    else
    {
        *SCICTL1 = (*SCICTL1) & 0x0FD; // 11111101
		*SCICTL2 = (*SCICTL2) & 0x0FE; // 11111110
	}
	*SCICTL1 = (*SCICTL1) | 0x20; // 00100000
	enableINTM();
}

char sMBPortSerialInit(unsigned long ulBaudRate, unsigned char ucDataBits, sMBParityMode eParity)
{ 
    switch ( eParity )
    {
        case MB_PAR_EVEN:
			if(ucDataBits == 8)
			{
				*SCICCR = 0x67; // 01100111
				
			}
			else if(ucDataBits == 7)
			{
				*SCICCR = 0x66; // 01100110
			}
			else
			{
				return 0;
			}
			break;
			
        case MB_PAR_ODD:
			if(ucDataBits == 8)
			{
				*SCICCR = 0x27; // 00100111
			}
			else if(ucDataBits == 7)
			{
				*SCICCR = 0x26; // 00100110
			}
			else
			{
				return 0;
			}
			break;
			
        case MB_PAR_NONE:
			if(ucDataBits == 8)
			{
				// No Parity is implemented, an additional stop bit is required.
				*SCICCR = 0x87; // 10000111
			}
			else if(ucDataBits == 7)
			{
				// No Parity is implemented, an additional stop bit is required.
				*SCICCR = 0x86; // 10000110
			}
			else
			{
				return 0;
			}
			break;

		default:
            break;
    }
	*SCIHBAUD = (UART_BAUD_CALC(ulBaudRate, F_CPU) & 0xFF00) >> 8;
	*SCILBAUD = UART_BAUD_CALC(ulBaudRate, F_CPU) & 0x00FF;
	*SCIPRI = 0x60;
	*MCRA = (*MCRA) | 0x0003;
	*IMR = (*IMR) | 0x0010;
    sMBPortSerialEnable( 0, 0 );
    return 1;
}

void sMBPortSerialPutByte(char ucByte)
{
	*SCITXBUF = ucByte;
}

void sMBPortSerialGetByte(unsigned char* pucByte)
{
	*pucByte = (unsigned char)(*SCIRXBUF);
}

void delay(int count)
{   
	int ii;
	int jj;
	for (ii = 0;ii <= 50; ii++) 
	{    
		jj = count;
		while(jj > 0) 
		jj--; 
	}
}

void interrupt sUART()
{
	switch (*PIVR)
	{
		case 6:
			arMBReceiveMessageFrame();
			break;
		case 7:
			arMBSendMessageFrame();
			break;
		default:
			break;
	}
	*IFR = 0x0010;
	enableINTM(); 
}
