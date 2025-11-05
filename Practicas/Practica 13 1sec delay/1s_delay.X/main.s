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
    ; Configure PORTB as output
    CLRF    LATB            ; Clear LATB output latch
    MOVLW   0x00            ; Load W with 0x00 (all outputs)
    MOVWF   TRISB           ; Set TRISB = 0x00 (all bits as outputs)
    
    ; Disable analog functions on PORTA and PORTB
    MOVLW   0x0F            ; Configure all pins as digital (ADCON1 = 0x0F)
    MOVWF   ADCON1          ; Write to ADCON1
    
    ; Oscillator config
    MOVLW   0b01110010      ; IRCF = 111 (8 MHz), SCS = 10 (internal oscillator)
    MOVWF   OSCCON
    
    ; Initialize counter to 0
    CLRF    counter
    ; Main Program
    GOTO    LOOP

LOOP:
    CALL delay_1sec
    BTG PORTB, 2, A
    GOTO    LOOP

    
delay_1sec:
    ; Disable Timer0
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    MOVLW   0b10000101  
    MOVWF   T0CON
    MOVLW   0x85            ; High byte
    MOVWF   TMR0H
    MOVLW   0xEE            ; Low byte
    MOVWF   TMR0L
    ; Clear overflow flag
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    ; Start Timer0
    BSF     T0CON, 7        ; TMR0ON bit (bit 7)
    
delay_wait:
    ; Wait for overflow flag
    BTFSS   INTCON, 2       ; TMR0IF bit (bit 2)
    GOTO    delay_wait
    ; Clear flag and stop timer
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    
    RETURN
   
    
END     resetVec