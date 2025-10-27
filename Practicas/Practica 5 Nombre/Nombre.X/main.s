#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

PSECT code
CONF:
    ;PORTA input
    CLRF    LATA, 0 
    MOVLW   0xFF    
    MOVWF   TRISA, 0
    
    ;PORTB Output
    CLRF    LATB, 0    
    MOVLW   0x00       
    MOVWF   TRISB, 0   
    BCF  PORTB,5
    
    ;Pines digitales
    MOVLW   0xF        
    MOVWF   ADCON1, 0  
    
    ;Main Prog
    GOTO LOOP

LOOP:
    MOVF    PORTA, W, a ;Leer Puerto A y 
    ANDLW   0x0F
    CALL    GET_COMBINACION ;Llamado a funcion de lookup
    MOVWF   PORTB, a ;Transferir combinacion a PORTB
    GOTO LOOP

#include"DisplayCode.s"
    
END     resetVec