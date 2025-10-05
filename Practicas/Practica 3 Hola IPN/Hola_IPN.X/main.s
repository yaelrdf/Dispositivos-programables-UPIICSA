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
    
    RETURN
    
 MAIN;
    //Leer de A
    MOVF PORTA, W
    //Comparaciones
    BTFSC 0X01
    GOTO Display_Y
    
    SUBLW 0X02
    GOTO Display_A
    
    SUBLW 0X03
    GOTO DISPLAY_E
    
    SUBLW 0X04
    GOTO DISPLAY_L
    
    SUBLW 0X04
    GOTO DISPLAY_ESPACIO
    
    SUBLW 0X05
    GOTO DISPLAY_S
    
    SUBLW 0X06
    GOTO DISPLAY_A
    
    SUBLW 0X07
    GOTO DISPLAY_L
    
    SUBLW 0X07
    GOTO DISPLAY_D
    
    SUBLW 0X08
    GOTO DISPLAY_A
    
    SUBLW 0X9
    GOTO DISPLAY_N
    
    SUBLW 0X0A
    GOTO DISPLAY_A
    
    
    