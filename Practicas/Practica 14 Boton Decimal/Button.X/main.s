#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

PSECT udata_acs
    counter:  DS 1 

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
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB           ;Display update
    ;Button check
    GOTO    button
    
Increment:
    INCF    counter, F      ; Increment counter in place
    MOVLW   10              ; Limit value
    CPFSLT  counter         ; Compare: Skip next if counter < 10
    CLRF    counter         ; Reset counter to 0 if >= 10
    RETURN
    

button:
    BTFSC PORTE, 3, A ; Skip if RE3 is low
    GOTO button
    ;Incrementar
    call    Increment    

Debounce:
    btfss   PORTE, 3, A ; Skip next if RE3 is high
    goto Debounce
    goto LOOP
        
#include "DisplayCode.s"
    
END     resetVec