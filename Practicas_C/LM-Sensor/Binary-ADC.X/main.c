#include <xc.h>

// Configuration bits
#pragma config FOSC = INTOSC_EC
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000

void adc_init(void) {
    ADCON0 = 0b00000001;  // Enable ADC, 
    ADCON1 = 0b00001110;  
    ADCON2 = 0b10101010;  
}

unsigned int adc_read(void) {
    ADCON0bits.GO = 1;           
    while(ADCON0bits.GO);        
    return ((ADRESH << 8) + ADRESL);  // 10Bits
}

void main(void) {
    OSCCON = 0b01110000;  // 8MHz 
    TRISB = 0x00;         // PORTB
    PORTB = 0x00;         // Clear PORTB
    TRISA = 0xFF;         // PORTA as input
    adc_init();           // Initialize ADC
    
    while(1) {
        unsigned int adc_val = adc_read();      
        unsigned char temp = adc_val >> 1;  //Conversion        
        PORTB = temp;     // Display on PORTB
        __delay_ms(500); 
    }
}