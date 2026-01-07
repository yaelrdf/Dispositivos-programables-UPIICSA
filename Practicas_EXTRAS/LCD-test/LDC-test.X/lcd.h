/*
 * LCD Library for PIC18F4550 - XC8 Compiler
 * 16x2 LCD in 4-bit mode
 * Arduino LiquidCrystal compatible API
 */

#ifndef LCD_H
#define LCD_H

#include <xc.h>
#include <stdint.h>

// LCD Pin Configuration - Modify as needed
#define LCD_RS_PIN      PORTDbits.RD0
#define LCD_RW_PIN      PORTDbits.RD1
#define LCD_EN_PIN      PORTDbits.RD2
#define LCD_D4_PIN      PORTDbits.RD4
#define LCD_D5_PIN      PORTDbits.RD5
#define LCD_D6_PIN      PORTDbits.RD6
#define LCD_D7_PIN      PORTDbits.RD7

#define LCD_RS_TRIS     TRISDbits.TRISD0
#define LCD_RW_TRIS     TRISDbits.TRISD1
#define LCD_EN_TRIS     TRISDbits.TRISD2
#define LCD_D4_TRIS     TRISDbits.TRISD4
#define LCD_D5_TRIS     TRISDbits.TRISD5
#define LCD_D6_TRIS     TRISDbits.TRISD6
#define LCD_D7_TRIS     TRISDbits.TRISD7

// LCD Commands
#define LCD_CLEAR_DISPLAY       0x01
#define LCD_RETURN_HOME         0x02
#define LCD_ENTRY_MODE_SET      0x04
#define LCD_DISPLAY_CONTROL     0x08
#define LCD_CURSOR_SHIFT        0x10
#define LCD_FUNCTION_SET        0x20
#define LCD_SET_CGRAM_ADDR      0x40
#define LCD_SET_DDRAM_ADDR      0x80

// Entry Mode flags
#define LCD_ENTRY_RIGHT         0x00
#define LCD_ENTRY_LEFT          0x02
#define LCD_ENTRY_SHIFT_INC     0x01
#define LCD_ENTRY_SHIFT_DEC     0x00

// Display Control flags
#define LCD_DISPLAY_ON          0x04
#define LCD_DISPLAY_OFF         0x00
#define LCD_CURSOR_ON           0x02
#define LCD_CURSOR_OFF          0x00
#define LCD_BLINK_ON            0x01
#define LCD_BLINK_OFF           0x00

// Cursor/Display Shift flags
#define LCD_DISPLAY_MOVE        0x08
#define LCD_CURSOR_MOVE         0x00
#define LCD_MOVE_RIGHT          0x04
#define LCD_MOVE_LEFT           0x00

// Function Set flags
#define LCD_8BIT_MODE           0x10
#define LCD_4BIT_MODE           0x00
#define LCD_2_LINE              0x08
#define LCD_1_LINE              0x00
#define LCD_5x10_DOTS           0x04
#define LCD_5x8_DOTS            0x00

// LCD dimensions
#define LCD_COLS                16
#define LCD_ROWS                2

// LCD Structure
typedef struct {
    uint8_t displayFunction;
    uint8_t displayControl;
    uint8_t displayMode;
    uint8_t numLines;
    uint8_t numCols;
    uint8_t currLine;
    uint8_t currCol;
} LCD_Config;

// Function prototypes
void lcd_begin(uint8_t cols, uint8_t rows);
void lcd_clear(void);
void lcd_home(void);
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_noDisplay(void);
void lcd_display(void);
void lcd_noCursor(void);
void lcd_cursor(void);
void lcd_noBlink(void);
void lcd_blink(void);
void lcd_scrollDisplayLeft(void);
void lcd_scrollDisplayRight(void);
void lcd_leftToRight(void);
void lcd_rightToLeft(void);
void lcd_autoscroll(void);
void lcd_noAutoscroll(void);
void lcd_createChar(uint8_t location, uint8_t charmap[]);
void lcd_write(uint8_t value);
void lcd_print(const char* str);
void lcd_printNum(int32_t num);
void lcd_printCenter(const char* str, uint8_t row);
void lcd_scrollText(const char* str, uint8_t row, uint16_t delay_ms);

// Low-level functions
void lcd_command(uint8_t cmd);
void lcd_write4bits(uint8_t value);
void lcd_pulseEnable(void);
void lcd_delay_us(uint16_t us);
void lcd_delay_ms(uint16_t ms);

#endif // LCD_H