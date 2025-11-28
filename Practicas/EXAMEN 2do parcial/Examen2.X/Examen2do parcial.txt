#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

; Variables
PSECT udata_acs
    DatoA:     DS 1
    DatoB:     DS 1
    Resultado: DS 1

PSECT code
CONF:
    ;Configuracion de puertos
    ;RA Input
    CLRF    LATA           
    MOVLW   0x10
    MOVWF   TRISA           

    ;RB Input
    CLRF    LATB
    MOVLW   0xFF
    MOVWF   TRISB           

    ;RD Output
    CLRF    LATD
    MOVLW   0x00
    MOVWF   TRISD         

    ;Deshabilitar operaciones analogicas
    MOVLW   0x0F
    MOVWF   ADCON1

    GOTO    LOOP

LOOP:
    ;Leer PORTA
    MOVF    PORTB, W
    ANDLW   0xF0    ;Enmascaramiento HIGH
    MOVWF   DatoA   
    SWAPF   DatoA, F  ; Ni idea, la use en la practica de la contrasena y asi funciono

    ;Enmascaramiento low
    MOVF    PORTB, W
    ANDLW   0x0F    ;Enmascaramiento low
    MOVWF   DatoB

    ;Decidir suma o resta
    BTFSC   PORTA, 4
    GOTO    or_oper
    GOTO    suma_oper

or_oper:
    MOVF    DatoA, W
    IORWF   DatoB, W        ; W = DatoA | DatoB
    ANDLW   0x0F
    MOVWF   LATD            ;Mover resultado a PORTD
    GOTO    LOOP

suma_oper:
    MOVF    DatoA, W
    ADDWF   DatoB, W        ; Enmascaranmienro con suma
    ANDLW   0x0F            ; Enmascaramiento para 4 bits de suma
    MOVWF   Resultado
    SWAPF   Resultado, W    ; Enmascarar pero al revez, para invertir
    MOVWF   LATD            ; Mover suma al puerto
    GOTO    LOOP

END resetVec
