#include "configbits.s"
`#include <xc.inc>

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF`
    
PSECT udata_acs
    counter:  DS 1
    counter2: DS 1
    
PSECT code
CONF:
    ;Configurar Puerto E como entrada
    CLRF    LATE            
    MOVLW   0x08           
    MOVWF   TRISE          
    
    ;PortB como salida
    CLRF    LATB   
    MOVLW   0x00   
    MOVWF   TRISB  
    
    ; PORTD como salida
    ;Display 2
    CLRF    LATD       
    MOVLW   0x00       
    MOVWF   TRISD      
    
    ;Deshabilitar funciones analogicas
    MOVLW   0x0F  
    MOVWF   ADCON1
    
    ;Oscilador a 8mhz
    MOVLW   0x70
    MOVWF   OSCCON
    
    ;Inicializar contadores
    CLRF    counter
    CLRF    counter2
    
    ;Programa principal
    GOTO    LOOP

LOOP:
    ;Manejo del primer display
    MOVF    counter, W      ; Contador a 2
    ANDLW   0x0F            ; Enmascarar 4 bits
    CALL    GET_COMBINACION ; Llamada a funcion de combinaciones
    MOVWF   PORTB           ; Display 
    
    ; 2nd display
    MOVF    counter2, W     ; Contador a w
    ANDLW   0x0F            ; Enmascaramiento de 4 bits
    CALL    GET_COMBINACION ; Llamada a funcion de combinaciones
    MOVWF   PORTD           ; Display 
    
    ;Manejo de boton
    BTFSC   PORTE, 3        ;Saltar si RE3 esta en un estado LOw
    GOTO    LOOP            ;Boton no pulsado volver al bucle
    
    ;Llamar a incremento
    CALL    Increment
    
    ; Debounce delay
    CALL    delay_100ms
wait_release:
    BTFSS   PORTE, 3        ; Saltar si PORTE3 ya no esta pulsado
    GOTO    wait_release    ; Regresar a esperar
    
    ; Debounce delay
    CALL    delay_100ms
    
    GOTO    LOOP

Increment:
    INCF    counter, F      ; Incrementar contador
    MOVLW   10              ; Valor limite (Partiendo de 1)
    CPFSLT  counter         ; Comparar y saltar si no ha superado el limite
    GOTO    Dual_counter    ; Si el contadod ha superado el limite ir a Dual counter
    RETURN                  

Dual_counter:
    CLRF    counter         ; Reiniciar contador1 (display decimas) a 0
    INCF    counter2, F     ; Incrementar contador 2 (Display decadas)
    MOVLW   10              ; Valor limite
    CPFSLT  counter2        ; Comparar y saltar si no ha superado el limite
    CLRF    counter2        ; Reiniciar contador
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