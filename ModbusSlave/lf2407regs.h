/***************************************************************************/
/* File name:	X24x.h                                                     */  
/* Project:		F2407 silicon functional test                      */
/* Originator:	DSP Digital Control Systems (DCS) Group (Houston)          */
/*                                                                         */       
/* Description:	F2407 register definitions.                                */ 
/*_________________________________________________________________________*/
/* Date of Mod	|			Description                        */
/*-------------------------------------------------------------------------*/
/* July. 10, 2004	| Created from header file used by liyp
  Copyright 2004--  liyp ltd. cmp. All rights reserved.
/*-------------------------------------------------------------------------*/
/* Data memory mapped registers
/*-------------------------------------------------------------------------*/

/* C2xx core registers*/
#ifndef _LF2407REGS_H
#define _LF2407REGS_H

volatile unsigned int *IMR      =	(volatile unsigned int *)0x0004;		// Interrupt Mask Register
volatile unsigned int *GREG	    =	(volatile unsigned int *)0x0005;		// Global memory allocation Register 
volatile unsigned int *IFR	    =	(volatile unsigned int *)0x0006;		// Interrupt Flag Register
volatile unsigned int *ABRPT    =	(volatile unsigned int *)0x001f;		// analyse breaker


volatile unsigned int *PIRQR0	=   (volatile unsigned int *)0x7010;		// Peripheral Interrupt Request Reg 0.
volatile unsigned int *PIRQR1	=   (volatile unsigned int *)0x7011;		// Peripheral Interrupt Request Reg 1.
volatile unsigned int *PIRQR2   =   (volatile unsigned int *)0x7012;		// Peripheral Interrupt Request Reg 2.

volatile unsigned int *PIACKR0	=   (volatile unsigned int *)0x7014;		// Peripheral interrupt acknowledge reg 0.
volatile unsigned int *PIACKR1	=   (volatile unsigned int *)0x7015;		// Peripheral interrupt acknowledge reg 1.
volatile unsigned int *PIACKR2	=   (volatile unsigned int *)0x7016;		// Peripheral interrupt acknowledge reg 2.

volatile unsigned int *SCSR1	=	(volatile unsigned int *)0x7018	;	// System Control & System Status Reg 1. 
volatile unsigned int *SCSR2	=	(volatile unsigned int *)0x7019	;	// System Control & System Status Reg 2.
volatile unsigned int *DIN	    =	(volatile unsigned int *)0x701C;		// Device Identification Register.
volatile unsigned int *PIVR	    =	(volatile unsigned int *)0x701E;		// Peripheral Interrupt Vector Reg. X241/2/3 only.

/* Watchdog (WD) registers*/      
volatile unsigned int *WDCNTR	=   (volatile unsigned int *)0x7023;	// WD Counter reg
volatile unsigned int *WDKEY	=	(volatile unsigned int *)0x7025	;	// WD Key reg
volatile unsigned int *WDCR	    =	(volatile unsigned int *)0x7029	;	// WD Control reg

/* SPI registers*/
volatile unsigned int *SPICCR	=   (volatile unsigned int *)0x7040;	// SPI Config Control Reg
volatile unsigned int *SPICTL	=   (volatile unsigned int *)0x7041;	// SPI Operation Control Reg
volatile unsigned int *SPISTS	=   (volatile unsigned int *)0x7042;	// SPI Status Reg
volatile unsigned int *SPIBRR	=   (volatile unsigned int *)0x7044;	// SPI Baud rate control reg
volatile unsigned int *SPIRXEMU	=   (volatile unsigned int *)0x7046;    // SPI Emulation buffer reg
volatile unsigned int *SPIRXBUF	=   (volatile unsigned int *)0x7047;	// SPI Serial receive buffer reg
volatile unsigned int *SPITXBUF	=   (volatile unsigned int *)0x7048	;	// SPI Serial transmit buffer reg
volatile unsigned int *SPIDAT	=   (volatile unsigned int *)0x7049;		// SPI Serial data reg
volatile unsigned int *SPIPC1	=   (volatile unsigned int *)0x704D;		// SPI Port Control Register 1
volatile unsigned int *SPIPC2   =   (volatile unsigned int *)0x704E	;	// SPI Port Control Register 2
volatile unsigned int *SPIPRI	=   (volatile unsigned int *)0x704F;		// SPI Priority control reg

