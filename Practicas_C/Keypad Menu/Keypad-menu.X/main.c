#include <xc.h>
#include "config_bits.h"
#include "lcd.h"
#include "keypad.h"

#define _XTAL_FREQ 8000000

Keypad_t keypad;
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

int32_t counter1 = 0;
int32_t counter2 = 0;
int32_t counter3 = 0;
uint8_t currentMode = 0; // 0:Counter1, 1:Counter2, 2:Counter3, 3:Phrases

const char* messages[] = {
    "Hello World!",
    "PIC18F4550",
    "LCD & Keypad",
    "Great!",
    "Working!"
};

void displayCurrentState(void);

void main() {
    // Initialize
    ADCON1 = 0x0F;
    lcd_begin(16, 2);
    Keypad_Init(&keypad, teclas);
    
    lcd_clear();
    lcd_printCenter("KEYPAD DEMO", 0);
    lcd_printCenter("READY", 1);
    __delay_ms(2000);
    
    displayCurrentState();
    
    while(1) {
        char key = Keypad_GetKey(&keypad);
        
        if(key != NO_KEY) {
            // Change mode with A, B, C
            if(key == 'A') {
                currentMode = 0;
                displayCurrentState();
            } else if(key == 'B') {
                currentMode = 1;
                displayCurrentState();
            } else if(key == 'C') {
                currentMode = 2;
                displayCurrentState();
            } else if(key == 'D') {
                currentMode = 3;
                displayCurrentState();
            }
            
            // Handle numbers
            else if(key >= '0' && key <= '9') {
                int value = key - '0';
                
                switch(currentMode) {
                    case 0: // Counter 1
                        counter1 += value;
                        break;
                    case 1: // Counter 2
                        counter2 += value;
                        break;
                    case 2: // Counter 3
                        counter3 += value;
                        break;
                }
                displayCurrentState();
            }
            
            // Special keys
            else if(key == '*') {
                // Reset current counter
                switch(currentMode) {
                    case 0: counter1 = 0; break;
                    case 1: counter2 = 0; break;
                    case 2: counter3 = 0; break;
                }
                displayCurrentState();
            } else if(key == '#') {
                // Show summary
                lcd_clear();
                lcd_printCenter("SUMMARY", 0);
                lcd_setCursor(0, 1);
                lcd_print("C1:");
                lcd_printNum(counter1);
                lcd_print(" C2:");
                lcd_printNum(counter2);
                __delay_ms(2000);
                lcd_clear();
                lcd_printCenter("SUMMARY", 0);
                lcd_setCursor(0, 1);
                lcd_print("C3:");
                lcd_printNum(counter3);
                __delay_ms(2000);
                displayCurrentState();
            }
            
            __delay_ms(100);
        }
        
        __delay_ms(10);
    }
}

void displayCurrentState(void) {
    lcd_clear();
    
    switch(currentMode) {
        case 0:
            lcd_printCenter("COUNTER 1", 0);
            lcd_setCursor(0, 1);
            lcd_print("Value: ");
            lcd_printNum(counter1);
            break;
            
        case 1:
            lcd_printCenter("COUNTER 2", 0);
            lcd_setCursor(0, 1);
            lcd_print("Value: ");
            lcd_printNum(counter2);
            break;
            
        case 2:
            lcd_printCenter("COUNTER 3", 0);
            lcd_setCursor(0, 1);
            lcd_print("Value: ");
            lcd_printNum(counter3);
            break;
            
        case 3:
            lcd_printCenter("MESSAGES", 0);
            lcd_setCursor(0, 1);
            lcd_print("Press 0-9");
            __delay_ms(1000);
            static uint8_t msgIndex = 0;
            msgIndex = (msgIndex + 1) % 5;
            lcd_clear();
            lcd_printCenter(messages[msgIndex], 0);
            lcd_printCenter(messages[(msgIndex + 1) % 5], 1);
            __delay_ms(2000);
            currentMode = 0; // Return to counter mode
            displayCurrentState();
            break;
    }
}