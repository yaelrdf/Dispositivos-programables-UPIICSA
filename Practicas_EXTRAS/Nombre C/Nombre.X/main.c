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
            case 0x00:  // y
                LATB = 0b01110110;    // 7-segment pattern for '0'
                break;

            case 0x01:  // a
                LATB = 0b11101110;    // 7-segment pattern for '1'
                break;

            case 0x02:  // e
                LATB = 0b10011110;    // 7-segment pattern for '2'
                break;

            case 0x03:  // l
                LATB = 0b00011100;    // 7-segment pattern for '3'
                break;

            case 0x04:  // -
                LATB = 0b00000010;    // 7-segment pattern for '4'
                break;

            case 0x05:  // s
                LATB = 0b10110110;    // 7-segment pattern for '5'
                break;

            case 0x06:  // a
                LATB = 0b11101110;    // 7-segment pattern for '6'
                break;

            case 0x07:  // l
                LATB = 0b00011100;    // 7-segment pattern for '7'
                break;

            case 0x08:  // d
                LATB = 0b01111010;    // 7-segment pattern for '8'
                break;

            case 0x09:  // a
                LATB = 0b11101110;    // 7-segment pattern for '9'
                break;

            case 0x0A:  // n
                LATB = 0b00101010;    // 7-segment pattern for 'A'
                break;

            case 0x0B:  // a
                LATB = 0b11101110;    // 7-segment pattern for 'B'
                break;

            case 0x0C:  // f
                LATB = 0b10001110;    // 7-segment pattern for 'C'
                break;

            case 0x0D:  // l
                LATB = 0b00011100;    // 7-segment pattern for 'D'
                break;

            case 0x0E:  // o
                LATB = 0b11111101;    // 7-segment pattern for 'E'
                break;

            case 0x0F:  // r
                LATB = 0b00001010;    // 7-segment pattern for 'F'
                break;

            case 0x10:  //e
                LATB= 0b10011110;
                break;
            
            case 0x11:  //s
                LATB = 0b10110110;
                break;
            
            default:
                LATB = 0x01;    // Turn off all segments
                break;
        }
    }
    return;
}
