#include "config_bits.h" 
#include <xc.h>

void main()
{
    TRISA=0xFF;                 /* Set PORTA as INPUT (connected to switches) */
    TRISB=0x00;                 /* Set PORTB as OUTPUT (connected to LEDs) */
    ADCON1=0x0F;                /* Disable analog conversion on all pins */
            
    while(1){
        if(PORTA == 0x05){   
            PORTB=0x01;      
        }
        else {
            PORTB=0x04;      
        }
    }
}