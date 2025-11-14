#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

PSEC
CONF:
    ;Configuracion de puertos
    MOVLW   0x0F            
    MOVWF   ADCON1          
    
    ; Configuracion de oscilador a 8mhz
    MOVLW   0b01110010      
    MOVWF   OSCCON
    
    ;Programa principal
    GOTO    LOOP

LOOP:
    ;LLamar al delay
    CALL delay_1sec
    ;Alternar LED
    BTG PORTB, 2, A
    ;Bucle
    GOTO    LOOP

    
delay_1sec:
    ;Deshabilitar timer
    BCF     T0CON, 7
    ;Configuracion TCON
    MOVLW   0b10000101  
    MOVWF   T0CON
    ;Cofiguracion del vector de interrupcion
    MOVLW   0x85           
    MOVWF   TMR0H
    MOVLW   0xEE
    MOVWF   TMR0L

    ;Limpiar overflow
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    ;Iniciar timer
    BSF     T0CON, 7        ; TMR0ON bit (bit 7)
    
delay_wait:
    ; Esperar al overflow
    BTFSS   INTCON, 2       ; TMR0IF bit (bit 2)
    GOTO    delay_wait
    ;Limpiar overflow y retornar
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    
    RETURN
   
    
END     resetVec