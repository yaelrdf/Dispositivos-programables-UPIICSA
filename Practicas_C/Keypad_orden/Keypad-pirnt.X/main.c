#include <xc.h>
#include "config_bits.h"
#include "lcd.h"
#include "keypad.h"

// Definir frecuencia para delays
#define _XTAL_FREQ 8000000

// Configurar OSCCON para 48MHz
#define OSCCON_CONFIG 0x72

// Declaraciones de funciones
void inicializar_sistema(void);
void mostrar_mensaje_inicial(void);

// Variables globales
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad_t miTeclado;

void main(void) {
    char tecla_actual;
    char tecla_anterior = NO_KEY;
    
    // Inicializar sistema
    inicializar_sistema();
    
    // Inicializar LCD y Keypad
    lcd_begin(16, 2);
    Keypad_Init(&miTeclado, teclas);
    
    // Mostrar mensaje de bienvenida
    mostrar_mensaje_inicial();
    
    while(1) {
        // Leer tecla actual
        tecla_actual = Keypad_GetKey(&miTeclado);
        
        // Solo procesar si hay una tecla nueva presionada
        if (tecla_actual != NO_KEY && tecla_actual != tecla_anterior) {
            // Limpiar solo la fila superior
            lcd_setCursor(0, 0);
            for(uint8_t i = 0; i < 16; i++) {
                lcd_write(' ');
            }
            
            // Centrar y mostrar la tecla
            lcd_setCursor(7, 1);  // Posición centrada (en medio de 16 caracteres)
            lcd_write(tecla_actual);
            
            // Mostrar información en la fila inferior
            lcd_setCursor(0, 1);
            lcd_print("Tecla: ");
            lcd_write(tecla_actual);
            lcd_print("     ");  // Espacios para limpiar resto de línea
            
            // Actualizar tecla anterior
            tecla_anterior = tecla_actual;
            
            // Debounce manual
            __delay_ms(200);
        }
        
        // Pequeña pausa para evitar uso excesivo de CPU
        __delay_ms(10);
    }
}

void inicializar_sistema(void) {
    // Configurar OSCCON para 48MHz
    OSCCON = OSCCON_CONFIG;
    TRISD = 0x00;  // Todo PORTD como salidas para LCD
    TRISB = 0xF0;  // RB4-RB7 como entradas (1), RB0-RB3 como salidas (0)
    
    // Configurar puertos digitales
    ADCON1 = 0x0F;  // Todo digital
}

void mostrar_mensaje_inicial(void) {
    lcd_clear();
    __delay_ms(2000);
    
    // Preparar display
    lcd_clear();
    
    lcd_setCursor(0, 1);
    lcd_print("Tecla: ");
}