#define MS_TIME_LOOP 0x0385
#define TI_LED 0x000C
#define _TI_LED	000ch

#define STR(x) #x

#define OUTMAC(address,data)  \
 asm("        LDPK    _"STR(data));  \
 asm("        OUT     _"STR(data) "," STR(address))
 
#define INMAC(address,data)   \
 asm("        LDPK    _"STR(data));  \
 asm("        IN      _"STR(data) "," STR(address))
 
static volatile unsigned char ti_led_data = 0;

void Delay(int count)
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

unsigned int wait_ms( volatile unsigned int delay_val )
{
	unsigned int i;
	unsigned int ms_ctr;
	for ( i = 0; i < delay_val; i++ )
	{
		ms_ctr = MS_TIME_LOOP;

		while ( ms_ctr )
		{
			ms_ctr--;
		}
	}
	return( delay_val );
}

void ti_led_on( unsigned int led_num )  //打开DS4，DS5，DS6，DS7
{
	unsigned char pattern;
	pattern = 0x0001;
	while (--led_num)
	{
		pattern <<= 1;        /* move bit over */
	}
	ti_led_data |= pattern;
	OUTMAC( _TI_LED, ti_led_data);     /* turn on the led */
}


void ti_led_off( unsigned int led_num )//关闭DS4，DS5，DS6，DS7
{
	unsigned char pattern;
	pattern = 0x0001;
	while (--led_num)
	{
		pattern <<= 1;        /* move bit over */
	}
	ti_led_data &= (~pattern);
	OUTMAC( _TI_LED, ti_led_data);     /* turn off the led */
}

unsigned char ti_led_state()
{
	return ti_led_data;
}