/* SCI registers*/
volatile unsigned int *SCICCR	=   (volatile unsigned int *)0x7050;		// SCI Communication control reg
volatile unsigned int *SCICTL1	=   (volatile unsigned int *)0x7051;		// SCI Control reg1
volatile unsigned int *SCIHBAUD	=   (volatile unsigned int *)0x7052	;	// SCI Baud Rate MSbyte reg
volatile unsigned int *SCILBAUD	=   (volatile unsigned int *)0x7053	;	// SCI Baud Rate LSbyte reg
volatile unsigned int *SCICTL2	=   (volatile unsigned int *)0x7054;		// SCI Control reg2
volatile unsigned int *SCIRXST	=   (volatile unsigned int *)0x7055;		// SCI Receiver Status reg
volatile unsigned int *SCIRXEMU	=   (volatile unsigned int *)0x7056	;	// SCI Emulation Data Buffer reg
volatile unsigned int *SCIRXBUF	=   (volatile unsigned int *)0x7057	;	// SCI Receiver Data buffer reg
volatile unsigned int *SCITXBUF	=   (volatile unsigned int *)0x7059	;	// SCI Transmit Data buffer reg
volatile unsigned int *SCIPC2	=   (volatile unsigned int *)0x705E;		// SCI Port Control reg2 (X240 only)
volatile unsigned int *SCIPRI   =  	(volatile unsigned int *)0x705F	;	// SCI Priority control reg

/* External interrupt configuration registers*/
volatile unsigned int *XINT1CR	=   (volatile unsigned int *)0x7070;		// External interrupt 1 config reg
volatile unsigned int *XINT2CR  =	(volatile unsigned int *)0x7071	;	// External interrupt 2 config reg

/* Digital I/O registers*/
volatile unsigned int *MCRA	    =	(volatile unsigned int *)0x7090	;	// Output Control Reg A
volatile unsigned int *MCRB	    =	(volatile unsigned int *)0x7092	;	// Output Control Reg B
volatile unsigned int *MCRC	    =	(volatile unsigned int *)0x7094	;	// Output Control Reg C
volatile unsigned int *PADATDIR	=   (volatile unsigned int *)0x7098	;	// I/O port A Data & Direction reg.
volatile unsigned int *PBDATDIR	=   (volatile unsigned int *)0x709A	;	// I/O port B Data & Direction reg.
volatile unsigned int *PCDATDIR	=   (volatile unsigned int *)0x709C	;	// I/O port C Data & Direction reg.
volatile unsigned int *PDDATDIR	=   (volatile unsigned int *)0x709E	;	// I/O port D Data & Direction reg.
volatile unsigned int *PEDATDIR	=   (volatile unsigned int *)0x7095	;	// I/O port E Data & Direction reg.
volatile unsigned int *PFDATDIR	=   (volatile unsigned int *)0x7096	;	// I/O port F Data & Direction reg.

