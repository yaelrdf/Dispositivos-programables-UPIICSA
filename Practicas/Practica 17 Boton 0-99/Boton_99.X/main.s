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
    CLRF    LATE            ; Clear LATA output latch
    MOVLW   0x08           ; Load W with 0xFF (all inputs)
    MOVWF   TRISE           ; Set TRISA = 0xFF (all bits as inputs)
    
    ; Configure PORTB as output
    CLRF    LATB            ; Clear LATB output latch
    MOVLW   0x00            ; Load W with 0x00 (all outputs)
    MOVWF   TRISB           ; Set TRISB = 0x00 (all bits as outputs)
    
    ;PORTD Output
    CLRF    LATD            ; Clear LATB output latch
    MOVLW   0x00            ; Load W with 0x00 (all outputs)
    MOVWF   TRISD           ; Set TRISB = 0x00 (all bits as outputs)
    
    ; Disable analog functions on PORTA and PORTB
    MOVLW   0x0F            ; Configure all pins as digital (ADCON1 = 0x0F)
    MOVWF   ADCON1          ; Write to ADCON1
    
    ; Oscillator config
    movlw   0x70
    MOVWF   OSCCON
    
    ; Initialize counter to 0
    CLRF    counter
    ; Main Program
    GOTO    LOOP

LOOP:
    ;1rs display
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB           ;Display 
    ;2nd display
    MOVF    counter2, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTD           ;Display 
    ;Button check
    CALL delay_100ms
    GOTO    button
    
Increment:
    INCF    counter, F      ; Increment counter in place
    MOVLW   10              ; Limit value
    CPFSLT  counter         ; Compare: Skip next if counter < 10
    GOTO Dual_counter
    ;CLRF    counter         ; Reset counter to 0 if >= 10
    GOTO LOOP

Dual_counter:
    CLRF counter
    INCF    counter2, F      ; Increment counter in place
    MOVLW   10              ; Limit value
    CPFSLT  counter2         ; Compare: Skip next if counter < 10
    CLRF    counter2         ; Reset counter to 0 if >= 10
    GOTO LOOP
    
    

button:
    BTFSC PORTE, 3, A ; Skip if RE3 is low
    GOTO button
    ;Incrementar
    call Increment  


delay_100ms:
    ; Disable Timer0
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    MOVLW   0b10000101  
    MOVWF   T0CON
    MOVLW   0x3C            ; High byte for 100ms
    MOVWF   TMR0H
    MOVLW   0xB0            ; Low byte for 100ms
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