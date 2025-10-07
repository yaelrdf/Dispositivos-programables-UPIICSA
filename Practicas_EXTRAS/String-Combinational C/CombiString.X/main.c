#include"config_bits.h"
#include <xc.h>
#include <string.h>

// Character lookup table based on your CSV data
const unsigned char charTable[39] = {
    0xFE, // 0
    0x60, // 1
    0xDA, // 2
    0xF2, // 3
    0x66, // 4
    0xB6, // 5
    0xBE, // 6
    0xE0, // 7
    0xFE, // 8
    0xE2, // 9
    0xEC, // A
    0x3E, // b
    0x9C, // C
    0x7A, // d
    0x9E, // E
    0x8E, // F
    0xBE, // G
    0x2E, // h
    0x0C, // I
    0x78, // J
    0x2C, // K
    0x1C, // L
    0x9F, // M
    0x2A, // n
    0xAA, // ?
    0xFD, // O
    0xCE, // P
    0xE6, // q
    0x0A, // r
    0xB6, // S
    0x8D, // T
    0x7C, // u
    0x38, // V
    0x00, // W
    0x6E, // X
    0x76, // y
    0xDB, // Z
    0x02  // -
};

// Function to get character code from lookup table
unsigned char getCharCode(char c) {
    if (c >= '0' && c <= '9') {
        return charTable[c - '0'];
    }
    else if (c >= 'A' && c <= 'Z') {
        return charTable[10 + (c - 'A')];
    }
    else if (c >= 'a' && c <= 'z') {
        // Convert lowercase to uppercase
        return charTable[10 + (c - 'a')];
    }
    else {
        switch(c) {
            case '-': return charTable[38];
            case ' ': return 0x00; // Blank display for space
            default: return 0x02; // Default to dash for unknown characters
        }
    }
}

void main(void) {
    // Set up oscillator (8MHz internal)
    OSCCON = 0x72;
    
    // Configure PORTA as digital inputs
    ADCON1 = 0x0F;
    TRISA = 0xFF;   // All PORTA as inputs
    
    // Configure PORTB as digital outputs
    TRISB = 0x00;   // All PORTB as outputs
    LATB = 0x00;    // Initialize PORTB to 0
    
    // Your hardcoded string - change this as needed
    char displayString[] = "Hola";
    unsigned int stringLength = strlen(displayString);
    
    while(1) {
        // Read PORTA to get character index
        unsigned char charIndex = PORTA;
        
        // Ensure index is within string bounds
        if (charIndex >= stringLength) {
            charIndex = stringLength - 1;
        }
        
        // Get the character from string and its display code
        char currentChar = displayString[charIndex];
        unsigned char displayCode = getCharCode(currentChar);
        
        // Output to 7-segment display on PORTB
        LATB = displayCode;
        
        // Small delay for stability (adjust as needed)
        for(unsigned int i = 0; i < 1000; i++);
    }
}