/* ADC registers*/
volatile unsigned int *ADCTRL1	=   (volatile unsigned int *)0x70a0;		// ADC Control Reg1
volatile unsigned int *ADCTRL2	=   (volatile unsigned int *)0x70a1;		// ADC Control Reg2
volatile unsigned int *MAXCONV	=   (volatile unsigned int *)0x70a2;		// MAX Converter Channel Reg
volatile unsigned int *CHSELSEQ1=   (volatile unsigned int *)0x70a3	;// Channel Select Reg1
volatile unsigned int *CHSELSEQ2=   (volatile unsigned int *)0x70a4	;	// Channel Select Reg2
volatile unsigned int *CHSELSEQ3=   (volatile unsigned int *)0x70a5	;	// Channel Select Reg3
volatile unsigned int *CHSELSEQ4=   (volatile unsigned int *)0x70a6	;	// Channel Select Reg4
volatile unsigned int *AUTO_SEQ_SR =(volatile unsigned int *)0x70a7	;	// auto Sequence  Reg
volatile unsigned int *RESULT0  =	(volatile unsigned int *)0x70a8	;	// Converter Result Reg0
volatile unsigned int *RESULT1  =	(volatile unsigned int *)0x70a9	;	// Converter Result Reg1
volatile unsigned int *RESULT2  =	(volatile unsigned int *)0x70aa	;	// Converter Result Reg2
volatile unsigned int *RESULT3  =	(volatile unsigned int *)0x70ab	;	// Converter Result Reg3
volatile unsigned int *RESULT4  =	(volatile unsigned int *)0x70ac	;	// Converter Result Reg4
volatile unsigned int *RESULT5  =	(volatile unsigned int *)0x70ad	;	// Converter Result Reg5
volatile unsigned int *RESULT6  =	(volatile unsigned int *)0x70ae	;	// Converter Result Reg6
volatile unsigned int *RESULT7  =	(volatile unsigned int *)0x70af	;	// Converter Result Reg7
volatile unsigned int *RESULT8  =	(volatile unsigned int *)0x70b0	;	// Converter Result Reg8
volatile unsigned int *RESULT9  =	(volatile unsigned int *)0x70b1	;	// Converter Result Reg9
volatile unsigned int *RESULT10 =	(volatile unsigned int *)0x70b2	;	// Converter Result Reg10
volatile unsigned int *RESULT11 = 	(volatile unsigned int *)0x70b3	;	// Converter Result Reg11
volatile unsigned int *RESULT12 =	(volatile unsigned int *)0x70b4	;	// Converter Result Reg12
volatile unsigned int *RESULT13 = 	(volatile unsigned int *)0x70b5	;	// Converter Result Reg13
volatile unsigned int *RESULT14 =	(volatile unsigned int *)0x70b6	;	// Converter Result Reg14
volatile unsigned int *RESULT15 =	(volatile unsigned int *)0x70b7	;	// Converter Result Reg15
volatile unsigned int *CALIBRATION=	(volatile unsigned int *)0x70b8	;	// Adjust Reg

