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
            case 0x00:  // 0
                LATB = 0x3F;    // 7-segment pattern for '0'
                break;

            case 0x01:  // 1
                LATB = 0x06;    // 7-segment pattern for '1'
                break;

            case 0x02:  // 2
                LATB = 0x5B;    // 7-segment pattern for '2'
                break;

            case 0x03:  // 3
                LATB = 0x4F;    // 7-segment pattern for '3'
                break;

            case 0x04:  // 4
                LATB = 0x66;    // 7-segment pattern for '4'
                break;

            case 0x05:  // 5
                LATB = 0x6D;    // 7-segment pattern for '5'
                break;

            case 0x06:  // 6
                LATB = 0x7D;    // 7-segment pattern for '6'
                break;

            case 0x07:  // 7
                LATB = 0x07;    // 7-segment pattern for '7'
                break;

            case 0x08:  // 8
                LATB = 0x7F;    // 7-segment pattern for '8'
                break;

            case 0x09:  // 9
                LATB = 0x6F;    // 7-segment pattern for '9'
                break;

            case 0x0A:  // A
                LATB = 0x77;    // 7-segment pattern for 'A'
                break;

            case 0x0B:  // B
                LATB = 0x7C;    // 7-segment pattern for 'B'
                break;

            case 0x0C:  // C
                LATB = 0x39;    // 7-segment pattern for 'C'
                break;

            case 0x0D:  // D
                LATB = 0x5E;    // 7-segment pattern for 'D'
                break;

            case 0x0E:  // E
                LATB = 0x79;    // 7-segment pattern for 'E'
                break;

            case 0x0F:  // F
                LATB = 0x71;    // 7-segment pattern for 'F'
                break;

            default:
                LATB = 0x00;    // Turn off all segments
                break;
        }
    }
    return;
}
