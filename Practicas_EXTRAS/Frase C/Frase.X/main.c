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
        case 0x00:  // N
            LATB = 0x54;    // 7-segment pattern for 'N'
            break;

        case 0x01:  // o
            LATB = 0x5C;    // 7-segment pattern for 'o'
            break;

        case 0x02:  // -
            LATB = 0x40;    // 7-segment pattern for '-'
            break;

        case 0x03:  // g
            LATB = 0x6F;    // 7-segment pattern for 'g'
            break;

        case 0x04:  // u
            LATB = 0x1C;    // 7-segment pattern for 'u'
            break;

        case 0x05:  // s
            LATB = 0x6D;    // 7-segment pattern for 's'
            break;

        case 0x06:  // t
            LATB = 0x78;    // 7-segment pattern for 't'
            break;

        case 0x07:  // a
            LATB = 0x5E;    // 7-segment pattern for 'a'
            break;

        case 0x08:  // -
            LATB = 0x40;    // 7-segment pattern for '-'
            break;

        case 0x09:  // p
            LATB = 0x73;    // 7-segment pattern for 'p'
            break;

        case 0x0A:  // r
            LATB = 0x50;    // 7-segment pattern for 'r'
            break;

        case 0x0B:  // o
            LATB = 0x5C;    // 7-segment pattern for 'o'
            break;

        case 0x0C:  // t
            LATB = 0x78;    // 7-segment pattern for 't'
            break;

        case 0x0D:  // e
            LATB = 0x79;    // 7-segment pattern for 'e'
            break;

        case 0x0E:  // u
            LATB = 0x1C;    // 7-segment pattern for 'u'
            break;

        case 0x0F:  // s
            LATB = 0x6D;    // 7-segment pattern for 's'
            break;

        default:
            LATB = 0x00;    // Turn off all segments
            break;
        }
    }
    return;
}