/* CAN(SCC) registers*/ 
volatile unsigned int *CANMDER	=   (volatile unsigned int *)0x7100	;	// CAN Mailbox Direction/Enable reg
volatile unsigned int *CANTCR	=   (volatile unsigned int *)0x7101	;	// CAN Transmission Control Reg
volatile unsigned int *CANRCR	=   (volatile unsigned int *)0x7102	;	// CAN Recieve COntrol Reg
volatile unsigned int *CANMCR	=   (volatile unsigned int *)0x7103	;	// CAN Master Control Reg
volatile unsigned int *CANBCR2	=   (volatile unsigned int *)0x7104	;	// CAN Bit COnfig Reg 2
volatile unsigned int *CANBCR1	=   (volatile unsigned int *)0x7105	;	// CAN Bit Config Reg 1
volatile unsigned int *CANESR	=   (volatile unsigned int *)0x7106	;	// CAN Error Status Reg
volatile unsigned int *CANGSR	=   (volatile unsigned int *)0x7107	;	// CAN Global Status Reg
volatile unsigned int *CANCEC	=   (volatile unsigned int *)0x7108	;	// CAN Trans and Rcv Err counters
volatile unsigned int *CANIFR	=   (volatile unsigned int *)0x7109	;	// CAN Interrupt Flag Registers 
volatile unsigned int *CANIMR	=   (volatile unsigned int *)0x710a	;	// CAN Interrupt Mask Registers
volatile unsigned int *CANLAM0H	=   (volatile unsigned int *)0x710b	;	// CAN Local Acceptance Mask MBx0/1
volatile unsigned int *CANLAM0L	=   (volatile unsigned int *)0x710c	;	// CAN Local Acceptance Mask MBx0/1
volatile unsigned int *CANLAM1H	=   (volatile unsigned int *)0x710d	;	// CAN Local Acceptance Mask MBx2/3
volatile unsigned int *CANLAM1L	=   (volatile unsigned int *)0x710e	;	// CAN Local Acceptance Mask MBx2/3
volatile unsigned int *CANMSGID0L =	(volatile unsigned int *)0x7200	;	// CAN Message ID for mailbox 0 (lower 16 bits)	
volatile unsigned int *CANMSGID0H =	(volatile unsigned int *)0x7201	;	// CAN Message ID for mailbox 0 (upper 16 bits)	
volatile unsigned int *CANMSGCTRL0=	(volatile unsigned int *)0x7202	;	// CAN RTR and DLC	
volatile unsigned int *CANMBX0A	=   (volatile unsigned int *)0x7204	;	// CAN 2 of 8 bytes of Mailbox 0	
volatile unsigned int *CANMBX0B	=   (volatile unsigned int *)0x7205	;	// CAN 2 of 8 bytes of Mailbox 0	
volatile unsigned int *CANMBX0C	=   (volatile unsigned int *)0x7206	;	// CAN 2 of 8 bytes of Mailbox 0	
volatile unsigned int *CANMBX0D	=   (volatile unsigned int *)0x7207	;	// CAN 2 of 8 bytes of Mailbox 0	
volatile unsigned int *CANMSGID1L =	(volatile unsigned int *)0x7208	;	// CAN Message ID for mailbox 1 (lower 16 bits)	
volatile unsigned int *CANMSGID1H =	(volatile unsigned int *)0x7209	;	// CAN Message ID for mailbox 1 (upper 16 bits)	
volatile unsigned int *CANMSGCTRL1=	(volatile unsigned int *)0x720A	;	// CAN RTR and DLC	
volatile unsigned int *CANMBX1A	=   (volatile unsigned int *)0x720C	;	// CAN 2 of 8 bytes of Mailbox 1	
volatile unsigned int *CANMBX1B	=   (volatile unsigned int *)0x720D	;	// CAN 2 of 8 bytes of Mailbox 1	
volatile unsigned int *CANMBX1C	=   (volatile unsigned int *)0x720E	;	// CAN 2 of 8 bytes of Mailbox 1	
volatile unsigned int *CANMBX1D	=   (volatile unsigned int *)0x720F	;	// CAN 2 of 8 bytes of Mailbox 1	
volatile unsigned int *CANMSGID2L =	(volatile unsigned int *)0x7210	;	// CAN Message ID for mailbox 2 (lower 16 bits)	
volatile unsigned int *CANMSGID2H =	(volatile unsigned int *)0x7211	;	// CAN Message ID for mailbox 2 (upper 16 bits)	
volatile unsigned int *CANMSGCTRL2=	(volatile unsigned int *)0x7212	;	// CAN RTR and DLC	
volatile unsigned int *CANMBX2A	=   (volatile unsigned int *)0x7214	;	// CAN 2 of 8 bytes of Mailbox 2	
volatile unsigned int *CANMBX2B	=   (volatile unsigned int *)0x7215	;	// CAN 2 of 8 bytes of Mailbox 2	
volatile unsigned int *CANMBX2C	=   (volatile unsigned int *)0x7216	;	// CAN 2 of 8 bytes of Mailbox 2	
volatile unsigned int *CANMBX2D	=   (volatile unsigned int *)0x7217	;	// CAN 2 of 8 bytes of Mailbox 2	
volatile unsigned int *CANMSGID3L =	(volatile unsigned int *)0x7218	;	// CAN Message ID for mailbox 3 (lower 16 bits)	
volatile unsigned int *CANMSGID3H =	(volatile unsigned int *)0x7219	;	// CAN Message ID for mailbox 3 (upper 16 bits)	
volatile unsigned int *CANMSGCTRL3=	(volatile unsigned int *)0x721A	;	// CAN RTR and DLC	
volatile unsigned int *CANMBX3A =	(volatile unsigned int *)0x721C	;	// CAN 2 of 8 bytes of Mailbox 3	
volatile unsigned int *CANMBX3B =	(volatile unsigned int *)0x721D	;	// CAN 2 of 8 bytes of Mailbox 3	
volatile unsigned int *CANMBX3C =	(volatile unsigned int *)0x721E	;	// CAN 2 of 8 bytes of Mailbox 3	
volatile unsigned int *CANMBX3D =	(volatile unsigned int *)0x721F	;	// CAN 2 of 8 bytes of Mailbox 3	
volatile unsigned int *CANMSGID4L =	(volatile unsigned int *)0x7220	;	// CAN Message ID for mailbox 4 (lower 16 bits)	
volatile unsigned int *CANMSGID4H =	(volatile unsigned int *)0x7221	;	// CAN Message ID for mailbox 4 (upper 16 bits)	
volatile unsigned int *CANMSGCTRL4=	(volatile unsigned int *)0x7222	;	// CAN RTR and DLC	
volatile unsigned int *CANMBX4A	=   (volatile unsigned int *)0x7224	;	// CAN 2 of 8 bytes of Mailbox 4	
volatile unsigned int *CANMBX4B	=   (volatile unsigned int *)0x7225	;	// CAN 2 of 8 bytes of Mailbox 4	
volatile unsigned int *CANMBX4C	=   (volatile unsigned int *)0x7226	;	// CAN 2 of 8 bytes of Mailbox 4	
volatile unsigned int *CANMBX4D	=   (volatile unsigned int *)0x7227	;	// CAN 2 of 8 bytes of Mailbox 4	
volatile unsigned int *CANMSGID5L =	(volatile unsigned int *)0x7228	;	// CAN Message ID for mailbox 5 (lower 16 bits)	
volatile unsigned int *CANMSGID5H =	(volatile unsigned int *)0x7229	;	// CAN Message ID for mailbox 5 (upper 16 bits)	
volatile unsigned int *CANMSGCTRL5=	(volatile unsigned int *)0x722A	;	// CAN RTR and DLC	
volatile unsigned int *CANMBX5A	=   (volatile unsigned int *)0x722C	;	// CAN 2 of 8 bytes of Mailbox 5	
volatile unsigned int *CANMBX5B	=   (volatile unsigned int *)0x722D	;	// CAN 2 of 8 bytes of Mailbox 5	
volatile unsigned int *CANMBX5C	=   (volatile unsigned int *)0x722E	;	// CAN 2 of 8 bytes of Mailbox 5	
volatile unsigned int *CANMBX5D	=   (volatile unsigned int *)0x722F;

