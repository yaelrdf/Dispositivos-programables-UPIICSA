/*****************************************************************************
 * PIC18F4550 - I2C DIAGNOSTIC VERSION
 * 
 * This version helps identify I2C communication problems
 * Shows debug information on LCD and blinks LEDs for feedback
 * 
 * IMPORTANT NOTES:
 * ===============
 * 1. Pull-up resistors (4.7k?) are REQUIRED for I2C!
 *    - Without them, communication may be unreliable
 *    - Internal weak pull-ups are NOT sufficient
 * 
 * 2. If you don't have pull-ups, you can try:
 *    - Enable internal weak pull-ups (less reliable)
 *    - Use 10k? resistors if you have them
 *    - Use any resistor 2.2k? - 10k?
 * 
 * PINOUT:
 * =======
 * I2C (Hardware MSSP):
 *   SDA -> RB0 (pin 33) + 4.7k? pull-up to 5V
 *   SCL -> RB1 (pin 34) + 4.7k? pull-up to 5V
 * 
 * LCD (PORTD):
 *   RS  -> RD0    RW  -> RD1    EN  -> RD2
 *   D4  -> RD4    D5  -> RD5    D6  -> RD6    D7  -> RD7
 * 
 * Indicators:
 *   Buzzer    -> RB4
 *   LED Verde -> RB5
 *   LED Rojo  -> RB6
 *****************************************************************************/

#define _XTAL_FREQ 8000000

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
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

// Debug counters
volatile uint16_t interrupt_count = 0;
volatile uint16_t data_received_count = 0;
volatile uint8_t last_received_byte = 0;
volatile uint8_t error_count = 0;

uint8_t access_granted;
char user_name[17];
char user_uid[9];
char buffer[17];

// ===================== FUNCTION PROTOTYPES =====================
void system_init(void);
void i2c_init(void);
void process_i2c_data(void);
void show_access(uint8_t granted);
void beep(uint8_t times);
void show_status(void);
void blink_leds(uint8_t times);

// ===================== INTERRUPT SERVICE ROUTINE =====================
void __interrupt() ISR(void) {
    // I2C Interrupt
    if (PIR1bits.SSPIF) {
        PIR1bits.SSPIF = 0;
        interrupt_count++;
        
        // Check for overflow or collision
        if (SSPCON1bits.SSPOV) {
            SSPCON1bits.SSPOV = 0;
            uint8_t dummy = SSPBUF;
            error_count++;
            i2c_state = I2C_IDLE;
            i2c_index = 0;
            LED_RED = 1;
            __delay_ms(10);
            LED_RED = 0;
            return;
        }
        
        if (SSPCON1bits.WCOL) {
            SSPCON1bits.WCOL = 0;
            error_count++;
            return;
        }
        
        // Address match (write mode)
        if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            uint8_t dummy = SSPBUF;
            i2c_state = I2C_RECEIVING_CMD;
            i2c_index = 0;
            SSPCON1bits.CKP = 1;
            
            // Blink green quickly to show address received
            LED_GREEN = 1;
            __delay_us(100);
            LED_GREEN = 0;
        }
        // Data received
        else if (SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            uint8_t data = SSPBUF;
            last_received_byte = data;
            data_received_count++;
            
            if (i2c_state == I2C_RECEIVING_CMD) {
                i2c_command = data;
                i2c_state = I2C_RECEIVING_DATA;
                i2c_index = 0;
            } else if (i2c_state == I2C_RECEIVING_DATA) {
                if (i2c_index < 32) {
                    i2c_data[i2c_index++] = data;
                }
            }
            
            SSPCON1bits.CKP = 1;
        }
        // STOP condition
        else if (SSPSTATbits.P) {
            if (i2c_state == I2C_RECEIVING_DATA && i2c_index > 0) {
                i2c_data_ready = 1;
            }
            i2c_state = I2C_IDLE;
            
            // Blink green to show complete transmission
            LED_GREEN = 1;
            __delay_ms(50);
            LED_GREEN = 0;
        }
    }
}

// ===================== MAIN FUNCTION =====================
void main(void) {
    system_init();
    
    __delay_ms(500);
    
    lcd_begin(16, 2);
    
    // Startup screen
    lcd_clear();
    lcd_printCenter("PIC I2C Slave", 0);
    lcd_printCenter("Diagnostic v1.0", 1);
    
    // Blink all LEDs to show we're alive
    blink_leds(3);
    beep(2);
    
    __delay_ms(2000);
    
    // Initialize I2C
    lcd_clear();
    lcd_printCenter("Init I2C...", 0);
    i2c_init();
    lcd_printCenter("I2C Ready", 1);
    beep(1);
    __delay_ms(1500);
    
    // Show I2C address
    lcd_clear();
    lcd_print("I2C Addr: 0x");
    sprintf(buffer, "%02X", I2C_SLAVE_ADDRESS);
    lcd_print(buffer);
    lcd_setCursor(0, 1);
    lcd_print("Waiting...");
    __delay_ms(2000);
    
    // Enable interrupts
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    // Show initial status
    show_status();
    
    uint16_t last_interrupt_count = 0;
    uint16_t last_data_count = 0;
    uint8_t status_update_counter = 0;
    
    // Main loop
    while(1) {
        // Check if new I2C data is ready
        if (i2c_data_ready) {
            INTCONbits.GIE = 0;
            
            // Show that we received data
            LED_GREEN = 1;
            
            lcd_clear();
            lcd_print("Data Received!");
            lcd_setCursor(0, 1);
            sprintf(buffer, "Cmd:0x%02X Len:%d", i2c_command, i2c_index);
            lcd_print(buffer);
            beep(1);
            __delay_ms(1000);
            
            process_i2c_data();
            i2c_data_ready = 0;
            
            LED_GREEN = 0;
            INTCONbits.GIE = 1;
        }
        
        // Update status display every ~1 second
        status_update_counter++;
        if (status_update_counter >= 100) {
            status_update_counter = 0;
            
            // Check if we got new interrupts
            if (interrupt_count != last_interrupt_count || 
                data_received_count != last_data_count) {
                
                // Activity detected - blink LED briefly
                LED_GREEN = 1;
                __delay_ms(10);
                LED_GREEN = 0;
                
                last_interrupt_count = interrupt_count;
                last_data_count = data_received_count;
            }
        }
        
        __delay_ms(10);
    }
}

