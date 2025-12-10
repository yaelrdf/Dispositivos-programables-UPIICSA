#include <xc.h>
#include "config_bits.h"
#include "lcd.h"
#include "keypad.h"

// Definir frecuencia para delays (requerido por keypad.h)
#define _XTAL_FREQ 8000000

// Configurar OSCCON para 48MHz
#define OSCCON_CONFIG 0x72

// Configuración de password
#define MAX_PASSWORD_LENGTH 8
const char PASSWORD_CORRECTO[] = "123456";

// Declaraciones de funciones
void inicializar_sistema(void);
void mostrar_mensaje_inicial(void);
void solicitar_password(void);
uint8_t ingresar_password(char* buffer);
void mostrar_estrellas(uint8_t longitud);
void limpiar_buffer_password(char* buffer);
uint8_t verificar_password(const char* password_ingresado);

// Variables globales
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad_t miTeclado;

void main(void) {
    // Inicializar sistema
    inicializar_sistema();
    
    // Inicializar LCD y Keypad
    lcd_begin(16, 2);
    Keypad_Init(&miTeclado, teclas);
    
    // Mostrar mensaje de bienvenida
    mostrar_mensaje_inicial();
    
    while(1) {
        // Solicitar password
        solicitar_password();
    }
}

void inicializar_sistema(void) {
    // Configurar OSCCON para 48MHz
    OSCCON = OSCCON_CONFIG;
    
    // Configurar TRIS para LCD (PORTD según documentación)
    TRISD = 0x00;  // Todo PORTD como salidas para LCD
    
    // Configurar TRIS para Keypad (PORTB según documentación)
    // RB0-RB3: Salidas (filas)
    // RB4-RB7: Entradas (columnas) - SIN pull-up interno
    TRISB = 0xF0;  // RB4-RB7 como entradas (1), RB0-RB3 como salidas (0)
    
    // IMPORTANTE: PIC18F4550 no tiene WPUB, usar resistencias pull-up externas
    // Asegúrate de conectar resistencias de 10kΩ de RB4-RB7 a VCC
    
    // Configurar puertos digitales
    ADCON1 = 0x0F;  // Todo digital
}

void mostrar_mensaje_inicial(void) {
    lcd_clear();
    lcd_printCenter("SISTEMA SEGURO", 0);
    lcd_scrollText(:"Seguro que funciona de verdad", 0, 100);
    __delay_ms(500);
}

void solicitar_password(void) {
    char password_ingresado[MAX_PASSWORD_LENGTH + 1];  // Array de tamaño fijo
    uint8_t longitud;
    
    // Limpiar buffer
    limpiar_buffer_password(password_ingresado);
    
    // Mostrar indicaciones
    lcd_clear();
    lcd_printCenter("INGRESE PASSWORD", 0);
    lcd_setCursor(0, 1);
    lcd_print("> ");
    
    // Ingresar password
    longitud = ingresar_password(password_ingresado);
    
    // Verificar password
    if (verificar_password(password_ingresado)) {
        // Password correcto
        lcd_clear();
        lcd_printCenter("ACCESO", 0);
        lcd_printCenter("CONCEDIDO", 1);
        __delay_ms(2000);
        
        // Puedes agregar aquí el código del sistema principal
        
        // Mensaje de salida
        lcd_clear();
        lcd_printCenter("Desbloqueaste", 0);
        lcd_printCenter("NADA", 1)
        __delay_ms(1000);

    } else {
        // Password incorrecto
        lcd_clear();
        lcd_printCenter("ACCESO", 0);
        lcd_printCenter("DENEGADO", 1);
        __delay_ms(2000);
    }
}

uint8_t ingresar_password(char* buffer) {
    uint8_t posicion = 0;
    char tecla;
    
    // Limpiar área de entrada en LCD
    lcd_setCursor(2, 1);
    for(uint8_t i = 0; i < MAX_PASSWORD_LENGTH; i++) {
        lcd_write(' ');
    }
    lcd_setCursor(2, 1);
    
    while(1) {
        // Leer tecla
        tecla = Keypad_GetKey(&miTeclado);
        
        if (tecla != NO_KEY) {
            __delay_ms(150);  // Debounce manual
            
            // Tecla numérica
            if (tecla >= '0' && tecla <= '9') {
                if (posicion < MAX_PASSWORD_LENGTH) {
                    buffer[posicion] = tecla;
                    posicion++;
                    
                    // Mostrar asteriscos
                    lcd_setCursor(2, 1);
                    mostrar_estrellas(posicion);
                    
                    // Mostrar espacio para siguiente dígito
                    if (posicion < MAX_PASSWORD_LENGTH) {
                        lcd_setCursor(2 + posicion, 1);
                        lcd_write('_');  // Cursor visual
                    }
                }
            }
            // Tecla A (Aceptar/Enviar)
            else if (tecla == 'A') {
                if (posicion > 0) {  // Asegurar que se ingresó algo
                    buffer[posicion] = '\0';  // Terminar string
                    __delay_ms(300);
                    return posicion;
                }
            }
            // Tecla B (Borrar/Retroceso)
            else if (tecla == 'B') {
                if (posicion > 0) {
                    posicion--;
                    buffer[posicion] = '\0';
                    
                    // Actualizar display
                    lcd_setCursor(2, 1);
                    mostrar_estrellas(posicion);
                    
                    // Mostrar espacio en la posición actual
                    lcd_setCursor(2 + posicion, 1);
                    lcd_write('_');
                    
                    // Borrar caracter extra si existe
                    if (posicion < MAX_PASSWORD_LENGTH - 1) {
                        lcd_setCursor(2 + posicion + 1, 1);
                        lcd_write(' ');
                    }
                    
                    lcd_setCursor(2 + posicion, 1);
                }
            }
            // Tecla C (Cancelar todo)
            else if (tecla == 'C') {
                limpiar_buffer_password(buffer);
                posicion = 0;
                
                // Limpiar display
                lcd_setCursor(2, 1);
                for(uint8_t i = 0; i < MAX_PASSWORD_LENGTH; i++) {
                    lcd_write(' ');
                }
                lcd_setCursor(2, 1);
                lcd_write('_');  // Cursor inicial
            }
            
            __delay_ms(100);  // Pequeña pausa entre teclas
        }
        
        __delay_ms(10);  // Pausa para evitar uso excesivo de CPU
    }
}

void mostrar_estrellas(uint8_t longitud) {
    for(uint8_t i = 0; i < longitud; i++) {
        lcd_write('*');
    }
}

void limpiar_buffer_password(char* buffer) {
    for(uint8_t i = 0; i < MAX_PASSWORD_LENGTH + 1; i++) {
        buffer[i] = '\0';
    }
}

uint8_t verificar_password(const char* password_ingresado) {
    uint8_t i = 0;
    
    // Comparar carácter por carácter
    while(i < MAX_PASSWORD_LENGTH) {
        if(password_ingresado[i] != PASSWORD_CORRECTO[i]) {
            return 0;  // Password incorrecto
        }
        
        // Si llegamos al final de ambos strings, son iguales
        if(password_ingresado[i] == '\0' && PASSWORD_CORRECTO[i] == '\0') {
            return 1;  // Password correcto
        }
        
        i++;
    }
    
    // Si llegamos aquí sin encontrar diferencias y ambos tienen la misma longitud
    return (password_ingresado[i] == '\0' && PASSWORD_CORRECTO[i] == '\0');
}