#include "configbits.s"
#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

PSECT udata_acs
counter:  DS 1 

PSECT code
CONF:        
    ;Configuracion de puertos
    MOVLW   0x0F            ; Configuracion de todos los pines como digitales
    MOVWF   ADCON1        
    
    ;Configuracion de oscilador
    MOVLW   0b01110010      ; IRCF = 111 (8 MHz), SCS = 10
    MOVWF   OSCCON
    
    ;Inicializar contador principal
    CLRF    counter
    ;Programa principal
    GOTO    LOOP

LOOP:
    MOVF    counter, W      ;Contador a w
    ANDLW   0x0F            ;Enmascaramiento de 4bits
    CALL    GET_COMBINACION ;Llamada a la subrutina de manejo de display
    MOVWF   PORTB           ;Mover el valor obtenido a PORB
    ;llamar al delay    
    CALL    delay_1sec
    
    ;Incrementar contador
    INCF    counter, F      ; Increment counter in place
    
    MOVLW   8               ; Valor limite (Partiendo de 1)
    CPFSLT  counter         ; Comparar con limite
    CLRF    counter         ; Reiniciar si el contador supero el limite
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
   
#include "DisplayCode.s"
END     resetVec


#include <xc.inc>
    
PSECT code

;GLOBAL GET_COMBINACION      ;Funcion global

GET_COMBINACION:
    ;Utilizando porgram counter conmutado para el manejo del program counter
    MULLW   2              
    MOVF    PRODL, W, a  
    ;Utilizando solo high byte
    MOVLW   HIGH(TABLE)
    MOVWF   PCLATH, a
    MOVLW   LOW(TABLE)	    
    ADDWF   PRODL, W, a     
    BTFSC   STATUS, 0, a    
    INCF    PCLATH, F, a    
    MOVWF   PCL, a          

    
TABLE:
    RETLW   0xE0    ; 7
    RETLW   0xBE    ; 6
    RETLW   0xB6    ; 5
    RETLW   0x66    ; 4
    RETLW   0xF2    ; 3
    RETLW   0xDA    ; 2
    RETLW   0x60    ; 1
    RETLW   0b11111100    ; 0