#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

PSECT code
CONF:
    ; Configure PORTA as input
    CLRF LATA, 0            ; Clear LATA output latch
    SETF TRISA, 0           ; Set TRISA = 0xFF (all bits as inputs)
    
    ; Configure PORTB as output
    CLRF LATB, 0            ; Clear LATB output latch
    CLRF TRISB, 0           ; Set TRISB = 0x00 (all bits as outputs)
    
    ; Disable analog functions on PORTA and PORTB
    CLRF ADCON1, 0          ; Configure all pins as digital
    MOVLW 0x0F              ; Load W with 0x0F
    MOVWF ADCON1, 0         ; Set ADCON1 to make all pins digital (FIXED!)
    ;Main Prog
    GOTO LOOP

LOOP:
    ; Load W with the comparison value
    MOVLW 0xA        ; Target value (0x3F = 63 decimal
    CPFSEQ PORTA, 0         ; Compare PORTA with W
    GOTO NO_IGUAL           ; If NOT equal, go to NO_IGUAL (FIXED!)
    GOTO IGUAL              ; If equal, go to IGUAL (FIXED!)
    
IGUAL:
    ; PORTA = target value, set PORTB to 0x09
    MOVLW 0x09              ; Value to display
    MOVWF LATB, 0           ; Write to PORTB
    GOTO LOOP
    
NO_IGUAL:
    MOVLW 0x01              ; Value to display if not equal
    MOVWF LATB, 0           ; Write to PORTB (FIXED!)
    GOTO LOOP

END resetVec