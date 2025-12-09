#include "keypad.h"

// Keypad definitions
#define ROW1 RB0
#define ROW2 RB1
#define ROW3 RB2
#define ROW4 RB3
#define COL1 RB4
#define COL2 RB5
#define COL3 RB6
#define COL4 RB7

#define _XTAL_FREQ 8000000

// Keypad mapping (customize based on your keypad layout)
const char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Initialize keypad pins
void Keypad_Init(void) {
    // Rows as output (RB0-RB3)
    TRISBbits.TRISB0 = 0;   // ROW1 output
    TRISBbits.TRISB1 = 0;   // ROW2 output
    TRISBbits.TRISB2 = 0;   // ROW3 output
    TRISBbits.TRISB3 = 0;   // ROW4 output
    
    // Columns as input with pull-ups (RB4-RB7)
    TRISBbits.TRISB4 = 1;   // COL1 input
    TRISBbits.TRISB5 = 1;   // COL2 input
    TRISBbits.TRISB6 = 1;   // COL3 input
    TRISBbits.TRISB7 = 1;   // COL4 input
    
}

// Function to get a key with debouncing
// Returns: 0 if no key pressed, ASCII character if key pressed
char Keypad_GetKey(void) {
    char key = Keypad_Scan();
    
    if(key != 0) {
        __delay_ms(20);         // Debounce delay
        
        // Check if key is still pressed after debounce
        if(Keypad_Scan() == key) {
            // Wait for key release
            while(Keypad_Scan() != 0) {
                __delay_ms(10);
            }
            return key;
        }
    }
    
    return 0;                   // No key pressed
}

// Function to scan the keypad once
// Returns: 0 if no key pressed, ASCII character if key pressed
char Keypad_Scan(void) {
    uint8_t row, col;
    
    // Scan each row
    for(row = 0; row < 4; row++) {
        // Set all rows high
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        LATBbits.LATB3 = 1;
        
        // Set current row low
        switch(row) {
            case 0: LATBbits.LATB0 = 0; break;
            case 1: LATBbits.LATB1 = 0; break;
            case 2: LATBbits.LATB2 = 0; break;
            case 3: LATBbits.LATB3 = 0; break;
        }
        
        __delay_us(10);         // Small delay for stabilization
        
        // Check each column
        for(col = 0; col < 4; col++) {
            uint8_t col_state;
            
            // Read current column
            switch(col) {
                case 0: col_state = PORTBbits.RB4; break;
                case 1: col_state = PORTBbits.RB5; break;
                case 2: col_state = PORTBbits.RB6; break;
                case 3: col_state = PORTBbits.RB7; break;
            }
            
            // If column is low (key pressed)
            if(col_state == 0) {
                // Return mapped character
                return keymap[row][col];
            }
        }
    }
    
    return 0;                   // No key pressed
}

// Simple function that returns key if pressed, otherwise returns 0 immediately
char Keypad_CheckKey(void) {
    uint8_t row, col;
    
    // Set all rows low to scan
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    
    __delay_us(10);
    
    // Read all columns at once
    uint8_t cols = PORTB >> 4;
    
    // If no column is low, no key pressed
    if(cols == 0x0F) {
        return 0;
    }
    
    // Determine which key is pressed
    for(row = 0; row < 4; row++) {
        // Set all rows high
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        LATBbits.LATB3 = 1;
        
        // Set current row low
        switch(row) {
            case 0: LATBbits.LATB0 = 0; break;
            case 1: LATBbits.LATB1 = 0; break;
            case 2: LATBbits.LATB2 = 0; break;
            case 3: LATBbits.LATB3 = 0; break;
        }
        
        __delay_us(10);
        
        // Check columns
        for(col = 0; col < 4; col++) {
            if((PORTB >> (4 + col)) & 1 == 0) {
                return keymap[row][col];
            }
        }
    }
    
    return 0;
}