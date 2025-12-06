#include "config_bits.h" 
#include <xc.h>

#define _XTAL_FREQ 8000000

/* Hexadecimal digit codes for 7-segment display */
const unsigned char DIGIT[] = {
    0x9E,    /* E */
    0x1C,    /* L */
    0x9E,    /* E */
    0x9C,    /* C */
    0x8D,    /* T */
    0x0A,    /* R */
    0xFD,    /* O */
    0x9E,    /* E */
    0x2A,    /* N */
    0x9C,    /* C */
    0x9E,    /* E */
    0x8E,    /* F */
    0xEE,    /* A */
    0x1C,    /* L */
    0xFD,    /* O */
    0xBE,    /* G */
    0x0A,    /* R */
    0xEE,    /* A */
    0x8E,    /* F */
    0x0C,    /* I */
    0xB6,    /* S */
    0x8D,    /* T */
    0xEE     /* A */
};

const int DIGIT_LENGTH = 22;

void main()
{
    TRISA = 0xFF;               /* Set PORTA as INPUT */
    TRISB = 0x00;               /* Set PORTB as OUTPUT */
    ADCON1 = 0x0F;
    OSCCON = 0x70;             /* Set internal oscillator to 8MHz */
    
    unsigned short CONTADOR = 0;
    
    while(1) {
        for (CONTADOR = 0; CONTADOR <= DIGIT_LENGTH; CONTADOR++) {
            PORTB = DIGIT[CONTADOR];
            __delay_ms(500);
        }
    }
}