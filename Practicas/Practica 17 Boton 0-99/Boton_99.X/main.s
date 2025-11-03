#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF
    
PSECT udata_acs
    counter:  DS 1
    counter2: DS 1
    
PSECT code
CONF:
    ; Configure PORTE as input
    CLRF    LATE            ; Clear LATE output latch
    MOVLW   0x08            ; RE3 as input
    MOVWF   TRISE           ; Set TRISE
    
    ; Configure PORTB as output
    CLRF    LATB            ; Clear LATB output latch
    MOVLW   0x00            ; Load W with 0x00 (all outputs)
    MOVWF   TRISB           ; Set TRISB = 0x00 (all bits as outputs)
    
    ; PORTD Output
    CLRF    LATD            ; Clear LATD output latch
    MOVLW   0x00            ; Load W with 0x00 (all outputs)
    MOVWF   TRISD           ; Set TRISD = 0x00 (all bits as outputs)
    
    ; Disable analog functions on PORTA and PORTB
    MOVLW   0x0F            ; Configure all pins as digital (ADCON1 = 0x0F)
    MOVWF   ADCON1          ; Write to ADCON1
    
    ; Oscillator config
    MOVLW   0x70
    MOVWF   OSCCON
    
    ; Initialize counters to 0
    CLRF    counter
    CLRF    counter2
    
    ; Main Program
    GOTO    LOOP

LOOP:
    ; 1st display
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB           ; Display 
    
    ; 2nd display
    MOVF    counter2, W     ; Counter2 to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTD           ; Display 
    
    ; Button check
    BTFSC   PORTE, 3        ; Skip if RE3 is low (button pressed)
    GOTO    LOOP            ; Button not pressed, loop back
    
    ; Button is pressed - increment counter
    CALL    Increment
    
    ; Debounce delay - wait for button release
    CALL    delay_100ms
wait_release:
    BTFSS   PORTE, 3        ; Skip if RE3 is high (button released)
    GOTO    wait_release    ; Still pressed, keep waiting
    
    ; Additional debounce delay after release
    CALL    delay_100ms
    
    GOTO    LOOP

Increment:
    INCF    counter, F      ; Increment counter in place
    MOVLW   10              ; Limit value
    CPFSLT  counter         ; Compare: Skip next if counter < 10
    GOTO    Dual_counter    ; Counter >= 10, handle overflow
    RETURN                  ; Counter < 10, return

Dual_counter:
    CLRF    counter         ; Reset counter to 0
    INCF    counter2, F     ; Increment counter2 in place
    MOVLW   10              ; Limit value
    CPFSLT  counter2        ; Compare: Skip next if counter2 < 10
    CLRF    counter2        ; Reset counter2 to 0 if >= 10
    RETURN

delay_100ms:
    ; Disable Timer0
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    MOVLW   0b10000111      ; Configure for 1:256 prescaler
    MOVWF   T0CON
    
    ; Load timer values for 100ms delay
    ; Preload = 65536 - 781 = 64755 = 0xFCF3
    MOVLW   0xFC            ; High byte
    MOVWF   TMR0H
    MOVLW   0xF3            ; Low byte
    MOVWF   TMR0L
    
    ; Clear overflow flag
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    
    ; Start Timer0
    BSF     T0CON, 7        ; TMR0ON bit (bit 7)

delay_wait_100ms:
    ; Wait for overflow flag
    BTFSS   INTCON, 2       ; TMR0IF bit (bit 2)
    GOTO    delay_wait_100ms
    
    ; Clear flag and stop timer
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    
    RETURN
        
#include "DisplayCode.s"
    
END     resetVec