/* Event Manager (EV) registers*/ 
volatile unsigned int *GPTCONA	=   (volatile unsigned int *)0x7400;		// GP Timer control register. 
volatile unsigned int *T1CNT	=	(volatile unsigned int *)0x7401;		// GP Timer 1 counter register. 
volatile unsigned int *T1CMPR	=   (volatile unsigned int *)0x7402;		// GP Timer 1 compare register. 
volatile unsigned int *T1CMP    =	(volatile unsigned int *)0x7402;
volatile unsigned int *T1PER    =	(volatile unsigned int *)0x7403;          // GP Timer 1 period register.
volatile unsigned int *T1CON    =	(volatile unsigned int *)0x7404;	// GP Timer 1 control register. 
volatile unsigned int *T2CNT    =	(volatile unsigned int *)0x7405;		// GP Timer 2 counter register. 
volatile unsigned int *T2CMP    =	(volatile unsigned int *)0x7406;         // GP Timer 2 compare register.
volatile unsigned int *T2PER    =	(volatile unsigned int *)0x7407;  	// GP Timer 2 period register. 
volatile unsigned int *T2CON    =   (volatile unsigned int *)0x7408;	// GP Timer 2 control register. 

volatile unsigned int *COMCONA  =  	(volatile unsigned int *)0x7411;		// Compare control register.
volatile unsigned int *ACTRA    =   (volatile unsigned int *)0x7413;		// Full compare action control register.
volatile unsigned int *DBTCONA  = 	(volatile unsigned int *)0x7415;		// Dead-band timer control register. 

volatile unsigned int *CMPR1    =   (volatile unsigned int *)0x7417;		// Full compare unit compare register1. 
volatile unsigned int *CMPR2    =	(volatile unsigned int *)0x7418;		// Full compare unit compare register2.
volatile unsigned int *CMPR3    =	(volatile unsigned int *)0x7419;		// Full compare unit compare register3.
volatile unsigned int *CAPCONA  =  	(volatile unsigned int *)0x7420;		// Capture control register. 
volatile unsigned int *CAPFIFOA = 	(volatile unsigned int *)0x7422;		// Capture FIFO status register. 
volatile unsigned int *CAP1FIFO =	(volatile unsigned int *)0x7423;		// Capture Channel 1 FIFO Top
volatile unsigned int *CAP2FIFO =	(volatile unsigned int *)0x7424;		// Capture Channel 2 FIFO Top
volatile unsigned int *CAP3FIFO =	(volatile unsigned int *)0x7425;		// Capture Channel 3 FIFO Top
volatile unsigned int *CAP1FBOT =   (volatile unsigned int *)0x7427;              // Capture Channel 1 FIFO Bottom
volatile unsigned int *CAP2FBOT =   (volatile unsigned int *)0x7428;              // Capture Channel 2 FIFO Bottom
volatile unsigned int *CAP3FBOT =   (volatile unsigned int *)0x7429;              // Capture Channel 3 FIFO Bottom
			
