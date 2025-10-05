#include "configbits.s"
#include <xc.inc>

;variable valor maximo
MAX EQU 0x08

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
    MOVWF    PORTA,0 ;Leer Puerto A y 
    ;ANDLW   0x0F                ; Mask lower 4 bits (keep only values 0-15)
    ; Use W as offset to fetch 7-segment pattern from lookup table
    ;MOVWF   WREG                ; Ensure value is in W for comparison
    SUBLW   MAX           ; Subtract W from MAX_VALUE (result in W)
                                ; If W <= MAX_VALUE, result is positive or zero (no borrow, C=1)
                                ; If W > MAX_VALUE, result is negative (borrow occurs, C=0)
    BC VALIDO         ; Branch if Carry set (input <= MAX_VALUE)
    ; If we reach here, input is out of range - skip display update
    GOTO    LOOP                ; Return to loop without updating display

VALIDO:
    MOVWF PORTA, 0  ; Read PORTA value again into W
    ;ANDLW   0x0F                ; Mask lower 4 bits
    ; Use W as offset to fetch 7-segment pattern from lookup table
    CALL    GET_COMBINACION         ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB, 0        ; Write 7-segment pattern to PORTB (update display)
    GOTO LOOP

#include"DisplayCode.s"
END     resetVec