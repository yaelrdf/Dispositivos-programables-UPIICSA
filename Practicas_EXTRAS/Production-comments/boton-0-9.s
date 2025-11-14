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
    ;Configuracion del puerto E como entrada
    CLRF    LATE          
    MOVLW   0x08            
    MOVWF   TRISE           
    
    ;Configuracion del puerto B como salida
    CLRF    LATB           
    MOVLW   0x00           
    MOVWF   TRISB          
    
    ;Deshabilitar funciones analogicas
    MOVLW   0x0F            
    MOVWF   ADCON1          
    
    ;Configuracion del osilador a 8mhz
    MOVLW   0x70
    MOVWF   OSCCON
    
    ;Inicializacion del contador principal
    CLRF    counter
    
    ;Programa principal
    GOTO    LOOP

LOOP:
    MOVF    counter, W      ; Contador a w
    ANDLW   0x0F            ; Enmascaramiento de 4 bitys
    CALL    GET_COMBINACION ; Llamada a la funcion de combinacion de display
    MOVWF   PORTB           ; Actualizar display
    
    ;Manejo de boton
    BTFSC   PORTE, 3        ; Saltar si el boton esta en una posicion LOW
    GOTO    LOOP            ; Si el boton esta en una posicion HIGH seguir con bucle
    
    ;Incrementar el contador
    CALL    Incremento
    
    ;Llamar al delay de 100ms
    CALL    delay_100ms

esperar_release:
    BTFSS   PORTE, 3        ;Saltar si el boton esta en posicion LOW
    GOTO    esperar_release    ; Si el boton continua pulsado esperar
    
    ;Debounce delay
    CALL    delay_100ms
    
    GOTO    LOOP

Incremento:
    INCF    counter, F      ; Incrementar el contador
    MOVLW   10              ; Valor limite
    CPFSLT  counter         ; Comparar y saltar si no es mayor al limite
    CLRF    counter         ; Reset
    RETURN

delay_100ms:
    ; Disable Timer0
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    MOVLW   0b10000111      ; Configurar prescaler
    MOVWF   T0CON
    
    ; Preload = 65536 - 781 = 64755 = 0xFCF3
    MOVLW   0xFC            ; High byte
    MOVWF   TMR0H
    MOVLW   0xF3            ; Low byte
    MOVWF   TMR0L
    
    ;Limpiar overflow
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    
    ; Iniciar timer
    BSF     T0CON, 7        ; TMR0ON bit (bit 7)

delay_wait_100ms:
    ;Esperar al overflow
    BTFSS   INTCON, 2       ; TMR0IF bit (bit 2)
    GOTO    delay_wait_100ms
    
    ;Limpiar overflow y retornar
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    
    RETURN
        
#include "DisplayCode.s"
    
END     resetVec