volatile unsigned int *EVAIMRA  =	(volatile unsigned int *)0x742C;		// Group A Interrupt Mask Register
volatile unsigned int *EVAIMRB  =	(volatile unsigned int *)0x742D;		// Group B Interrupt Mask Register
volatile unsigned int *EVAIMRC  =	(volatile unsigned int *)0x742E;		// Group C Interrupt Mask Register
volatile unsigned int *EVAIFRA  =	(volatile unsigned int *)0x742F;		// Group A Interrupt Flag Register 
volatile unsigned int *EVAIFRB  =	(volatile unsigned int *)0x7430;		// Group B Interrupt Flag Register
volatile unsigned int *EVAIFRC  =	(volatile unsigned int *)0x7431;		// Group C Interrupt Flag register

volatile unsigned int *GPTCONB  =   (volatile unsigned int *)0x7500;		// Group C Interrupt Flag Register
volatile unsigned int *T3CNT    =   (volatile unsigned int *)0x7501;		// Group C Interrupt Flag Register
volatile unsigned int *T3CMP    =   (volatile unsigned int *)0x7502;		// Group C Interrupt Flag Register
volatile unsigned int *T3PER    =   (volatile unsigned int *)0x7503;		// Group C Interrupt Flag Register
volatile unsigned int *T3CON    =   (volatile unsigned int *)0x7504;		// Group C Interrupt Flag Register
volatile unsigned int *T4CNT    =   (volatile unsigned int *)0x7505;		// Group C Interrupt Flag Register
volatile unsigned int *T4CMP    =   (volatile unsigned int *)0x7506;		// Group C Interrupt Flag Register
volatile unsigned int *T4PER    =   (volatile unsigned int *)0x7507;		// Group C Interrupt Flag Register
volatile unsigned int *T4CON    =   (volatile unsigned int *)0x7508;		// Group C Interrupt Flag Register
volatile unsigned int *COMCONB  =   (volatile unsigned int *)0x7511;		// Group C Interrupt Flag Register
volatile unsigned int *ACTRB    =   (volatile unsigned int *)0x7513;		// Group C Interrupt Flag Register
volatile unsigned int *DBTCONB  =   (volatile unsigned int *)0x7515;		// Group C Interrupt Flag Register
volatile unsigned int *CMPR4    =   (volatile unsigned int *)0x7517;		// Group C Interrupt Flag Register
volatile unsigned int *CMPR5    =   (volatile unsigned int *)0x7518;		// Group C Interrupt Flag Register
volatile unsigned int *CMPR6    =   (volatile unsigned int *)0x7519;		// Group C Interrupt Flag Register
volatile unsigned int *CAPCONB  =   (volatile unsigned int *)0x7520;		// Group C Interrupt Flag Register
volatile unsigned int *CAPFIFOB =   (volatile unsigned int *)0x7522;		// Group C Interrupt Flag Register
volatile unsigned int *CAP4FIFO =   (volatile unsigned int *)0x7523;		// Group C Interrupt Flag Register
volatile unsigned int *CAP5FIFO =   (volatile unsigned int *)0x7524;		// Group C Interrupt Flag Register
volatile unsigned int *CAP6FIFO =   (volatile unsigned int *)0x7525;		// Group C Interrupt Flag Register
volatile unsigned int *CAP4FBOT =   (volatile unsigned int *)0x7527;		// Group C Interrupt Flag Register
volatile unsigned int *CAP5FBOT =   (volatile unsigned int *)0x7528;		// Group C Interrupt Flag Register
volatile unsigned int *CAP6FBOT =   (volatile unsigned int *)0x7529;		// Group C Interrupt Flag Register

volatile unsigned int *EVBIMRA  =   (volatile unsigned int *)0x752c;		// Group C Interrupt Flag Register
volatile unsigned int *EVBIMRB  =   (volatile unsigned int *)0x752d;		// Group C Interrupt Flag Register
volatile unsigned int *EVBIMRC  =   (volatile unsigned int *)0x752e;	// Group C Interrupt Flag Register
volatile unsigned int *EVBIFRA  =   (volatile unsigned int *)0x752f;		// Group C Interrupt Flag Register
volatile unsigned int *EVBIFRB  =   (volatile unsigned int *)0x7530;	// Group C Interrupt Flag Register
volatile unsigned int *EVBIFRC  =   (volatile unsigned int *)0x7531;		// Group C Interrupt Flag Register
//-----------------------------=--------------------------------------------
// I/O space mapped registers
//--------------------------------------------------------------------------
volatile unsigned int *WSGR   = (volatile unsigned int *)0xFFFF;	// Wait-State Generator Control Reg
//--------------------------------------------------------------------------
// Data Page pointer definition
//--------------------------------------------------------------------------


