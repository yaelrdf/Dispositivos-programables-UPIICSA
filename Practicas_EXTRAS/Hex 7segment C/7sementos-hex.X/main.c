/*
 * File:   main.c
 * Author: mcmin
 *
 * Created on October 6, 2025, 1:15 PM
 */

#include"config_bits.h"
#include <xc.h>

void main(void) {
    //Port Configs
    OSCCON=0x72;                /* Use internal oscillator of 8MHz Frequency */
    TRISB=0x00;                 /* Set direction of PORTB as OUTPUT to which LED is connected */
    LATA=0x00;
    //Input
    ADCON1 = 0x0F;              // All PORTA pins as digital
    TRISA=0xFF;
    LATB=0x00;
            
    while(1){
       //INPUT
       unsigned char input = PORTA;
       
       switch (input) {
        case 0x00:  // H
            LATB = 0x76;    // 7-segment pattern for 'H'
            break;
            
        case 0x01:  // o
            LATB = 0x5C;    // 7-segment pattern for 'o'
            break;
            
        case 0x02:  // l
            LATB = 0x0E;    // 7-segment pattern for 'l'
            break;
            
        case 0x03:  // a
            LATB = 0x5E;    // 7-segment pattern for 'a'
            break;
            
        case 0x04:  // -
            LATB = 0x40;    // 7-segment pattern for '-'
            break;
            
        case 0x05:  // I
            LATB = 0x06;    // 7-segment pattern for 'I'
            break;
            
        case 0x06:  // P
            LATB = 0x73;    // 7-segment pattern for 'P'
            break;
            
        case 0x07:  // N
            LATB= 0x54;    // 7-segment pattern for 'N'
            break;
            
        default:
            LATB = 0x00;    // Turn off all segments
        break;
    }
    }
    return;
}
