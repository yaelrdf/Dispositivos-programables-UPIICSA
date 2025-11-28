#include "config_bits.h" 
#include <xc.h>

#define _XTAL_FREQ 8000000

/* Hexadecimal digit codes for 7-segment display */
const unsigned char DIGIT[] = {
    0b11111100,    /* 0 */
    0b01100000,    /* 1 */
    0b11011010,    /* 2 */
    0b11110010,    /* 3 */
    0b01100110,    /* 4 */
    0b10110110,    /* 5 */
    0b10111110,    /* 6 */
    0b11100000,    /* 7 */
    0b11111110,    /* 8 */
    0b11100110,    /* 9 */
    0b11101110,    /* A */
    0b00111110,    /* B */
    0b10011100,    /* C */
    0b01111010,    /* D */
    0b10011110,    /* E */
    0b10001110     /* F */
};

void main()
{
    TRISA = 0xFF;               /* Set PORTA as INPUT */
    TRISB = 0x00;               /* Set PORTB as OUTPUT */
    ADCON1 = 0x0F;
    OSCCON = 0x70;             /* Set internal oscillator to 8MHz */
    
    unsigned short CONTADOR = 0;
    
    while(1) {
        for (CONTADOR = 0; CONTADOR <= 15; CONTADOR++) {
            PORTB = DIGIT[CONTADOR];
            __delay_ms(500);
        }
    }
}