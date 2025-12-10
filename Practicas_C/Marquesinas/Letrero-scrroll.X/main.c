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
    
    // Texto que se desplazará (puedes cambiarlo)
    char *texto_desplazante = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam tristique finibus massa, id placerat felis porttitor eu. Morbi ac nulla. ";
    
    // Texto estático en la fila superior (puedes cambiarlo)
    char *texto_estatico = "Yael RDF";
    
    while(1) {
        // Mostrar texto estático en la fila 0
        lcd_clear();
        lcd_printCenter(texto_estatico, 1);
        
        // Desplazar texto en la fila 1
        lcd_scrollText(texto_desplazante, 0, 100);
        
        // Pequeña pausa antes de repetir
        __delay_ms(1000);
    }
}