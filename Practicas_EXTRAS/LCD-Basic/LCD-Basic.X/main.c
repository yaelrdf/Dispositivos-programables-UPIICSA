#include <xc.h>
#include "lcd.h"
#include "config_bits.h"

#define _XTAL_FREQ 8000000

// Custom character: Heart symbol
uint8_t heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000
};

// Custom character: Degree symbol
uint8_t degree[8] = {
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

// Custom character: Bell
uint8_t bell[8] = {
    0b00100,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b00000,
    0b00100,
    0b00000
};

void main(void) {
    // Initialize oscillator and ports
    OSCCON = 0x72;  // Internal oscillator setup
    ADCON1 = 0x0F;  // All digital I/O
    CMCON = 0x07;   // Comparators off
    
    // Initialize LCD (16 columns, 2 rows)
    lcd_begin(16, 2);
    
    // Example 1: Simple text display
    lcd_clear();
    lcd_setCursor(0, 0);
    lcd_print("Hello PIC!");
    lcd_setCursor(0, 1);
    lcd_print("LCD Library");
    __delay_ms(2000);
    
    // Example 2: Centered text
    lcd_clear();
    lcd_printCenter("Welcome!", 0);
    lcd_printCenter("PIC18F4550", 1);
    __delay_ms(2000);
    
    // Example 3: Number display
    lcd_clear();
    lcd_setCursor(0, 0);
    lcd_print("Counter: ");
    for(int i = 0; i <= 20; i++) {
        lcd_setCursor(9, 0);
        lcd_printNum(i);
        lcd_print("  "); // Clear extra digits
        __delay_ms(200);
    }
    __delay_ms(1000);
    
    // Example 4: Custom characters
    lcd_createChar(0, heart);
    lcd_createChar(1, degree);
    lcd_createChar(2, bell);
    
    lcd_clear();
    lcd_setCursor(0, 0);
    lcd_print("Custom chars:");
    lcd_setCursor(0, 1);
    lcd_write(0); // Heart
    lcd_print(" ");
    lcd_write(1); // Degree
    lcd_print(" ");
    lcd_write(2); // Bell
    __delay_ms(2000);
    
    // Example 5: Temperature display with degree symbol
    lcd_clear();
    lcd_setCursor(0, 0);
    lcd_print("Temperature:");
    lcd_setCursor(0, 1);
    lcd_printNum(25);
    lcd_write(1); // Degree symbol
    lcd_print("C");
    __delay_ms(2000);
    
    // Example 6: Cursor modes
    lcd_clear();
    lcd_setCursor(0, 0);
    lcd_print("Cursor Test");
    
    lcd_setCursor(0, 1);
    lcd_cursor();
    lcd_print("Cursor ON");
    __delay_ms(2000);
    
    lcd_setCursor(0, 1);
    lcd_blink();
    lcd_print("Blink ON ");
    __delay_ms(2000);
    
    lcd_noCursor();
    lcd_noBlink();
    
    // Example 7: Text scrolling
    lcd_clear();
    lcd_scrollText("This is a long scrolling message for the LCD display!", 0, 300);
    __delay_ms(1000);
    
    // Example 8: Display control
    lcd_clear();
    lcd_printCenter("Display", 0);
    lcd_printCenter("Control Demo", 1);
    __delay_ms(1000);
    
    for(int i = 0; i < 3; i++) {
        lcd_noDisplay();
        __delay_ms(500);
        lcd_display();
        __delay_ms(500);
    }
    
    // Example 9: Scroll display functions
    lcd_clear();
    lcd_printCenter("Scroll Demo", 0);
    __delay_ms(1000);
    
    lcd_setCursor(0, 1);
    lcd_print("LEFT  ");
    for(int i = 0; i < 10; i++) {
        lcd_scrollDisplayLeft();
        __delay_ms(200);
    }
    
    lcd_setCursor(0, 1);
    lcd_print("RIGHT ");
    for(int i = 0; i < 10; i++) {
        lcd_scrollDisplayRight();
        __delay_ms(200);
    }
    
    lcd_home();
    
    // Example 10: Main loop with live data
    int counter = 0;
    while(1) {
        lcd_clear();
        lcd_setCursor(0, 0);
        lcd_print("Runtime: ");
        lcd_printNum(counter++);
        lcd_print("s");
        
        lcd_setCursor(0, 1);
        lcd_write(0); // Heart symbol
        lcd_print(" PIC18F4550 ");
        lcd_write(0); // Heart symbol
        
        __delay_ms(1000);
    }
}