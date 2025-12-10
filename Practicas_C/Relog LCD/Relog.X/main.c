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

    uint8_t minutos = 0;
    uint8_t segundos = 0;
    char tiempo_str[6]; // Para almacenar "MM:SS"
    
    while(1) {
        // Formatear el tiempo como "MM:SS"
        tiempo_str[0] = (minutos / 10) + '0';
        tiempo_str[1] = (minutos % 10) + '0';
        tiempo_str[2] = ':';
        tiempo_str[3] = (segundos / 10) + '0';
        tiempo_str[4] = (segundos % 10) + '0';
        tiempo_str[5] = '\0';
        
        // Limpiar y mostrar centrado en la fila 0
        lcd_clear();
        lcd_printCenter(tiempo_str, 0);
        
        // Esperar 1 segundo
        __delay_ms(1000);
        
        // Incrementar segundos
        segundos++;
        
        // Ajustar minutos y segundos
        if(segundos >= 60) {
            segundos = 0;
            minutos++;
            if(minutos >= 60) {
                minutos = 0; // Reiniciar después de 59:59
            }
        }
    }
}