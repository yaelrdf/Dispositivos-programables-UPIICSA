#include "config_bits.h"
#include <xc.h>
#include <stdint.h>
#include "keypad.h"

#define _XTAL_FREQ 8000000

void main(void) {
    OSCCON = 0x72;              // Configure oscillator for 20MHz
    
    Keypad_Init();              // Initialize keypad (from keypad.c)
    TRISD = 0x00;               // PORTD as output for demonstration
    LATD = 0x00;                // Clear PORTD
    
    while(1) {
        // Method 1: Get key with debouncing
        char key = Keypad_GetKey();
        
        if(key == 0x41) {
            LATD=0xFF;      // If 'A' is pressed, set PORTD high

        }   
        else if(key != 0){
            LATD=key;      // Otherwise, set PORTD low
        }     
        __delay_ms(100);
    }
}