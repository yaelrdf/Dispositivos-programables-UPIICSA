#include <xc.h>
#include "lcd.h"
#include "config_bits.h"

#define _XTAL_FREQ 8000000

void main(void) {
    // Initialize oscillator and ports
    OSCCON = 0x72;  // Internal oscillator setup
    ADCON1 = 0x0F;  // All digital I/O
    CMCON = 0x07;   // Comparators off
    TRISD = 0x00;  // PORTD as output for LCD
    
    lcd_begin(16, 2);
    
    // Partes de la canción (cada una para una fila)
    const char* partes_cancion[] = {
        // Primera tanda: fila 0 -> fila 1
        "Que se siente en las noches sin poder dormir",
        "Se acumulan los viernes que no quieres salir",
        
        // Segunda tanda: fila 0 -> fila 1
        "Los domingos de bajon te da por pedir perdon es asi",
        "Y que bonito ahora te toca a ti",
        
        // Tercera tanda: fila 0 -> fila 1
        "Te juro que no quiero ser cruel",
        "Tu pasaste la pagina y te corto el papel",
        
        // Cuarta tanda: fila 0 -> fila 1
        "Es domingo de bajon y otra vez pides perdon ahora si",
        "Yo esa pelicula ya me la vi",
        
        // Quinta tanda: fila 0 -> fila 1
        "No vuelvas cuando no te quiera nadie",
        "Ni cuando planes salgan al reves",
        
        // Sexta tanda: fila 0 -> fila 1
        "si no sirve mi amor ultimo rie mejor",
        "porque no has vuelto a reir otra vez",
        
    };
    
    int total_partes = 12; // 11 tandas × 2 líneas = 22 partes
    
    // Primero: inicialización con texto estático y desplazante
    lcd_clear();
    lcd_printCenter("Morat", 0);
    lcd_scrollText("Domingo de bajon", 1, 250);
    __delay_ms(2000);
    
    // Mostrar las partes de la canción
    for (int i = 0; i < total_partes; i += 2) {
        // Mostrar primera parte en fila 0 con scroll
        lcd_clear();
        lcd_scrollText(partes_cancion[i], 0, 100);
        
        // Pequeña pausa entre filas
        __delay_ms(500);
        
        // Mostrar segunda parte en fila 1 con scroll
        lcd_clear();
        lcd_scrollText(partes_cancion[i + 1], 1, 100);
        
        // Pausa entre tandas
        __delay_ms(1500);
    }
    
    // Mensaje final
    lcd_clear();
    lcd_printCenter("FIN", 0);
    lcd_printCenter("<3", 1);
    
    while(1) {
        // Mantener el mensaje final
        __delay_ms(1000);
    }
}