#ifndef _LF2407_H
#define _LF2407_H

void inline disableINTM()
{
	asm(" setc INTM ");
}

void inline enableINTM()
{
	asm(" clrc INTM ");
}

#endif