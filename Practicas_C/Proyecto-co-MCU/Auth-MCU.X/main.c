/*****************************************************************************
 * PIC18F4550 - I2C SLAVE DISPLAY CONTROLLER
 * 
 * Recibe datos del Arduino UNO vía I2C y controla:
 * - LCD 16x2 (muestra información de acceso)
 * - LED Verde (acceso concedido)
 * - LED Rojo (acceso denegado)
 * - Buzzer (indicación sonora)
 * 
 * PINOUT:
 * =======
 * LCD (PORTD):
 *   RS  -> RD0    RW  -> RD1    EN  -> RD2
 *   D4  -> RD4    D5  -> RD5    D6  -> RD6    D7  -> RD7
 * 
 * I2C (Hardware MSSP):
 *   SDA -> RB0 (SDA)
 *   SCL -> RB1 (SCL)
 *   Requiere pull-ups de 4.7kΩ en ambas líneas!
 * 
 * Indicadores:
 *   Buzzer    -> RB4
 *   LED Verde -> RB5
 *   LED Rojo  -> RB6
 * 
 * PROTOCOLO I2C:
 * ==============
 * Dirección I2C: 0x20
 * 
 * Comando 0xAA (Access Data):
 *   Byte 0: 0xAA (comando)
 *   Byte 1: granted (1=acceso, 0=denegado)
 *   Bytes 2-9: UID (8 bytes, string)
 *   Bytes 10-25: Name (16 bytes, string)
 * 
 * Comando 0xF0 (Status):
 *   Byte 0: 0xF0 (comando)
 *   Byte 1: status code
 *****************************************************************************/

#define _XTAL_FREQ 8000000  // 8MHz interno

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include "config_bits.h"
#include "lcd.h"

// ===================== I2C CONFIGURATION =====================
#define I2C_SLAVE_ADDRESS 0x20

// ===================== PIN DEFINITIONS =====================
#define BUZZER_TRIS     TRISBbits.TRISB4
#define BUZZER          LATBbits.LATB4
#define LED_GREEN_TRIS  TRISBbits.TRISB5
#define LED_GREEN       LATBbits.LATB5
#define LED_RED_TRIS    TRISBbits.TRISB6
#define LED_RED         LATBbits.LATB6

// ===================== I2C STATES =====================
#define I2C_IDLE            0
#define I2C_RECEIVING_CMD   1
#define I2C_RECEIVING_DATA  2

// ===================== COMMANDS =====================
#define CMD_ACCESS_DATA     0xAA
#define CMD_STATUS          0xF0

// ===================== GLOBAL VARIABLES =====================
volatile uint8_t i2c_state = I2C_IDLE;
volatile uint8_t i2c_command = 0;
volatile uint8_t i2c_data[32];
volatile uint8_t i2c_index = 0;
volatile uint8_t i2c_data_ready = 0;

uint8_t access_granted;
char user_name[17];
char user_uid[9];

// ===================== FUNCTION PROTOTYPES =====================
void system_init(void);
void i2c_init(void);
void process_i2c_data(void);
void show_access(uint8_t granted);
void beep(uint8_t times);
void show_idle_screen(void);

// ===================== INTERRUPT SERVICE ROUTINE =====================
void __interrupt() ISR(void) {
    // I2C Interrupt
    if (PIR1bits.SSPIF) {
        PIR1bits.SSPIF = 0;  // Clear flag
        
        // Check for I2C errors
        if ((SSPCON1bits.SSPOV) || (SSPCON1bits.WCOL)) {
            // Clear overflow/collision
            SSPCON1bits.SSPOV = 0;
            SSPCON1bits.WCOL = 0;
            uint8_t dummy = SSPBUF;  // Read to clear
            i2c_state = I2C_IDLE;
            i2c_index = 0;
            return;
        }
        
        // Check status
        uint8_t status = SSPSTAT & 0x2D;  // Mask relevant bits
        
        // Address matched (R/W = 0, D/A = 0)
        if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            uint8_t dummy = SSPBUF;  // Read address to clear BF
            i2c_state = I2C_RECEIVING_CMD;
            i2c_index = 0;
            SSPCON1bits.CKP = 1;  // Release clock
        }
        // Data received (R/W = 0, D/A = 1)
        else if (SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            uint8_t data = SSPBUF;
            
            if (i2c_state == I2C_RECEIVING_CMD) {
                i2c_command = data;
                i2c_state = I2C_RECEIVING_DATA;
                i2c_index = 0;
            } else if (i2c_state == I2C_RECEIVING_DATA) {
                if (i2c_index < 32) {
                    i2c_data[i2c_index++] = data;
                }
            }
            
            SSPCON1bits.CKP = 1;  // Release clock
        }
        // STOP condition detected
        else if (SSPSTATbits.P) {
            if (i2c_state == I2C_RECEIVING_DATA) {
                i2c_data_ready = 1;  // Signal main loop
            }
            i2c_state = I2C_IDLE;
        }
        // Start condition
        else if (SSPSTATbits.S) {
            // New transmission starting
        }
    }
}

