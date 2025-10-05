#include "configbits.s"
#include <xc.inc>

    PSECT resetVec,class=CODE,reloc=2
    PSECT code
    GOTO CONF
 
CONF:
    //I/o Config
    CLRF    ADCON0, a    ; limpiar el adcon 0, para que quede el registro como digital
    MOVLW   0x0F         ; mueve el 15 a W
    MOVWF   ADCON1, a    ; de w al ADCON1, MUEVO EL 15
    MOVLW   00000111B    ; MUEVO EL VALOR DE 7 A W ; CONFIGURA LOS COMPARADORES PARA ENTRAR
    MOVWF   CMCON, A     ; /PARA LOS PINES RA0-RA3
    MOVLW   11111111B    ; MUEVE FF A W
    MOVWF   TRISA, a     ; EL 0 ES EL ACCESS BANK , habilito al puerto a como entrada
    CLRF    PORTA, a     ; 0 EN EL ACCES BANK , limpio
    MOVLW   00000000B    ; habilito al puerto b como salida
    MOVWF   TRISB, 0
    CLRF    PORTB, a     ; limpio
    //Main
    GOTO MAIN
    

 MAIN:
    ; Read PORTA
    MOVF PORTA,w,c          ; W = PORTA
    ; Compare with 0x05
    SUBLW 0x05              ; W = 0x05 - W
    BNZ NOT_EQU          ; Branch if not zero (not equal)
    ; If equal: output 0x0F to PORTB
    MOVLW 0x0F
    MOVF PORTB,c
    GOTO MAIN
    
NOT_EQU:
    ; If not equal: output 0x0A to PORTB
    MOVLW 0x0A
    MOVWF PORTB,c
    GOTO MAIN
    
    END
    


