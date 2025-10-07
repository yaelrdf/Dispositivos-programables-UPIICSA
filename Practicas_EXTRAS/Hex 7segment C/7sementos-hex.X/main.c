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
                LATB = 0b11111110;    // 7-segment pattern for '0'
                break;

            case 0x01:  // 1
                LATB = 0b01100000;    // 7-segment pattern for '1'
                break;

            case 0x02:  // 2
                LATB = 0b11011010;    // 7-segment pattern for '2'
                break;

            case 0x03:  // 3
                LATB = 0b11110010;    // 7-segment pattern for '3'
                break;

            case 0x04:  // 4
                LATB = 0b01100110;    // 7-segment pattern for '4'
                break;

            case 0x05:  // 5
                LATB = 0b10110110;    // 7-segment pattern for '5'
                break;

            case 0x06:  // 6
                LATB = 0b10111110;    // 7-segment pattern for '6'
                break;

            case 0x07:  // 7
                LATB = 0b11100000;    // 7-segment pattern for '7'
                break;

            case 0x08:  // 8
                LATB = 0b11111110;    // 7-segment pattern for '8'
                break;

            case 0x09:  // 9
                LATB = 0b11100010;    // 7-segment pattern for '9'
                break;

            case 0x0A:  // A
                LATB = 0b11101110;    // 7-segment pattern for 'A'
                break;

            case 0x0B:  // B
                LATB = 0b00111110;    // 7-segment pattern for 'B'
                break;

            case 0x0C:  // C
                LATB = 0b10011100;    // 7-segment pattern for 'C'
                break;

            case 0x0D:  // D
                LATB = 0b01111010;    // 7-segment pattern for 'D'
                break;

            case 0x0E:  // E
                LATB = 0b10011110;    // 7-segment pattern for 'E'
                break;

            case 0x0F:  // F
                LATB = 0b10001110;    // 7-segment pattern for 'F'
                break;

            default:
                LATB = 0b00000001;
                break;
        }
    }
    return;
}
