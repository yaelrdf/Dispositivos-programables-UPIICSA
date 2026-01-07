#include <xc.h>
#include <stdint.h>
#include "lcd.h"
#include "config_bits.h"

#define _XTAL_FREQ 8000000

// Custom character (convertido a formato LCD-x8c: 8 filas de 5 bits)
const uint8_t custom_char[8] = {
    0b10001,   // 0x11 -> 10001
    0b11011,   // 0x1B -> 11011
    0b10101,   // 0x15 -> 10101
    0b10001,   // 0x11 -> 10001
    0b10001,   // 0x11 -> 10001
    0b01010,   // 0x0A -> 01010
    0b00100,   // 0x04 -> 00100
    0b00000    // 0x00 -> 00000
};

void main(void) {
    // Configuración de puertos
    TRISA = 0xFF;               // Set PORTA as INPUT
    TRISB = 0x00;               // Set PORTB as OUTPUT
    ADCON1 = 0x0F;              // Puertos analógicos como digitales
    OSCCON = 0x70;              // Set internal oscillator to 8MHz
    
    // Initialize LCD (16 columnas, 2 filas)
    lcd_begin(16, 2);
    
    // Create custom character
    lcd_createChar(0, custom_char);
    
    while(1) {
        // Display messages
        lcd_setCursor(0, 0);
        lcd_print("Holiiiss");
        
        __delay_ms(2000);
        
        // Second line
        lcd_setCursor(0, 1);
        lcd_print("Sabias que:");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Te amo");
        lcd_setCursor(0, 1);
        lcd_print("Mucho <3");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Eres muy");
        lcd_setCursor(0, 1);
        lcd_print("Importante");
        
        __delay_ms(2000);
        
        // Efecto especial: limpiar y mostrar "para mi"
        lcd_setCursor(0, 1);
        lcd_print("                "); // Limpiar línea 1
        lcd_setCursor(0, 1);
        lcd_print("para mi");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Me encantan:");
        lcd_setCursor(0, 1);
        lcd_print("Tus abrazos");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Me encantan:");
        lcd_setCursor(0, 1);
        lcd_print("Tus besos");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Me encanta:");
        lcd_setCursor(0, 1);
        lcd_print("Tu sonrisa");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Me encanta");
        lcd_setCursor(0, 1);
        lcd_print("Tu persona");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Me encanta");
        lcd_setCursor(0, 1);
        lcd_print("Tu amor");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Bestabe");
        lcd_setCursor(0, 1);
        lcd_print("Te amo muchisimo <3");
        
        __delay_ms(2000);
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("No lo olvides");
        lcd_setCursor(0, 1);
        lcd_print(":)");
        
        __delay_ms(2000);
        lcd_clear();
    }
}