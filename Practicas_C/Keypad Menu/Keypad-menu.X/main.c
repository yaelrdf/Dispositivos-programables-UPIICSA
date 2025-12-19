#include <xc.h>
#include "lcd.h"
#include "keypad.h"

#define _XTAL_FREQ 8000000  // Define tu frecuencia aqu�

// Mapa del teclado
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad_t miTeclado;

// Crear caracteres personalizados
void crear_caracteres_personalizados(void) {
    // Car�cter para la '�' - FIXED to look more like "�"
    uint8_t enie[8] = {
        0b11111,
        0b00000,
        0b01110,
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b10001
    };
    
    // �rbol de navidad
    uint8_t arbol[8] = {
        0b00100,
        0b01110,
        0b01110,
        0b11111,
        0b00100,
        0b01110,
        0b10101,
        0b00100
    };
    
    // Estrella
    uint8_t estrella[8] = {
        0b00100,
        0b10101,
        0b01110,
        0b11011,
        0b01110,
        0b10101,
        0b00100,
        0b00000
    };
    
    // Campana
    uint8_t campana[8] = {
        0b00100,
        0b01110,
        0b01110,
        0b01110,
        0b11111,
        0b00000,
        0b00100,
        0b00000
    };
    
    // Copo de nieve
    uint8_t copo_nieve[8] = {
        0b00100,
        0b10101,
        0b01110,
        0b11011,
        0b01110,
        0b10101,
        0b00100,
        0b00000
    };
    
    // Regalo
    uint8_t regalo[8] = {
        0b11111,
        0b10001,
        0b10101,
        0b10001,
        0b10101,
        0b10001,
        0b11111,
        0b00000
    };
    
    // Bloque completo para barra de carga (posici�n 6)
    uint8_t bloque_completo[8] = {
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111
    };
    
    lcd_createChar(0, enie);         // � en posici�n 0
    lcd_createChar(1, arbol);        // �rbol en posici�n 1
    lcd_createChar(2, estrella);     // estrella en posici�n 2
    lcd_createChar(3, campana);      // campana en posici�n 3
    lcd_createChar(4, copo_nieve);   // copo en posici�n 4
    lcd_createChar(5, regalo);       // regalo en posici�n 5
    lcd_createChar(6, bloque_completo); // bloque completo en posici�n 6
}

// Pantalla principal
void mostrar_menu_principal(void) {
    lcd_clear();
    lcd_printCenter("== MENU ==", 0);
    lcd_setCursor(0, 1);
    lcd_print("A,B,C,D");
}

// Opci�n A: Feliz Navidad con decoraciones
void opcion_a(void) {
    lcd_clear();
    
    lcd_setCursor(2, 0);
    lcd_write(2);  // estrella
    lcd_print(" Feliz ");
    lcd_write(2);  // estrella
    
    lcd_setCursor(2, 1);
    lcd_write(1);  // �rbol
    lcd_print("Navidad");
    lcd_write(1);  // �rbol  
    
    while (1) {
        // Verificar si se presion� '*'
        char tecla = Keypad_GetKey(&miTeclado);
        if (tecla == '*') {
            break;
        }
        
        // Alternar decoraciones en los bordes
        static uint8_t decoracion = 3;
        lcd_setCursor(0, 0);
        lcd_write(decoracion);
        lcd_setCursor(15, 0);
        lcd_write(decoracion);
        lcd_setCursor(0, 1);
        lcd_write((decoracion + 1) % 6);
        lcd_setCursor(15, 1);
        lcd_write((decoracion + 1) % 6);
        
        decoracion = (decoracion + 1) % 6;
        
        __delay_ms(500);
    }
}

// Opci�n B: Texto decorativo de Navidad
void opcion_b(void) {
    lcd_clear();
    
    lcd_printCenter("Yael", 0);
    lcd_printCenter("RDF", 1);
    
    // Esperar hasta que se presione '*'
    while (1) {
        char tecla = Keypad_GetKey(&miTeclado);
        if (tecla == '*') {
            return;
        }
        __delay_ms(10);
    }
}

// Opci�n C: Barra de carga con "A�o" con �
void opcion_c(void) {
    lcd_clear();
    lcd_printCenter("Cargando...", 0);
    
    // Barra de progreso simple usando el car�cter de bloque completo
    for (int pos = 0; pos < 16; pos++) {
        lcd_setCursor(pos, 1);
        lcd_write(6);  // Usar el bloque completo
        __delay_ms(100);
        
        // Verificar si se presion� '*'
        char tecla = Keypad_GetKey(&miTeclado);
        if (tecla == '*') {
            return;
        }
    }
    
    // Mensaje final con "�"
    lcd_clear();
    lcd_setCursor(4, 0);
    lcd_print("Feliz A");
    lcd_write(0);  // Mostrar '�' personalizada
    lcd_print("o");
    
    lcd_setCursor(4, 1);
    lcd_print("Nuevo 2026!");
    
    // Esperar hasta que se presione '*'
    while (1) {
        char tecla = Keypad_GetKey(&miTeclado);
        if (tecla == '*') {
            break;
        }
        __delay_ms(10);
    }
}

// Opci�n D: Reloj simple
void opcion_d(void) {
    uint8_t minutos = 0;
    uint8_t segundos = 0;
    char tiempo_str[6];
    
    lcd_clear();
    lcd_printCenter("Reloj", 0);
    
    while (1) {
        // Formatear tiempo
        tiempo_str[0] = (minutos / 10) + '0';
        tiempo_str[1] = (minutos % 10) + '0';
        tiempo_str[2] = ':';
        tiempo_str[3] = (segundos / 10) + '0';
        tiempo_str[4] = (segundos % 10) + '0';
        tiempo_str[5] = '\0';
        
        lcd_setCursor(5, 1);
        lcd_print(tiempo_str);
        
        // Esperar 1 segundo
        for (int i = 0; i < 100; i++) {
            __delay_ms(10);
            
            // Verificar si se presion� '*'
            char tecla = Keypad_GetKey(&miTeclado);
            if (tecla == '*') {
                return;
            }
        }
        
        segundos++;
        if (segundos >= 60) {
            segundos = 0;
            minutos++;
            if (minutos >= 60) {
                minutos = 0;
            }
        }
    }
}

void main(void) {
    // Configuraci�n del PIC
    OSCCON = 0x72;  // Internal oscillator setup
    ADCON1 = 0x0F;  // All digital I/O
    CMCON = 0x07;   // Comparators off
    
    // Inicializar LCD
    lcd_begin(16, 2);
    
    // Crear caracteres personalizados
    crear_caracteres_personalizados();
    
    // Inicializar teclado
    Keypad_Init(&miTeclado, teclas);
    
    while (1) {
        mostrar_menu_principal();
        
        // Esperar entrada del usuario
        char tecla = Keypad_WaitForKey(&miTeclado);
        
        switch (tecla) {
            case 'A':
                opcion_a();
                break;
            case 'B':
                opcion_b();
                break;
            case 'C':
                opcion_c();
                break;
            case 'D':
                opcion_d();
                break;
            case '*':
                // Ya estamos en el men� principal
                break;
            default:
                // Otra tecla - mostrar mensaje
                lcd_clear();
                lcd_printCenter("Tecla no valida", 0);
                __delay_ms(1000);
                break;
        }
    }
}