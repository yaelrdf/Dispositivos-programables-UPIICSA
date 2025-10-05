/*
 * File:   main.c
 * Author: yaelrdf
 *
 * Created on October 3, 2025, 1:08 AM
 */

#include "config_bits.h" 
#include <xc.h>

void MSdelay(unsigned int val)
{
 unsigned int i,j;
 for(i=0;i<val;i++)
     for(j=0;j<165;j++);         /*This count Provide delay of 1 ms for 8MHz Frequency */
 } 

void main()
{
    OSCCON=0x72;                /* Use internal oscillator of 8MHz Frequency */
    TRISB=0x00;                 /* Set direction of PORTB as OUTPUT to which LED is connected */
    LATA=0x00;
    //Input
    ADCON1 = 0x0F;              // All PORTA pins as digital
    TRISA=0xFF;
    LATB=0x00;
            
    while(1){
        //Input
        unsigned char input = PORTA;
        
        if(input==0b1101010){
            LATB=0x07;
        }
        else{
            LATB=0x27;
        }
        //LATB = 0x0F;            /* Turn ON LED for 500 ms */
        //MSdelay (500);
        //LATB = 0x00;            /* Turn OFF LED for 500 ms */
        //MSdelay (500);
        //LATB=PORTA;
    }
}