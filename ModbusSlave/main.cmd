/****************************************************************************/
/*  LINKER COMMAND FILE - MEMORY SPECIFICATION for C240/243 		    */
/*  									    */
/****************************************************************************/
-c                /* ROM autoinitialization model */
-m main.map       /* Create a map file            */
- stack 40
 
/****************************************************************************/
/******** Specify the memory configuration **********************************/
/****************************************************************************/
MEMORY
{
	/* Program */
    PAGE 0: VECS:    origin =  0000h, length = 0040h  /* Interrupt Vectors */
    		PVECS:   origin =  0044h, length = 0080h  /*  */
            PROG:    origin =  00C4h, length = 7F3Ch  /* FLASH */
			SARAM_P: origin =  8000h, length = 0800h  /* SARAM */
			EX1_PM:  origin =  8800h, length = 7600h  /* External */
			B0:      origin = 0FF00h, length = 0100h  /* DARAM */
	/* Data */
    PAGE 1: MMRS:    origin =  0000h, length = 0060h  /* Memory-Mapped, Registers, Reserved Addresses */
    		B2:      origin =  0060h, length = 0020h  /* DARAM */
            B0:      origin =  0200h, length = 0100h  /* DARAM */
            B1:      origin =  0300h, length = 0100h  /* DARAM */
            SARAM_D: origin =  0800h, length = 0800h  /* SARAM */
			PERIPH:  origin =  7000h, length = 1000h  /* Peripheral Memory-Mapped Registers */
            EX2_DM:  origin =  8000h, length = 8000h  /* External */

}
/*--------------------------------------------------------------------------*/
/* SECTIONS ALLOCATION                                                      */
/*--------------------------------------------------------------------------*/
SECTIONS
{
	.reset	 : { } > VECS	  PAGE 0
    .vectors : { } > VECS  	  PAGE 0
    .pvecs   : { } > PVECS	  PAGE 0
	.text    : { } > PROG	  PAGE 0    /* Executable code and constants */
    .cinit   : { } > PROG	  PAGE 0    /* Tables for explicitly initialized global and static variables */
    .switch  : { } > PROG	  PAGE 0    /* Tables for implementing switch statements */
	.data    : { } > B1       PAGE 1
	.const   : { } > B2  	  PAGE 1    /* Global and static const variables that are explicitly initialized and that are string literals */
    .bss     : { } > SARAM_D  PAGE 1    /* Global and static variables */
    .stack   : { } > SARAM_D  PAGE 1    /* Software stack area */
    .sysmem  : { } > SARAM_D  PAGE 1    /* Dynamic memory area for malloc functions */
}