// ===================== MAIN FUNCTION =====================
void main(void) {
    system_init();
    lcd_begin(16, 2);
    i2c_init();
    
    __delay_ms(500);
    
    // Startup screen
    lcd_clear();
    lcd_printCenter("Sistema RFID", 0);
    lcd_printCenter("PIC Slave v1.0", 1);
    beep(2);
    __delay_ms(2000);
    
    // Show idle screen
    show_idle_screen();
    
    // Enable interrupts
    INTCONbits.PEIE = 1;  // Peripheral interrupts
    INTCONbits.GIE = 1;   // Global interrupts
    
    // Main loop
    while(1) {
        // Check if new I2C data is ready
        if (i2c_data_ready) {
            INTCONbits.GIE = 0;  // Disable interrupts temporarily
            process_i2c_data();
            i2c_data_ready = 0;
            INTCONbits.GIE = 1;  // Re-enable interrupts
        }
        
        __delay_ms(10);
    }
}

// ===================== SYSTEM INITIALIZATION =====================
void system_init(void) {
    // Configure internal oscillator (8MHz)
    OSCCONbits.IRCF = 0b111;  // 8MHz
    OSCCONbits.SCS = 0b00;
    while(!OSCCONbits.IOFS);
    
    // Configure ports
    ADCON1 = 0x0F;  // All digital
    CMCON = 0x07;   // Comparators off
    
    // Configure indicator pins
    BUZZER_TRIS = 0;
    LED_GREEN_TRIS = 0;
    LED_RED_TRIS = 0;
    
    // Initial states
    BUZZER = 0;
    LED_GREEN = 0;
    LED_RED = 0;
}

// ===================== I2C INITIALIZATION =====================
void i2c_init(void) {
    // Configure I2C pins (RB0=SDA, RB1=SCL)
    TRISBbits.TRISB0 = 1;  // SDA as input (will be controlled by MSSP)
    TRISBbits.TRISB1 = 1;  // SCL as input
    
    // Configure MSSP module for I2C Slave mode
    SSPSTAT = 0x80;  // Slew rate control disabled for Standard mode
    
    SSPCON1 = 0x36;  // I2C Slave mode, 7-bit address, enable MSSP
    SSPCON2 = 0x01;  // Clock stretching enabled
    
    // Set slave address
    SSPADD = (I2C_SLAVE_ADDRESS << 1);  // Left shift for 7-bit address
    
    // Enable I2C interrupt
    PIR1bits.SSPIF = 0;   // Clear flag
    PIE1bits.SSPIE = 1;   // Enable interrupt
    
    // Clear state
    i2c_state = I2C_IDLE;
    i2c_data_ready = 0;
}

// ===================== PROCESS I2C DATA =====================
void process_i2c_data(void) {
    switch (i2c_command) {
        case CMD_ACCESS_DATA:
            // Parse access data
            // Byte 0: granted flag
            // Bytes 1-8: UID
            // Bytes 9-24: Name
            
            if (i2c_index >= 25) {
                access_granted = i2c_data[0];
                
                // Copy UID
                memset(user_uid, 0, sizeof(user_uid));
                for (uint8_t i = 0; i < 8 && i < i2c_index - 1; i++) {
                    user_uid[i] = i2c_data[i + 1];
                }
                
                // Copy Name
                memset(user_name, 0, sizeof(user_name));
                for (uint8_t i = 0; i < 16 && (i + 9) < i2c_index; i++) {
                    user_name[i] = i2c_data[i + 9];
                }
                
                // Show access result
                show_access(access_granted);
            }
            break;
            
        case CMD_STATUS:
            // Status command received
            if (i2c_index >= 1) {
                uint8_t status = i2c_data[0];
                
                if (status == 0xFF) {
                    // Arduino ready signal
                    lcd_clear();
                    lcd_printCenter("Arduino", 0);
                    lcd_printCenter("Conectado", 1);
                    beep(1);
                    __delay_ms(1500);
                    show_idle_screen();
                }
            }
            break;
            
        default:
            // Unknown command
            break;
    }
}

// ===================== DISPLAY FUNCTIONS =====================
void show_access(uint8_t granted) {
    lcd_clear();
    
    if (granted) {
        // Access granted
        lcd_printCenter("Acceso", 0);
        lcd_printCenter("Concedido", 1);
        LED_GREEN = 1;
        LED_RED = 0;
        beep(1);
        __delay_ms(1500);
        
        // Show user name
        lcd_clear();
        lcd_printCenter("Bienvenido:", 0);
        lcd_printCenter(user_name, 1);
        __delay_ms(2000);
        
        LED_GREEN = 0;
    } else {
        // Access denied
        lcd_printCenter("Acceso", 0);
        lcd_printCenter("Denegado", 1);
        LED_RED = 1;
        LED_GREEN = 0;
        beep(3);
        __delay_ms(1500);
        
        // Show UID
        lcd_clear();
        lcd_printCenter("UID:", 0);
        lcd_setCursor(0, 1);
        
        // Center the UID
        uint8_t uid_len = strlen(user_uid);
        uint8_t start_pos = (16 - uid_len) / 2;
        lcd_setCursor(start_pos, 1);
        lcd_print(user_uid);
        
        __delay_ms(2000);
        LED_RED = 0;
    }
    
    // Return to idle screen
    show_idle_screen();
}

void show_idle_screen(void) {
    lcd_clear();
    lcd_printCenter("Control de", 0);
    lcd_printCenter("Acceso RFID", 1);
}

// ===================== UTILITY FUNCTIONS =====================
void beep(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        BUZZER = 1;
        __delay_ms(100);
        BUZZER = 0;
        if (i < times - 1) {
            __delay_ms(100);
        }
    }
}