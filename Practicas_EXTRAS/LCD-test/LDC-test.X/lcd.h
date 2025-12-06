#ifndef LCD_H
#define LCD_H

#include <xc.h>
#include <stdint.h>

// LCD Pin Definitions
#define LCD_RS          LATDbits.LATD0  // Changed to PORTD
#define LCD_EN          LATDbits.LATD1
#define LCD_D4          LATDbits.LATD2
#define LCD_D5          LATDbits.LATD3
#define LCD_D6          LATDbits.LATD4
#define LCD_D7          LATDbits.LATD5

// LCD Commands
#define LCD_CLEAR       0x01
#define LCD_HOME        0x02
#define LCD_ENTRY_MODE  0x06
#define LCD_DISPLAY_OFF 0x08
#define LCD_DISPLAY_ON  0x0C
#define LCD_CURSOR_ON   0x0E
#define LCD_CURSOR_BLINK 0x0F
#define LCD_FUNCTION_4BIT_2LINES 0x28
#define LCD_SET_CGRAM   0x40
#define LCD_SET_DDRAM   0x80

// Function Prototypes
void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);
void LCD_String(const char *str);
void LCD_SetCursor(uint8_t row, uint8_t column);
void LCD_Clear(void);
void LCD_CreateChar(uint8_t location, uint8_t charmap[]);

#endif