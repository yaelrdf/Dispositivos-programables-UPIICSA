#include <xc.h>
#include "lcd.h"

#pragma config FOSC = HS
#define _XTAL_FREQ 8000000

void main(void) {
    // ALL digital first
    ADCON1 = 0x0F;
    
    lcd_begin(16, 2);
    
    //ADC
    TRISA0 = 1;
    ADCON0 = 0x01;
    ADCON1 = 0x0E;
    ADCON2 = 0x8A;
    
    while(1) {
        ADCON0bits.GO = 1;
        while(ADCON0bits.GO);
        
        int adc = (ADRESH << 8) | ADRESL;
        int temp = (adc * 500) / 1023;
        
        lcd_clear();
        lcd_print("ADC:");
        lcd_printNum(adc);
        lcd_setCursor(0,1);
        lcd_print("Temp:");
        lcd_printNum(temp);
        
        __delay_ms(1000);
    }
}