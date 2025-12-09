#ifndef KEYPAD_H
#define KEYPAD_H

#include <xc.h>
#include <stdint.h>

// Keypad function prototypes
void Keypad_Init(void);
char Keypad_GetKey(void);
char Keypad_Scan(void);
char Keypad_CheckKey(void);

// Keypad mapping (shared across files)
extern const char keymap[4][4];

#endif