#define B2_SADDR  0x0060		//Page 1 of peripheral file (7000h/80h
#define B2_EADDR  0x007f		//Page 1 of peripheral file (7000h/80h
#define B0_SADDR  0x0200		//Page 1 of peripheral file (7000h/80h
#define B0_EADDR  0x02ff		//Page 1 of peripheral file (7000h/80h
#define B1_SADDR  0x0300		//Page 1 of peripheral file (7000h/80h
#define B1_EADDR  0x03ff		//Page 1 of peripheral file (7000h/80h
#define SARAM	  0x0800		//Page 1 of peripheral file (7000h/80h
#define EXTDATA	  0x8000		//Page 1 of peripheral file (7000h/80h

#define DP_B2	  0		//Page 1 of peripheral file (7000h/80h
#define DP_B01	  4		//Page 1 of peripheral file (7000h/80h
#define DP_B02	  5		//Page 1 of peripheral file (7000h/80h
#define DP_B11    6		//Page 1 of peripheral file (7000h/80h
#define DP_B12    7		//Page 1 of peripheral file (7000h/80h
#define DP_SARAM1 16		//Page 1 of peripheral file (7000h/80h
#define DP_PF1	  224		//Page 1 of peripheral file (7000h/80h
#define DP_PF2	  225		//Page 1 of peripheral file (7000h/80h
#define DP_PF3	  226		//Page 1 of peripheral file (7000h/80h
#define DP_PF4	  227		//Page 1 of peripheral file (7000h/80h
#define DP_PF5	  228		//Page 1 of peripheral file (7000h/80h
#define DP_EVA	  232		//EV Registers Page
#define DP_EVB	  234		//EV Registers Page
#define DP_EXT1	  256		//EV Registers Page

//--------------------------------------------------------------------------
// Bit codes for Test bit instruction (BIT) (15 Loads bit 0 into TC)
//--------------------------------------------------------------------------
#define BIT15		0x0000		// Bit Code for 15
#define BIT14		0x0001		// Bit Code for 14
#define BIT13		0x0002		// Bit Code for 13
#define BIT12		0x0003		// Bit Code for 12
#define BIT11		0x0004		// Bit Code for 11
#define BIT10		0x0005		// Bit Code for 10
#define BIT9		0x0006		// Bit Code for 9
#define BIT8		0x0007		// Bit Code for 8
#define BIT7		0x0008		// Bit Code for 7
#define BIT6		0x0009		// Bit Code for 6
#define BIT5		0x000A		// Bit Code for 5
#define BIT4		0x000B		// Bit Code for 4
#define BIT3		0x000C		// Bit Code for 3
#define BIT2		0x000D		// Bit Code for 2
#define BIT1		0x000E		// Bit Code for 1
#define BIT0		0x000F		// Bit Code for 0

#define B15_MSK		0x8000		// Bit MASK for 15
#define B14_MSK		0x4000		// Bit MASK for 14
#define B13_MSK		0x2000		// Bit MASK for 13
#define B12_MSK		0x1000		// Bit MASK for 12
#define B11_MSK		0x0800		// Bit MASK for 11
#define B10_MSK		0x0400		// Bit MASK for 10
#define B9_MSK		0x0200		// Bit MASK for 9
#define B8_MSK		0x0100		// Bit MASK for 8
#define B7_MSK		0x0080		// Bit MASK for 7
#define B6_MSK		0x0040		// Bit MASK for 6
#define B5_MSK		0x0020		// Bit MASK for 5
#define B4_MSK		0x0010		// Bit MASK for 4
#define B3_MSK		0x0008		// Bit MASK for 3
#define B2_MSK		0x0004		// Bit MASK for 2
#define B1_MSK		0x0002		// Bit MASK for 1
#define B0_MSK		0x0001		// Bit MASK for 0

#endif