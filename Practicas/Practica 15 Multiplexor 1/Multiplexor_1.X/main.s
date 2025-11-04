#include "configbits.s"
#include "DisplayCode.s"
#include <xc.inc>

; Variables
PSECT udata_acs
counter:    DS 1 
limit:      DS 1
last_porta: DS 1

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO    CONF

PSECT code
CONF:
    ; PORTA input (lower 3 bits: RA0-RA2)
    CLRF    LATA, A 
    MOVLW   0xFF    
    MOVWF   TRISA, A
    
    ; PORTB Output
    CLRF    LATB, A
    CLRF    TRISB, A        ; All outputs
    CLRF    PORTB, A
    
    ; All pins digital
    MOVLW   0x0F
    MOVWF   ADCON1, A

    ; 8 MHz internal oscillator
    MOVLW   0b01110010
    MOVWF   OSCCON, A
    
    ; Initialize variables
    CLRF    counter, A
    MOVLW   0xFF
    MOVWF   last_porta, A    ; Invalid initial value
    
    GOTO    MAIN_LOOP

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
    GOTO    Contador_Hex_Asendente
    GOTO    Contador_octal_Desendente
    GOTO    Reset_FUNC
    GOTO    Contador_decimal_asendente

;==============================================
;Contador hexadecimal asendente
;==============================================
Contador_Hex_Asendente:
    MOVLW   15
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_COMBINACION
    RETURN

;==============================================
;Contador Octal desendente
;==============================================
Contador_octal_Desendente:
    MOVLW   7
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_NUMBER_DESC
    RETURN

;==============================================
;Reset
;==============================================
Reset_FUNC:
    MOVLW   0
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_COMBINACION
    RETURN

;==============================================
; Contador decimal Asendente
;==============================================
Contador_decimal_asendente:
    MOVLW   9
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_COMBINACION
    RETURN

;==============================================
; INCREMENT_COUNTER - Increment with wraparound
;==============================================
INCREMENT_COUNTER:
    INCF    counter, F, A
    MOVF    counter, W, A
    CPFSLT  limit, A        ; Skip if limit < counter
    RETURN                  ; Counter still valid
    CLRF    counter, A      ; Reset counter
    RETURN

;==============================================
; DELAY_1SEC - Approximately 1 second delay
;==============================================
DELAY_1SEC:
    BCF     T0CON, 7, A     ; Stop Timer0
    MOVLW   0b10000101      ; 16-bit, prescaler 1:64
    MOVWF   T0CON, A
    MOVLW   0x85            ; Preload high byte
    MOVWF   TMR0H, A
    MOVLW   0xEE            ; Preload low byte
    MOVWF   TMR0L, A
    BCF     INTCON, 2, A    ; Clear overflow flag
    BSF     T0CON, 7, A     ; Start Timer0
    
DELAY_WAIT:
    BTFSS   INTCON, 2, A    ; Wait for overflow
    GOTO    DELAY_WAIT
    BCF     INTCON, 2, A    ; Clear flag
    BCF     T0CON, 7, A     ; Stop timer
    RETURN