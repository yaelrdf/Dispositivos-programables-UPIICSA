#include <xc.h>
#include "lcd.h"
#include "config_bits.h"

#define _XTAL_FREQ 8000000

void main(void) {
    // Initialize oscillator and ports
    OSCCON = 0x72;  // Internal oscillator setup
    ADCON1 = 0x0F;  // All digital I/O
    CMCON = 0x07;   // Comparators off
    
    // Initialize LCD (16 columns, 2 rows)
    lcd_begin(16, 2);

    while(1) {
        // Primera pantalla
        lcd_clear();
        lcd_printCenter("IPN", 0);
        __delay_ms(1000);
        lcd_printCenter("UPIICSA", 1);
        __delay_ms(2000);
        
        // Segunda pantalla
        lcd_clear();
        lcd_printCenter("Dispositivos", 0);
        __delay_ms(1000);
        lcd_printCenter("Programables", 1);
        __delay_ms(2000);
    }
}