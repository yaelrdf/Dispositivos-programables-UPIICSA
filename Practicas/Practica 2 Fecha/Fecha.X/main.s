#include "configbits.s"
#include "DisplayCode.s"
#include <xc.inc>
    
; Define variables in data memory    
VARIABLES:
    PSECT udata_acs
datoA:  DS 1    ; Reserve 1 byte for datoA
datoB:  DS 1    ; Reserve 1 byte for datoB (though not used in your code)

    PSECT code

ORG Ox0000
GOTO CONFIG
 
CONFIG:
    //Ports Config
    CLRF PORTA
    CLRF TRISA
    CLRF PORTB
    CLRF TRISB
    //I/o Config
    MOVLW OXFF
    MOVWF TRISA
    MOVLW OX00
    MOVWF TRISB
    GOTO MAIN

MAIN:
    


