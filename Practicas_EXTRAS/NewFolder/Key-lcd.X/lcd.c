#include "lcd.h"
#include <stdio.h>

#define _XTAL_FREQ 8000000


// Delay functions (unchanged)
void LCD_Delay(uint16_t ms) {
    for(uint16_t i = 0; i < ms; i++) {
        for(uint8_t j = 0; j < 100; j++);
    }
}

void LCD_PulseEnable(void) {
    LCD_EN = 1;
    __delay_us(1);
    LCD_EN = 0;
    __delay_us(100);
}

void LCD_SendNibble(uint8_t nibble) {
    // Set PORTD pins according to nibble
    LCD_D4 = (nibble >> 0) & 0x01;
    LCD_D5 = (nibble >> 1) & 0x01;
    LCD_D6 = (nibble >> 2) & 0x01;
    LCD_D7 = (nibble >> 3) & 0x01;
    LCD_PulseEnable();
}

// Alternative implementation using direct port manipulation (faster):
void LCD_SendNibble_Fast(uint8_t nibble) {
    // Clear only LCD data bits (D4-D7) while preserving other PORTD bits
    LATD &= 0xC3;  // Clear bits 2-5 (1100 0011 in binary)
    
    // Set the nibble to bits 2-5 (shift left by 2)
    LATD |= ((nibble & 0x0F) << 2);
    
    LCD_PulseEnable();
}

void LCD_Command(uint8_t cmd) {
    LCD_RS = 0;  // Command mode
    
    // Send high nibble
    LCD_SendNibble(cmd >> 4);
    
    // Send low nibble
    LCD_SendNibble(cmd & 0x0F);
    
    if(cmd == LCD_CLEAR || cmd == LCD_HOME)
        LCD_Delay(2);
    else
        LCD_Delay(1);
}

void LCD_Data(uint8_t data) {
    LCD_RS = 1;  // Data mode
    
    // Send high nibble
    LCD_SendNibble(data >> 4);
    
    // Send low nibble
    LCD_SendNibble(data & 0x0F);
    
    LCD_Delay(1);
}

void LCD_Init(void) {
    // Configure PORTD pins as outputs
    // Set TRISD bits for LCD pins (0-5) as outputs, others unchanged
    TRISD &= 0xC0;  // Clear bits 0-5 (1100 0000 in binary)
    // Or individually:
    // TRISDbits.TRISD0 = 0;  // RS
    // TRISDbits.TRISD1 = 0;  // EN
    // TRISDbits.TRISD2 = 0;  // D4
    // TRISDbits.TRISD3 = 0;  // D5
    // TRISDbits.TRISD4 = 0;  // D6
    // TRISDbits.TRISD5 = 0;  // D7
    
    // IMPORTANT: On PIC18F4550, PORTD is multiplexed with USB
    // Disable USB module if not using it
    UCONbits.USBEN = 0;  // Disable USB module
    
    LCD_Delay(20);  // Wait for LCD to power up
    
    // Initialization sequence
    LCD_RS = 0;
    LCD_EN = 0;
    
    LCD_SendNibble(0x03);
    LCD_Delay(5);
    LCD_SendNibble(0x03);
    LCD_Delay(1);
    LCD_SendNibble(0x03);
    LCD_Delay(1);
    LCD_SendNibble(0x02);  // Switch to 4-bit mode
    
    // Function set
    LCD_Command(LCD_FUNCTION_4BIT_2LINES);
    
    // Display off
    LCD_Command(LCD_DISPLAY_OFF);
    
    // Clear display
    LCD_Command(LCD_CLEAR);
    
    // Entry mode set
    LCD_Command(LCD_ENTRY_MODE);
    
    // Display on, cursor off
    LCD_Command(LCD_DISPLAY_ON);
    
    LCD_Delay(2);
}

// Other functions remain the same...
void LCD_String(const char *str) {
    while(*str) {
        LCD_Data(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t column) {
    uint8_t address;
    
    if(row == 0)
        address = 0x00 + column;
    else if(row == 1)
        address = 0x40 + column;
    else
        address = 0x00 + column;  // Default to first row
    
    LCD_Command(LCD_SET_DDRAM | address);
}

void LCD_Clear(void) {
    LCD_Command(LCD_CLEAR);
    LCD_Delay(2);
}

void LCD_CreateChar(uint8_t location, uint8_t charmap[]) {
    // Location must be 0-7 for custom characters
    LCD_Command(LCD_SET_CGRAM | (location << 3));
    
    for(uint8_t i = 0; i < 8; i++) {
        LCD_Data(charmap[i]);
    }
    
    // Return to DDRAM
    LCD_Command(LCD_SET_DDRAM);
}