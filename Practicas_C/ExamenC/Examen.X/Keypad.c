/**
 * @file keypad.c
 * @brief Implementation of Arduino-style Keypad Library for PIC18F4550
 */

#include "keypad.h"

#define _XTAL_FREQ 8000000

// Default timing values
static uint16_t holdTime = HOLD_TIME;

// Simple millisecond counter (incremented by GetKey calls)
static uint32_t simple_millis = 0;

// Forward declarations of internal functions
static char scanKeys(Keypad_t* keypad);
static uint32_t getMillis(void);

/**
 * @brief Initialize the keypad
 */
void Keypad_Init(Keypad_t* keypad, char keymap[ROWS][COLS]) {
    // Copy keymap
    for (uint8_t i = 0; i < ROWS; i++) {
        for (uint8_t j = 0; j < COLS; j++) {
            keypad->keys[i][j] = keymap[i][j];
        }
    }
    
    // Set up pin numbers
    keypad->rowPins[0] = 0;  // RB0
    keypad->rowPins[1] = 1;  // RB1
    keypad->rowPins[2] = 2;  // RB2
    keypad->rowPins[3] = 3;  // RB3
    
    keypad->colPins[0] = 4;  // RB4
    keypad->colPins[1] = 5;  // RB5
    keypad->colPins[2] = 6;  // RB6
    keypad->colPins[3] = 7;  // RB7
    
    // Configure PORTB
    // Rows (RB0-RB3) as outputs, initially high
    // Columns (RB4-RB7) as inputs with weak pull-ups
    TRISB = 0xF0;  // 11110000 - RB0-3 output, RB4-7 input
    PORTB = 0xFF;  // All high
    
    // Enable weak pull-ups on PORTB
    INTCON2bits.RBPU = 0;  // Enable pull-ups
    
    // Initialize keypad state
    keypad->currentKey = NO_KEY;
    keypad->lastKey = NO_KEY;
    keypad->state = IDLE;
    keypad->lastUpdateTime = 0;
    keypad->pressTime = 0;
}

/**
 * @brief Simple millisecond counter
 * @note This is a simplified version that doesn't require Timer interrupts
 */
static uint32_t getMillis(void) {
    simple_millis++;
    return simple_millis;
}

/**
 * @brief Scan the keypad matrix
 */
static char scanKeys(Keypad_t* keypad) {
    // Scan each row
    for (uint8_t row = 0; row < ROWS; row++) {
        // Set all rows high
        PORTB |= 0x0F;
        
        // Set current row low
        PORTB &= ~(1 << keypad->rowPins[row]);
        
        // Small delay for signal to stabilize
        __delay_us(10);
        
        // Check each column
        for (uint8_t col = 0; col < COLS; col++) {
            // If column is low, key is pressed
            if (!(PORTB & (1 << keypad->colPins[col]))) {
                // Set all rows high again
                PORTB |= 0x0F;
                return keypad->keys[row][col];
            }
        }
    }
    
    // Set all rows high
    PORTB |= 0x0F;
    return NO_KEY;
}

/**
 * @brief Get a key press
 */
char Keypad_GetKey(Keypad_t* keypad) {
    char key = scanKeys(keypad);
    uint32_t currentTime = getMillis();
    
    // State machine for key handling
    switch (keypad->state) {
        case IDLE:
            if (key != NO_KEY) {
                keypad->currentKey = key;
                keypad->state = PRESSED;
                keypad->pressTime = currentTime;
                keypad->lastUpdateTime = currentTime;
                return key;
            }
            break;
            
        case PRESSED:
            if (key == keypad->currentKey) {
                // Check for hold (simplified - based on scan count)
                if ((currentTime - keypad->pressTime) >= (holdTime / 10)) {
                    keypad->state = HOLD;
                }
            } else if (key == NO_KEY) {
                // Key released
                keypad->state = RELEASED;
                keypad->lastKey = keypad->currentKey;
            }
            break;
            
        case HOLD:
            if (key != keypad->currentKey) {
                keypad->state = RELEASED;
                keypad->lastKey = keypad->currentKey;
            }
            break;
            
        case RELEASED:
            keypad->state = IDLE;
            keypad->currentKey = NO_KEY;
            break;
    }
    
    return NO_KEY;
}

