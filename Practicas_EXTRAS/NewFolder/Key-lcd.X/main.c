#include "config_bits.h"
#include <xc.h>
#include <stdint.h>
#include "keypad.h"
#include "lcd.h"

#define _XTAL_FREQ 8000000


void Selector(int key){
    switch(key){
        case 'A':
            LCD_Clear();
            LCD_SetCursor(0,0);
            LCD_String("Hola Mundo");
            break;

        case 'B':
            LCD_Clear();
            LCD_SetCursor(0,0);
            LCD_String("Key B ");
            break;

        case 'C':
            LCD_Clear();
            LCD_SetCursor(0,0);
            LCD_String("Key C");
            break;

        case 'D':
            LCD_Clear();
            LCD_SetCursor(0,0);
            LCD_String("Key D");
            break;

        default:
            LCD_Clear();
            LCD_SetCursor(0,0);
            LCD_String("Ni idea");
            break;
    }
}

void main(void) {
    TRISA = 0xFF;               /* Set PORTA as INPUT */
    TRISB = 0x00;               /* Set PORTB as OUTPUT */
    ADCON1 = 0x0F;
    OSCCON = 0x70;             /* Set internal oscillator to 8MHz */
    
    // Initialize LCD on PORTD
    LCD_Init();
    Keypad_Init();
    
    while(1) {
        // Method 1: Get key with debouncing
        char key = Keypad_GetKey();
        
        if(key != 0) {
            Selector(key);
        }
        __delay_ms(100);
    }
}