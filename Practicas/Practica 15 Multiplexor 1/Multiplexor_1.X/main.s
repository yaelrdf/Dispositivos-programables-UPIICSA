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
    ; Configure PORTA as input
    CLRF    LATA            ; Clear LATA output latch
    MOVLW   0xFF            ; Load W with 0xFF (all inputs)
    MOVWF   TRISA           ; Set TRISA = 0xFF (all bits as inputs)
    
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

;===================================================
;Program

MAIN_LOOP:
    ; Read PORTA and mask to 3 bits
    MOVF    PORTA, W, A
    ANDLW   0x0F
    ; Check if PORTA changed
    CPFSEQ  last_porta, A
    GOTO    PORTA_CHANGED
    ; PORTA same, continue with current function
    GOTO    RUN_FUNCTION
    
PORTA_CHANGED:
    ; PORTA changed, reset counter and save new value
    MOVWF   last_porta, A
    CLRF    counter, A
    
RUN_FUNCTION:
    ; Get one display pattern based on current counter
    MOVF    last_porta, W, A
    CALL    SELECT_FUNCTION
    ; Display the pattern returned in W
    MOVWF   PORTB, A
    ; Delay
    CALL    DELAY_1SEC
    ; Increment counter with wraparound
    CALL    INCREMENT_COUNTER
    GOTO    MAIN_LOOP

;==============================================
; SELECT_FUNCTION - Jump to appropriate function
; Input: W = function number (0-7)
; Output: W = display pattern
;==============================================
SELECT_FUNCTION:
    MULLW   2
    MOVF    PRODL, W, a
    MOVLW   HIGH(FUNCTION_TABLE)
    MOVWF   PCLATH, a
    MOVLW   LOW(FUNCTION_TABLE)	    
    ADDWF   PRODL, W, a    
    BTFSC   STATUS, 0, a    
    INCF    PCLATH, F, a    
    MOVWF   PCL, a

FUNCTION_TABLE:
    GOTO    Ascendente_hex
    GOTO    Desendente_octal
    GOTO    FUNC_Reset
    GOTO    Asendente_decimal

;============================================
;Contador asendente Hexadecimal
;============================================
Ascendente_hex:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB           ;Display update    
    ; Increment counter
    INCF    counter, F      ; Increment counter in place
    MOVLW   15              ; Limit value
    CPFSLT  counter         ; Compare: Skip next if counter < 10
    CLRF    counter         ; Reset counter to 0 if >= 10
    GOTO    LOOP

;============================================
;Contador Desecdente_octal
;TODO Refactor for desending
;==========================================
Desecdente_octal:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB           ;Display update    
    ; Increment counter
    INCF    counter, F      ; Increment counter in place
    MOVLW   15              ; Limit value
    CPFSLT  counter         ; Compare: Skip next if counter < 10
    CLRF    counter         ; Reset counter to 0 if >= 10
    GOTO    LOOP

;=============================================
;Reset permanent
;=============================================
FUNC_Reset:
    MOVF    0, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB           ;Display update
    GOTO    LOOP

;=============================================
;Asendente_decimal
;============================================
Asendente_decimal:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    GET_COMBINACION ; Call lookup routine (returns pattern in W)
    MOVWF   PORTB           ;Display update    
    ; Increment counter
    INCF    counter, F      ; Increment counter in place
    MOVLW   9              ; Limit value
    CPFSLT  counter         ; Compare: Skip next if counter < 10
    CLRF    counter         ; Reset counter to 0 if >= 10
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
    
#include "DisplayCode.s"
    
END     resetVec