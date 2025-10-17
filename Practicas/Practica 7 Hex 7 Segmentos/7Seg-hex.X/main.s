#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

PSECT code
CONF:
    ; Configure PORTA as input
    CLRF    LATA, 0            ; Clear LATA output latch
    MOVLW   0xFF        ; Load W with 0xFF (all inputs)
    MOVWF   TRISA, 0           ; Set TRISA = 0xFF (all bits as inputs)
    
    ; Configure PORTB as output
    CLRF    LATB, 0            ; Clear LATB output latch
    MOVLW   0x00        ; Load W with 0x00 (all outputs)
    MOVWF   TRISB, 0           ; Set TRISB = 0x00 (all bits as outputs)\
    BCF  PORTB,5
    
    ; Disable analog functions on PORTA and PORTB
    MOVLW   0xF        ; Configure all pins as digital (ADCON1 = 0x0F)
    MOVWF   ADCON1, 0          ; Write to ADCON1
    
    ;Main Prog
    GOTO LOOP

LOOP:
    MOVF    PORTA, W, a ;Leer Puerto A y 
    ANDLW   0x0F                ; Mask lower 4 bita
    CALL    GET_COMBINACION         ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB, a        ; Write 7-segment pattern to PORTB (update display)
    GOTO LOOP

#include"DisplayCode.s"
    
END     resetVec