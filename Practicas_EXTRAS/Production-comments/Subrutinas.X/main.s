#include "configbits.s"
#include <xc.inc>

    PSECT code

;Definiciones de variables
    GLOBAL CON, CONS, CONS1, CONS2
    CON    EQU 0x0D    ; Contador principal
    CONS   EQU 0x0F    ; Conntador para 1ms
    CONS1  EQU 0x10    ; Contador para 1 seg
    CONS2  EQU 0x11    ; Contador para 3 seg

;Programa principal
    ORG 0x020

INICIO:
    ;Inicializacion de variables
    CLRF    CON, a
    CLRF    CONS, a
    CLRF    CONS1, a
    CLRF    CONS2, a
    
    ;Configuracion de puertos
    MOVLW   0x00
    MOVWF   TRISB, a   ; PORTB como output
    CLRF    PORTB, a   ; Limpiar PORTB

MAIN:
    ; Contador a PORTB
    MOVF    CON, W, a
    MOVWF   PORTB, a
    
    ;LLamar al delay de 3seg
    CALL    RET_3S
    
    ; Incrementar contador
    INCF    CON, F, a
    
    ; Bucle 
    GOTO    MAIN

; Subrutina de 3seg
RET_3S:
    MOVLW   3          ;Llamar 3 veces para 3 segudnos
    MOVWF   CONS2, a
OUTER_3S:
    CALL    RET_1S     ; Llamar 3 veces al delay de 1 segundo
    DECFSZ  CONS2, F, a
    GOTO    OUTER_3S
    RETURN

; Subrutina para delay de 1 segundo
RET_1S:
    MOVLW   3          ; 1000ms = 3 * 256 + 232ms
    MOVWF   CONS1, a
OUTER_1S:
    ; LLamar al delay de 1ms 232 veces
    MOVLW   232        ; 232ms
    MOVWF   CONS, a
INNER_1S:
    CALL    RET_1MS
    DECFSZ  CONS, F, a
    GOTO    INNER_1S
    
    DECFSZ  CONS1, F, a
    GOTO    OUTER_1S
    RETURN

; Subrutina de 1ms
RET_1MS:
    MOVLW   198        ;Contar desde 198
    MOVWF   CONS, a
BUCLE:
    NOP
    DECFSZ  CONS, F, a
    GOTO    BUCLE
    RETURN

    END
    