// ===================== SYSTEM INITIALIZATION =====================
void system_init(void) {
    // Configure internal oscillator (8MHz)
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b00;
    while(!OSCCONbits.IOFS);
    
    // Configure ports
    ADCON1 = 0x0F;
    CMCON = 0x07;
    
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
    // Configure I2C pins
    TRISBbits.TRISB0 = 1;  // SDA as input
    TRISBbits.TRISB1 = 1;  // SCL as input
    
    // Enable internal weak pull-ups (if no external resistors)
    // NOTE: External 4.7k? pull-ups are STRONGLY recommended!
    INTCON2bits.RBPU = 0;   // Enable PORTB pull-ups
    
    // Configure MSSP for I2C Slave mode
    SSPSTAT = 0x80;  // Slew rate disabled, standard mode
    SSPCON1 = 0x36;  // I2C Slave, 7-bit address, enable
    SSPCON2 = 0x01;  // Clock stretching enabled
    
    // Set slave address (left shifted by 1)
    SSPADD = (I2C_SLAVE_ADDRESS << 1);
    
    // Clear and enable I2C interrupt
    PIR1bits.SSPIF = 0;
    PIE1bits.SSPIE = 1;
    
    // Clear state
    i2c_state = I2C_IDLE;
    i2c_data_ready = 0;
    interrupt_count = 0;
    data_received_count = 0;
    error_count = 0;
}

// ===================== PROCESS I2C DATA =====================
void process_i2c_data(void) {
    switch (i2c_command) {
        case CMD_ACCESS_DATA:
            lcd_clear();
            lcd_print("Access Data!");
            lcd_setCursor(0, 1);
            sprintf(buffer, "Bytes: %d", i2c_index);
            lcd_print(buffer);
            __delay_ms(1000);
            
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
            } else {
                lcd_clear();
                lcd_print("Error: Too few");
                lcd_setCursor(0, 1);
                lcd_print("bytes received");
                LED_RED = 1;
                beep(3);
                __delay_ms(2000);
                LED_RED = 0;
                show_status();
            }
            break;
            
        case CMD_STATUS:
            lcd_clear();
            lcd_print("Status Command");
            
            if (i2c_index >= 1) {
                uint8_t status = i2c_data[0];
                lcd_setCursor(0, 1);
                sprintf(buffer, "Status: 0x%02X", status);
                lcd_print(buffer);
                
                if (status == 0xFF) {
                    beep(1);
                    __delay_ms(1500);
                    lcd_clear();
                    lcd_printCenter("Arduino", 0);
                    lcd_printCenter("Connected!", 1);
                    LED_GREEN = 1;
                    __delay_ms(1500);
                    LED_GREEN = 0;
                }
            }
            __delay_ms(1000);
            show_status();
            break;
            
        default:
            lcd_clear();
            lcd_print("Unknown Cmd:");
            lcd_setCursor(0, 1);
            sprintf(buffer, "0x%02X", i2c_command);
            lcd_print(buffer);
            LED_RED = 1;
            beep(2);
            __delay_ms(1500);
            LED_RED = 0;
            show_status();
            break;
    }
}

// ===================== DISPLAY FUNCTIONS =====================
void show_access(uint8_t granted) {
    lcd_clear();
    
    if (granted) {
        lcd_printCenter("Acceso", 0);
        lcd_printCenter("Concedido", 1);
        LED_GREEN = 1;
        LED_RED = 0;
        beep(1);
        __delay_ms(1500);
        
        lcd_clear();
        lcd_printCenter("Bienvenido:", 0);
        lcd_printCenter(user_name, 1);
        __delay_ms(2000);
        
        LED_GREEN = 0;
    } else {
        lcd_printCenter("Acceso", 0);
        lcd_printCenter("Denegado", 1);
        LED_RED = 1;
        LED_GREEN = 0;
        beep(3);
        __delay_ms(1500);
        
        lcd_clear();
        lcd_printCenter("UID:", 0);
        lcd_setCursor(0, 1);
        
        uint8_t uid_len = strlen(user_uid);
        uint8_t start_pos = (16 - uid_len) / 2;
        lcd_setCursor(start_pos, 1);
        lcd_print(user_uid);
        
        __delay_ms(2000);
        LED_RED = 0;
    }
    
    show_status();
}

void show_status(void) {
    lcd_clear();
    
    // Line 1: Show interrupt and data counts
    sprintf(buffer, "Int:%u Dat:%u", interrupt_count, data_received_count);
    lcd_print(buffer);
    
    // Line 2: Show errors and last byte
    lcd_setCursor(0, 1);
    sprintf(buffer, "Err:%u Lst:0x%02X", error_count, last_received_byte);
    lcd_print(buffer);
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

void blink_leds(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        LED_GREEN = 1;
        LED_RED = 1;
        __delay_ms(200);
        LED_GREEN = 0;
        LED_RED = 0;
        __delay_ms(200);
    }
}