/**
 * @brief Get key state
 */
uint8_t Keypad_GetState(Keypad_t* keypad) {
    Keypad_GetKey(keypad);  // Update state
    return keypad->state;
}

/**
 * @brief Check if specific key is pressed
 */
bool Keypad_IsPressed(Keypad_t* keypad, char key) {
    return (keypad->currentKey == key && 
            (keypad->state == PRESSED || keypad->state == HOLD));
}

/**
 * @brief Wait for a key press (blocking)
 */
char Keypad_WaitForKey(Keypad_t* keypad) {
    char key = NO_KEY;
    while (key == NO_KEY) {
        key = Keypad_GetKey(keypad);
        __delay_ms(10);
    }
    return key;
}

/**
 * @brief Set debounce time
 */
//void Keypad_Set50(Keypad_t* keypad, uint16_t ms) {
//    50 = ms;
//}

/**
 * @brief Set hold time
 */
void Keypad_SetHoldTime(Keypad_t* keypad, uint16_t ms) {
    holdTime = ms;
}

// ========== Additional Utility Functions ==========

/**
 * @brief Get a string from keypad
 */
uint8_t Keypad_GetString(Keypad_t* keypad, char* buffer, uint8_t maxLength, char terminateKey) {
    uint8_t index = 0;
    char key;
    
    while (index < maxLength) {
        key = Keypad_WaitForKey(keypad);
        
        if (key == terminateKey) {
            buffer[index] = '\0';
            return index;
        }
        
        buffer[index++] = key;
        
        // Wait for key release
        while (Keypad_GetKey(keypad) != NO_KEY) {
            __delay_ms(10);
        }
        __delay_ms(50);
    }
    
    buffer[index] = '\0';
    return index;
}

/**
 * @brief Get an integer from keypad
 */
int32_t Keypad_GetNumber(Keypad_t* keypad, char terminateKey) {
    char buffer[12];  // Max for 32-bit int
    uint8_t index = 0;
    int32_t result = 0;
    bool negative = false;
    char key;
    
    while (index < 11) {
        key = Keypad_WaitForKey(keypad);
        
        if (key == terminateKey) {
            break;
        }
        
        // Handle negative sign
        if (key == '-' && index == 0) {
            negative = true;
        } else if (Keypad_IsDigit(key)) {
            buffer[index++] = key;
        }
        
        // Wait for key release
        while (Keypad_GetKey(keypad) != NO_KEY) {
            __delay_ms(10);
        }
        __delay_ms(50);
    }
    
    // Convert to integer
    for (uint8_t i = 0; i < index; i++) {
        result = result * 10 + (buffer[i] - '0');
    }
    
    return negative ? -result : result;
}

/**
 * @brief Get a password
 */
uint8_t Keypad_GetPassword(Keypad_t* keypad, char* buffer, uint8_t maxLength, char terminateKey) {
    uint8_t index = 0;
    char key;
    
    while (index < maxLength) {
        key = Keypad_WaitForKey(keypad);
        
        if (key == terminateKey) {
            buffer[index] = '\0';
            return index;
        }
        
        buffer[index++] = key;
        
        // Wait for key release
        while (Keypad_GetKey(keypad) != NO_KEY) {
            __delay_ms(10);
        }
        __delay_ms(50);
    }
    
    buffer[index] = '\0';
    return index;
}

/**
 * @brief Check if key is a digit
 */
bool Keypad_IsDigit(char key) {
    return (key >= '0' && key <= '9');
}

/**
 * @brief Convert key to integer
 */
int8_t Keypad_KeyToInt(char key) {
    if (Keypad_IsDigit(key)) {
        return (key - '0');
    }
    return -1;
}

/**
 * @brief Flush pending keys
 */
void Keypad_Flush(Keypad_t* keypad) {
    keypad->currentKey = NO_KEY;
    keypad->lastKey = NO_KEY;
    keypad->state = IDLE;
    
    // Wait for all keys to be released
    while (scanKeys(keypad) != NO_KEY) {
        __delay_ms(10);
    }
}