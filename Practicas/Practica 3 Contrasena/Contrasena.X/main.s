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
    MOVLW 0x1E ;Tarjet value
    CPFSEQ PORTA, 0   ;Comparar con PORTA
    GOTO NO_IGUAL     
    GOTO IGUAL  ;Si igual llamar a igual
    
IGUAL:
    MOVLW 0xFF ;Si contrasena
    MOVWF LATB, 0
    GOTO LOOP
    
NO_IGUAL:
    MOVLW 0xC ;No contrasena
    MOVWF LATB, 0
    GOTO LOOP

END resetVec

