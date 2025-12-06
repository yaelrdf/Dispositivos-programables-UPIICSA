#include <xc.h>
#include "lcd.h"
#include "config_bits.h"

#define _XTAL_FREQ 8000000

// Custom character
const uint8_t custom_char[8] = {
  0x11,
  0x1B,
  0x15,
  0x11,
  0x11,
  0x0A,
  0x04,
  0x00
};

void main(void) {
    
    TRISA = 0xFF;               /* Set PORTA as INPUT */
    TRISB = 0x00;               /* Set PORTB as OUTPUT */
    ADCON1 = 0x0F;
    OSCCON = 0x70;             /* Set internal oscillator to 8MHz */
    
    // Initialize LCD on PORTD
    LCD_Init();
    
    // Create custom character
    LCD_CreateChar(0, custom_char);
    
    
    while(1) {
      // Display messages
      LCD_SetCursor(0, 0);
      LCD_String("Holiiiss");
      
      __delay_ms(2000);
      //Second line
      LCD_SetCursor(1, 0);
      LCD_String("Sabias que:");
      
      __delay_ms(2000);
      LCD_Clear();
      
      LCD_SetCursor(0,0);
      LCD_String("Te amo");
      LCD_SetCursor(1,0);
      LCD_String("Mucho <3");

      __delay_ms(2000);
      LCD_Clear();
      LCD_String("Eres muy");
      LCD_SetCursor(1,0);
      LCD_String("Importante");
      LCD_SetCursor(1,0);
      __delay_ms(2000);
      LCD_String("                "); 
      LCD_SetCursor(1,0);
      LCD_String("para mi"); 


      __delay_ms(2000);
      LCD_Clear();
      LCD_SetCursor(0,0);
      LCD_String("Me encantan:");
      LCD_SetCursor(1,0);
      LCD_String("Tus abrazos");

      __delay_ms(2000);
      LCD_Clear();
      LCD_SetCursor(0,0);
      LCD_String("Me encantan:");
      LCD_SetCursor(1,0);
      LCD_String("Tus besos");

      __delay_ms(2000);
      LCD_Clear();
      LCD_SetCursor(0,0);
      LCD_String("Me encanta:");
      LCD_SetCursor(1,0);
      LCD_String("Tu sonrisa");
      
      __delay_ms(2000);
      LCD_Clear();
      LCD_SetCursor(0,0);
      LCD_String("Me encanta");
      LCD_SetCursor(1,0);
      LCD_String("Tu persona");

      __delay_ms(2000);
      LCD_Clear();
      LCD_SetCursor(0,0);
      LCD_String("Me encanta");
      LCD_SetCursor(1,0);
      LCD_String("Tu amor");

      __delay_ms(2000);
      LCD_Clear();
      LCD_SetCursor(0,0);
      LCD_String("Bestabe");
      LCD_SetCursor(1,0);
      LCD_String("Te amo muchisimo <3");

      __delay_ms(2000);
      LCD_Clear();
      LCD_SetCursor(0,0);
      LCD_String("No lo olvides");
      LCD_SetCursor(1,0);
      LCD_String(":)");
      __delay_ms(2000);
    }
}