#ifndef _LED_H
#define _LED_H
 
void Delay(int count);
unsigned int wait_ms( volatile unsigned int delay_val );
void ti_led_on( unsigned int led_num );
void ti_led_off( unsigned int led_num );
unsigned char ti_led_state();
#endif