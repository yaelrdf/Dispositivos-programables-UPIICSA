#include <xc.h>
#include "config_bits.h"
#include "lcd.h"
#include "keypad.h"

#define _XTAL_FREQ 8000000  // Define tu frecuencia aquí

// Mapa del teclado
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad_t miTeclado;
volatile unsigned int piso = 0;  // Added volatile for interrupt safety
volatile unsigned char emergency_stop = 0;  // Emergency flag

// Pantalla principal
void idle(void) {
    lcd_clear();
    lcd_printCenter("Bienvenido", 0);
    lcd_setCursor(0, 1);
    lcd_print("Piso: ");
    lcd_printNum(piso);
}

void emergency_stop_sequence(void) {
    emergency_stop = 1;  // Set emergency flag
    lcd_clear();
    lcd_printCenter("PARADA", 0);
    lcd_printCenter("EMERGENCIA", 1);
    
    // Wait for reset or acknowledgment
    while (1) {
        // Check if 'C' is pressed to reset (or any other key you prefer)
        if (Keypad_IsPressed(&miTeclado, 'C')) {
            emergency_stop = 0;  // Clear emergency flag
            __delay_ms(500);     // Debounce delay
            break;
        }
        __delay_ms(10);  // Small delay to avoid busy waiting
    }
    
    idle();  // Return to idle state
}

// Modified movement functions with emergency check
void subir(void) {
    unsigned int steps = 0;
    emergency_stop = 0;  // Clear emergency flag at start
    
    lcd_clear();
    lcd_printCenter("Subiendo...", 0);
    lcd_setCursor(0, 1);
    lcd_print("Piso: ");
    lcd_printNum(piso);
    
    // Simulate movement with emergency check
    while (steps < 10 && !emergency_stop) {  // 10 steps simulation
        __delay_ms(100);  // Movement delay
        
        // Check for emergency stop during movement
        if (Keypad_IsPressed(&miTeclado, 'D')) {
            emergency_stop_sequence();
            return;  // Exit if emergency activated
        }
        
        steps++;
    }
    
    // Only increment floor if no emergency
    if (!emergency_stop) {
        piso++;
        idle();
    }
}

void bajar(void) {
    unsigned int steps = 0;
    emergency_stop = 0;  // Clear emergency flag at start
    
    lcd_clear();
    lcd_printCenter("Bajando...", 0);
    lcd_setCursor(0, 1);
    lcd_print("Piso: ");
    lcd_printNum(piso);
    
    // Simulate movement with emergency check
    while (steps < 10 && !emergency_stop) {  // 10 steps simulation
        __delay_ms(100);  // Movement delay
        
        // Check for emergency stop during movement
        if (Keypad_IsPressed(&miTeclado, 'D')) {
            emergency_stop_sequence();
            return;  // Exit if emergency activated
        }
        
        steps++;
    }
    
    // Only decrement floor if no emergency
    if (!emergency_stop) {
        piso--;
        idle();
    }
}

// Function to check for emergency stop continuously
void check_emergency_during_delay(unsigned int delay_ms) {
    unsigned int i;
    for (i = 0; i < delay_ms / 10; i++) {
        if (Keypad_IsPressed(&miTeclado, 'D')) {
            emergency_stop_sequence();
            break;
        }
        __delay_ms(10);
    }
}

void main(void) {
    // Configuracion del PIC
    OSCCON = 0x72;  // Configurar oscilador a 8MHz
    CMCON = 0x07;  //
    TRISB = 0xF0;  // RB4-RB7 como entradas (1), RB0-RB3 como salidas (0)
    ADCON1 = 0x0F;  // Todo digital
    
    // Inicializar LCD
    lcd_begin(16, 2);
    
    // Inicializar teclado
    Keypad_Init(&miTeclado, teclas);
    
    emergency_stop = 0;  // Initialize emergency flag
    idle();

    while (1) {
        // Check for emergency stop even in idle
        if (Keypad_IsPressed(&miTeclado, 'D')) {
            emergency_stop_sequence();
            continue;  // Skip the rest of the loop
        }
        
        // Esperar entrada del usuario with emergency check
        char tecla = Keypad_WaitForKey(&miTeclado);
        
        // Check if emergency was pressed while waiting
        if (emergency_stop) {
            continue;
        }
        
        switch (tecla) {
            case 'A':
                if (piso < 2 && !emergency_stop) {
                    subir();
                } else if (!emergency_stop) {
                    lcd_clear();
                    lcd_printCenter("Piso mas alto", 0);
                    lcd_printCenter("alcanzado", 1);
                    check_emergency_during_delay(2000);
                    if (!emergency_stop) idle();
                }
                break;
            case 'B':
                if (piso > 0 && !emergency_stop) {
                    bajar();
                } else if (!emergency_stop) {
                    lcd_clear();
                    lcd_printCenter("Piso mas bajo", 0);
                    lcd_printCenter("alcanzado", 1);
                    check_emergency_during_delay(2000);
                    if (!emergency_stop) idle();
                }
                break;
            case 'D':
                emergency_stop_sequence();
                break;
            default:
                // Otra tecla - mostrar mensaje
                if (!emergency_stop) {
                    lcd_clear();
                    lcd_printCenter("Tecla no valida", 0);
                    check_emergency_during_delay(1000);
                    if (!emergency_stop) idle();
                }
                break;
        }
    }
}