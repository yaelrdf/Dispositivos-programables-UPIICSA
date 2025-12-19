#include <xc.h>
#include <string.h>
#include "config_bits.h"
#include "lcd.h"
#include "keypad.h"

// Configuration bits (you should have these in config_bits.h)
// #pragma config statements...

// Define oscillator frequency
#define _XTAL_FREQ 8000000    // 8MHz oscillator

// Global variables
Keypad_t keypad;
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

int32_t counterA = 0;
int32_t counterB = 0;
int32_t counterC = 0;
uint8_t currentMenu = 0;
uint8_t phraseIndex = 0;

const char* phrases[] = {
    "Hello!",
    "Welcome",
    "PIC18F4550",
    "LCD Menu",
    "Keypad Demo",
    "Working!",
    "Good Job!",
    "Finished"
};

// Function prototypes
void initSystem(void);
void displayMainMenu(void);
void handleKeyPress(char key);
void menuCounterA(void);
void menuCounterB(void);
void menuCounterC(void);
void menuPhrases(void);
void menuInfo(void);
void displayCounterScreen(const char* name, int32_t* counter);
void updateDisplay(void);

void main(void) {
    initSystem();
    
    // Welcome message
    lcd_clear();
    lcd_printCenter("SYSTEM READY", 0);
    lcd_printCenter("KEYPAD DEMO", 1);
    __delay_ms(2000);
    
    displayMainMenu();
    
    while(1) {
        char key = Keypad_GetKey(&keypad);
        
        if(key != NO_KEY) {
            handleKeyPress(key);
            __delay_ms(100); // Simple debounce
        }
        
        __delay_ms(10);
    }
}

void initSystem(void) {
    // Configure all pins as digital
    ADCON1 = 0x0F;
    
    // Initialize LCD
    lcd_begin(16, 2);
    
    // Initialize Keypad (using the correct function name from your library)
    Keypad_Init(&keypad, teclas);
    
    // Set contrast by sending some commands
    lcd_setCursor(0, 0);
    lcd_print("Init Complete");
    __delay_ms(500);
    lcd_clear();
}

void displayMainMenu(void) {
    lcd_clear();
    lcd_printCenter("MAIN MENU", 0);
    
    switch(currentMenu) {
        case 0:
            lcd_setCursor(0, 1);
            lcd_print(">Counter A   A<");
            break;
        case 1:
            lcd_setCursor(0, 1);
            lcd_print(">Counter B   B<");
            break;
        case 2:
            lcd_setCursor(0, 1);
            lcd_print(">Counter C   C<");
            break;
        case 3:
            lcd_setCursor(0, 1);
            lcd_print(">Phrases    D<");
            break;
        case 4:
            lcd_setCursor(0, 1);
            lcd_print(">Info       *<");
            break;
    }
}

void handleKeyPress(char key) {
    // Navigation
    if(key == 'A' && currentMenu > 0) {
        currentMenu--;
        displayMainMenu();
        return;
    }
    
    if(key == 'B' && currentMenu < 4) {
        currentMenu++;
        displayMainMenu();
        return;
    }
    
    // Menu selection
    switch(currentMenu) {
        case 0: // Counter A
            if(key == 'C') {
                counterA++;
                lcd_setCursor(13, 1);
                lcd_printNum(counterA);
            } else if(key == 'D') {
                if(counterA > 0) counterA--;
                lcd_setCursor(13, 1);
                lcd_printNum(counterA);
            } else if(key == '*') {
                counterA = 0;
                lcd_setCursor(13, 1);
                lcd_print("0   ");
            }
            break;
            
        case 1: // Counter B
            if(key == 'C') {
                counterB++;
                lcd_setCursor(13, 1);
                lcd_printNum(counterB);
            } else if(key == 'D') {
                if(counterB > 0) counterB--;
                lcd_setCursor(13, 1);
                lcd_printNum(counterB);
            } else if(key == '*') {
                counterB = 0;
                lcd_setCursor(13, 1);
                lcd_print("0   ");
            }
            break;
            
        case 2: // Counter C
            if(key == 'C') {
                counterC++;
                lcd_setCursor(13, 1);
                lcd_printNum(counterC);
            } else if(key == 'D') {
                if(counterC > 0) counterC--;
                lcd_setCursor(13, 1);
                lcd_printNum(counterC);
            } else if(key == '*') {
                counterC = 0;
                lcd_setCursor(13, 1);
                lcd_print("0   ");
            }
            break;
            
        case 3: // Phrases
            if(key == 'C') {
                // Show next phrase
                phraseIndex = (phraseIndex + 1) % 8;
                lcd_clear();
                lcd_printCenter("PHRASE", 0);
                lcd_setCursor(0, 1);
                lcd_print(phrases[phraseIndex]);
                __delay_ms(2000);
                displayMainMenu();
            } else if(key == 'D') {
                // Show previous phrase
                phraseIndex = (phraseIndex == 0) ? 7 : phraseIndex - 1;
                lcd_clear();
                lcd_printCenter("PHRASE", 0);
                lcd_setCursor(0, 1);
                lcd_print(phrases[phraseIndex]);
                __delay_ms(2000);
                displayMainMenu();
            }
            break;
            
        case 4: // Info
            if(key == 'C') {
                // Show all counters
                lcd_clear();
                lcd_printCenter("ALL COUNTERS", 0);
                lcd_setCursor(0, 1);
                lcd_print("A:");
                lcd_printNum(counterA);
                lcd_print(" B:");
                lcd_printNum(counterB);
                __delay_ms(2000);
                
                lcd_clear();
                lcd_printCenter("ALL COUNTERS", 0);
                lcd_setCursor(0, 1);
                lcd_print("C:");
                lcd_printNum(counterC);
                lcd_print(" Ph:");
                lcd_printNum(phraseIndex);
                __delay_ms(2000);
                
                displayMainMenu();
            } else if(key == '*') {
                // Reset everything
                counterA = 0;
                counterB = 0;
                counterC = 0;
                phraseIndex = 0;
                lcd_clear();
                lcd_printCenter("ALL RESET", 0);
                lcd_printCenter("Complete!", 1);
                __delay_ms(1500);
                displayMainMenu();
            }
            break;
    }
    
    // Quick navigation shortcuts
    if(key == '1') {
        currentMenu = 0;
        displayMainMenu();
    } else if(key == '2') {
        currentMenu = 1;
        displayMainMenu();
    } else if(key == '3') {
        currentMenu = 2;
        displayMainMenu();
    } else if(key == '4') {
        currentMenu = 3;
        displayMainMenu();
    } else if(key == '5') {
        currentMenu = 4;
        displayMainMenu();
    }
}