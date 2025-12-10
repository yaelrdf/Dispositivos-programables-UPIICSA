/*
 * LCD Library for PIC18F4550 - XC8 Compiler
 * Implementation file
 */

#include "lcd.h"
#include <string.h>

#define _XTAL_FREQ 8000000

static LCD_Config lcd_config;

// Delay functions (assuming 48MHz clock, adjust for your setup)
void lcd_delay_us(uint16_t us) {
    while(us--) {
        __delay_us(1);
    }
}

void lcd_delay_ms(uint16_t ms) {
    while(ms--) {
        __delay_ms(1);
    }
}

// Pulse the Enable pin
void lcd_pulseEnable(void) {
    LCD_EN_PIN = 0;
    lcd_delay_us(1);
    LCD_EN_PIN = 1;
    lcd_delay_us(1);
    LCD_EN_PIN = 0;
    lcd_delay_us(100);
}

// Write 4 bits to LCD
void lcd_write4bits(uint8_t value) {
    LCD_D4_PIN = (value >> 0) & 0x01;
    LCD_D5_PIN = (value >> 1) & 0x01;
    LCD_D6_PIN = (value >> 2) & 0x01;
    LCD_D7_PIN = (value >> 3) & 0x01;
    lcd_pulseEnable();
}

// Send command to LCD
void lcd_command(uint8_t cmd) {
    LCD_RS_PIN = 0;
    LCD_RW_PIN = 0;
    
    // Send high nibble
    lcd_write4bits(cmd >> 4);
    // Send low nibble
    lcd_write4bits(cmd & 0x0F);
    
    // Some commands need more time
    if(cmd == LCD_CLEAR_DISPLAY || cmd == LCD_RETURN_HOME) {
        lcd_delay_ms(2);
    }
}

// Write data to LCD
void lcd_write(uint8_t value) {
    LCD_RS_PIN = 1;
    LCD_RW_PIN = 0;
    
    // Send high nibble
    lcd_write4bits(value >> 4);
    // Send low nibble
    lcd_write4bits(value & 0x0F);
    
    lcd_config.currCol++;
}

// Initialize LCD
void lcd_begin(uint8_t cols, uint8_t rows) {
    // Configure pins as outputs
    LCD_RS_TRIS = 0;
    LCD_RW_TRIS = 0;
    LCD_EN_TRIS = 0;
    LCD_D4_TRIS = 0;
    LCD_D5_TRIS = 0;
    LCD_D6_TRIS = 0;
    LCD_D7_TRIS = 0;
    
    // Initialize pins to low
    LCD_RS_PIN = 0;
    LCD_RW_PIN = 0;
    LCD_EN_PIN = 0;
    
    lcd_config.numCols = cols;
    lcd_config.numLines = rows;
    lcd_config.currLine = 0;
    lcd_config.currCol = 0;
    
    lcd_config.displayFunction = LCD_4BIT_MODE | LCD_2_LINE | LCD_5x8_DOTS;
    
    // Wait for LCD to power up
    lcd_delay_ms(50);
    
    // Initialization sequence for 4-bit mode
    LCD_RS_PIN = 0;
    LCD_EN_PIN = 0;
    
    // First attempt
    lcd_write4bits(0x03);
    lcd_delay_ms(5);
    
    // Second attempt
    lcd_write4bits(0x03);
    lcd_delay_us(150);
    
    // Third attempt
    lcd_write4bits(0x03);
    lcd_delay_us(150);
    
    // Set to 4-bit mode
    lcd_write4bits(0x02);
    lcd_delay_us(150);
    
    // Function set
    lcd_command(LCD_FUNCTION_SET | lcd_config.displayFunction);
    
    // Display control
    lcd_config.displayControl = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
    lcd_display();
    
    // Clear display
    lcd_clear();
    
    // Entry mode
    lcd_config.displayMode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DEC;
    lcd_command(LCD_ENTRY_MODE_SET | lcd_config.displayMode);
}

// Clear display
void lcd_clear(void) {
    lcd_command(LCD_CLEAR_DISPLAY);
    lcd_config.currLine = 0;
    lcd_config.currCol = 0;
    lcd_delay_ms(2);
}

// Return home
void lcd_home(void) {
    lcd_command(LCD_RETURN_HOME);
    lcd_config.currLine = 0;
    lcd_config.currCol = 0;
    lcd_delay_ms(2);
}

// Set cursor position
void lcd_setCursor(uint8_t col, uint8_t row) {
    static uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    
    if(row >= lcd_config.numLines) {
        row = lcd_config.numLines - 1;
    }
    
    lcd_config.currLine = row;
    lcd_config.currCol = col;
    
    lcd_command(LCD_SET_DDRAM_ADDR | (col + row_offsets[row]));
}

