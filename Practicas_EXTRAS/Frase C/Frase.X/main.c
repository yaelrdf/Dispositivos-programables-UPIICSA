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
       //For
       //Electroencefalografista
       switch (input) {
        case 0x00:  // E
            LATB = 0b10011110;
            break;

        case 0x01:  // L
            LATB = 0b00011100;
            break;

        case 0x02:  // E
            LATB = 0b10011110;
            break;

        case 0x03:  // C
            LATB = 0b10011100;
            break;

        case 0x04:  // T
            LATB = 0b10001101;
            break;

        case 0x05:  // R
            LATB = 0b00001010;
            break;

        case 0x06:  // O
            LATB = 0b11111101;
            break;

        case 0x07:  // E
            LATB = 0b10011110;
            break;

        case 0x08:  // N
            LATB = 0b00101010;
            break;

        case 0x09:  // C
            LATB = 0b10011100;
            break;

        case 0x0A:  // E
            LATB = 0b10011110;
            break;

        case 0x0B:  // F
            LATB = 0b10001110;
            break;

        case 0x0C:  // A
            LATB = 0b11101110;
            break;

        case 0x0D:  // L
            LATB = 0b00011100;
            break;

        case 0x0E:  // O
            LATB = 0b11111101;
            break;

        case 0x0F:  // G
            LATB = 0b10111110;
            break;

        case 0x10: //R
            LATB = 0b00001010;
            break;

        case 0x11: //A
            LATB = 0b11101110;    
            break;
        
        case 0x12: //F
            LATB = 0b10001110;
            break;
        
        case 0x13: //I
            LATB = 0b00001100;
            break;
        
        case 0x14: //S
            LATB = 0b10110110;
            break;
        
        case 0x15: //T
            LATB = 0b10001101;
            break;
        
        case 0x16: //A
            LATB = 0b11101110;
            break;

        default:
            LATB = 0b00000001;
            break;
        }
    }
    return;
}
