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
    MOVLW ADCON1         ; Set ADCON1 to make all pins digital
    ;Main Prog
    GOTO LOOP

LOOP:
    ; Load W with the comparison value 0x0A
    MOVLW 0x0A              ; Target value
    
    ; Compare PORTA with W, skip next instruction if equal
    CPFSEQ PORTA, 0         ; Compare PORTA with W (0x0A)
    GOTO IGUAL ;Si PORTA es igual
    GOTO NO_IGUAL ;Si PORTA no es igual
    
IGUAL:
    ; PORTA = 0x0A, set PORTB to 0xFF
    MOVLW 0x09 ;Valor a mostrar
    MOVWF LATB, 0
    GOTO LOOP
    
NO_IGUAL:
    MOVLW 0x01 ;Valor a mostrar si no igual
    MOVLW LATB
    GOTO LOOP

END resetVec