// Turn display off
void lcd_noDisplay(void) {
    lcd_config.displayControl &= ~LCD_DISPLAY_ON;
    lcd_command(LCD_DISPLAY_CONTROL | lcd_config.displayControl);
}

// Turn display on
void lcd_display(void) {
    lcd_config.displayControl |= LCD_DISPLAY_ON;
    lcd_command(LCD_DISPLAY_CONTROL | lcd_config.displayControl);
}

// Turn cursor off
void lcd_noCursor(void) {
    lcd_config.displayControl &= ~LCD_CURSOR_ON;
    lcd_command(LCD_DISPLAY_CONTROL | lcd_config.displayControl);
}

// Turn cursor on
void lcd_cursor(void) {
    lcd_config.displayControl |= LCD_CURSOR_ON;
    lcd_command(LCD_DISPLAY_CONTROL | lcd_config.displayControl);
}

// Turn blink off
void lcd_noBlink(void) {
    lcd_config.displayControl &= ~LCD_BLINK_ON;
    lcd_command(LCD_DISPLAY_CONTROL | lcd_config.displayControl);
}

// Turn blink on
void lcd_blink(void) {
    lcd_config.displayControl |= LCD_BLINK_ON;
    lcd_command(LCD_DISPLAY_CONTROL | lcd_config.displayControl);
}

// Scroll display left
void lcd_scrollDisplayLeft(void) {
    lcd_command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);
}

// Scroll display right
void lcd_scrollDisplayRight(void) {
    lcd_command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);
}

// Left to right text direction
void lcd_leftToRight(void) {
    lcd_config.displayMode |= LCD_ENTRY_LEFT;
    lcd_command(LCD_ENTRY_MODE_SET | lcd_config.displayMode);
}

// Right to left text direction
void lcd_rightToLeft(void) {
    lcd_config.displayMode &= ~LCD_ENTRY_LEFT;
    lcd_command(LCD_ENTRY_MODE_SET | lcd_config.displayMode);
}

// Enable autoscroll
void lcd_autoscroll(void) {
    lcd_config.displayMode |= LCD_ENTRY_SHIFT_INC;
    lcd_command(LCD_ENTRY_MODE_SET | lcd_config.displayMode);
}

// Disable autoscroll
void lcd_noAutoscroll(void) {
    lcd_config.displayMode &= ~LCD_ENTRY_SHIFT_INC;
    lcd_command(LCD_ENTRY_MODE_SET | lcd_config.displayMode);
}

// Create custom character (0-7)
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x07; // Only 8 locations (0-7)
    lcd_command(LCD_SET_CGRAM_ADDR | (location << 3));
    
    for(uint8_t i = 0; i < 8; i++) {
        lcd_write(charmap[i]);
    }
    
    // Return to DDRAM
    lcd_setCursor(lcd_config.currCol, lcd_config.currLine);
}

// Print string
void lcd_print(const char* str) {
    while(*str) {
        lcd_write(*str++);
    }
}

// Print number
void lcd_printNum(int32_t num) {
    char buffer[12];
    uint8_t i = 0;
    uint8_t isNegative = 0;
    
    if(num < 0) {
        isNegative = 1;
        num = -num;
    }
    
    // Convert number to string (reverse order)
    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while(num > 0);
    
    if(isNegative) {
        buffer[i++] = '-';
    }
    
    // Print in correct order
    while(i > 0) {
        lcd_write(buffer[--i]);
    }
}

// Center text on row
void lcd_printCenter(const char* str, uint8_t row) {
    uint8_t len = strlen(str);
    uint8_t pos = 0;
    
    if(len < lcd_config.numCols) {
        pos = (lcd_config.numCols - len) / 2;
    }
    
    lcd_setCursor(pos, row);
    lcd_print(str);
}

// Scroll text horizontally
void lcd_scrollText(const char* str, uint8_t row, uint16_t delay_ms) {
    uint8_t len = strlen(str);
    
    if(len <= lcd_config.numCols) {
        // Text fits, just display it
        lcd_setCursor(0, row);
        lcd_print(str);
        return;
    }
    
    // Scroll text from right to left
    for(int16_t pos = lcd_config.numCols; pos >= -(int16_t)len; pos--) {
        lcd_setCursor(0, row);
        
        // Clear line
        for(uint8_t i = 0; i < lcd_config.numCols; i++) {
            lcd_write(' ');
        }
        
        lcd_setCursor(0, row);
        
        // Print visible portion
        for(uint8_t i = 0; i < lcd_config.numCols; i++) {
            int16_t str_idx = i - pos;
            if(str_idx >= 0 && str_idx < len) {
                lcd_write(str[str_idx]);
            } else {
                lcd_write(' ');
            }
        }
        
        lcd_delay_ms(delay_ms);
    }
}