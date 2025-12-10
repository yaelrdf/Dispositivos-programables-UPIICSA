/**
 * @file keypad.h
 * @brief Arduino-style Keypad Library for PIC18F4550 (XC8 Compiler)
 * @details 4x4 Keypad interface connected to PORTB
 * 
 * Pin Configuration (PORTB):
 * - RB0-RB3: Rows (Output)
 * - RB4-RB7: Columns (Input with pull-ups)
 */

#ifndef KEYPAD_H
#define KEYPAD_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

// Keypad dimensions
#define ROWS 4
#define COLS 4

// Key states
#define IDLE       0
#define PRESSED    1
#define HOLD       2
#define RELEASED   3

// Special key values
#define NO_KEY     '\0'

// Timing constants (in milliseconds)
#define DEBOUNCE_TIME  10
#define HOLD_TIME      1000

/**
 * @brief Keypad structure
 */
typedef struct {
    char keys[ROWS][COLS];      // Key map
    uint8_t rowPins[ROWS];      // Row pin numbers (RB0-RB3)
    uint8_t colPins[COLS];      // Column pin numbers (RB4-RB7)
    char currentKey;            // Currently pressed key
    char lastKey;               // Last key that was pressed
    uint8_t state;              // Current key state
    uint32_t lastUpdateTime;    // For debouncing
    uint32_t pressTime;         // Time when key was pressed
} Keypad_t;

/**
 * @brief Initialize the keypad
 * @param keypad Pointer to keypad structure
 * @param keymap 2D array of key characters
 */
void Keypad_Init(Keypad_t* keypad, char keymap[ROWS][COLS]);

/**
 * @brief Get a key press (equivalent to Arduino getKey())
 * @param keypad Pointer to keypad structure
 * @return Character of pressed key or NO_KEY
 */
char Keypad_GetKey(Keypad_t* keypad);

/**
 * @brief Get key state (equivalent to Arduino getState())
 * @param keypad Pointer to keypad structure
 * @return Current key state (IDLE, PRESSED, HOLD, RELEASED)
 */
uint8_t Keypad_GetState(Keypad_t* keypad);

/**
 * @brief Check if key is pressed (equivalent to Arduino isPressed())
 * @param keypad Pointer to keypad structure
 * @param key Character to check
 * @return true if key is pressed
 */
bool Keypad_IsPressed(Keypad_t* keypad, char key);

/**
 * @brief Wait for a key press (blocking)
 * @param keypad Pointer to keypad structure
 * @return Character of pressed key
 */
char Keypad_WaitForKey(Keypad_t* keypad);

/**
 * @brief Set debounce time
 * @param keypad Pointer to keypad structure
 * @param ms Debounce time in milliseconds
 */
void Keypad_SetDebounceTime(Keypad_t* keypad, uint16_t ms);

/**
 * @brief Set hold time
 * @param keypad Pointer to keypad structure
 * @param ms Hold time in milliseconds
 */
void Keypad_SetHoldTime(Keypad_t* keypad, uint16_t ms);

// ========== Additional Utility Functions ==========

/**
 * @brief Get a numeric string from keypad
 * @param keypad Pointer to keypad structure
 * @param buffer Buffer to store the string
 * @param maxLength Maximum length of string
 * @param terminateKey Key to terminate input (e.g., '#')
 * @return Number of characters entered
 */
uint8_t Keypad_GetString(Keypad_t* keypad, char* buffer, uint8_t maxLength, char terminateKey);

/**
 * @brief Get an integer number from keypad
 * @param keypad Pointer to keypad structure
 * @param terminateKey Key to terminate input (e.g., '#')
 * @return Integer value entered
 */
int32_t Keypad_GetNumber(Keypad_t* keypad, char terminateKey);

/**
 * @brief Get a password (masked input)
 * @param keypad Pointer to keypad structure
 * @param buffer Buffer to store the password
 * @param maxLength Maximum length of password
 * @param terminateKey Key to terminate input
 * @return Number of characters entered
 */
uint8_t Keypad_GetPassword(Keypad_t* keypad, char* buffer, uint8_t maxLength, char terminateKey);

/**
 * @brief Check if a key is a digit
 * @param key Character to check
 * @return true if key is '0'-'9'
 */
bool Keypad_IsDigit(char key);

/**
 * @brief Convert key character to integer value
 * @param key Character to convert
 * @return Integer value (0-9) or -1 if not a digit
 */
int8_t Keypad_KeyToInt(char key);

/**
 * @brief Flush any pending key presses
 * @param keypad Pointer to keypad structure
 */
void Keypad_Flush(Keypad_t* keypad);

// ========== Delay Configuration ==========

/**
 * @brief Simple delay using compiler built-in function
 * @param ms Milliseconds to delay
 * @note Uses __delay_ms() from xc.h
 */
#define delay_ms(ms) __delay_ms(ms)

#endif // KEYPAD_H