#include "configbits.s"
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
    GOTO    FUNC_0_COLOR
    GOTO    FUNC_1_SUPERHEROE1
    GOTO    FUNC_2_UPIICSA
    GOTO    FUNC_3_DISPOSITIVOS
    GOTO    FUNC_4_SUPERHEROE2
    GOTO    FUNC_5_DEPORTE
    GOTO    FUNC_6_DECIMAL_ASC
    GOTO    FUNC_7_HEX_DESC

;==============================================
; Function 0: COLOR (4 patterns)
;==============================================
FUNC_0_COLOR:
    MOVLW   3
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_COLOR_PATTERN
    RETURN

;==============================================
; Function 1: SUPERHEROE1 - BATMAN (6 patterns)
;==============================================
FUNC_1_SUPERHEROE1:
    MOVLW  5
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_HEROES1_PATTERN
    RETURN

;==============================================
; Function 2: UPIICSA (7 patterns)
;==============================================
FUNC_2_UPIICSA:
    MOVLW   4
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_UPIICSA_PATTERN
    RETURN

;==============================================
; Function 3: DISPOSITIVOS (12 patterns)
;==============================================
FUNC_3_DISPOSITIVOS:
    MOVLW   11
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_DISPOSITIVOS_PATTERN
    RETURN

;==============================================
; Function 4: SUPERHEROE2 - THOR (4 patterns)
;==============================================
FUNC_4_SUPERHEROE2:
    MOVLW  3
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_HEROES2_PATTERN
    RETURN

;==============================================
; Function 5: DEPORTE - GIMNASIA (8 patterns)
;==============================================
FUNC_5_DEPORTE:
    MOVLW   7
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_DEPORTE_PATTERN
    RETURN

;==============================================
; Function 6: Decimal Ascendente 0-9 (10 patterns)
;==============================================
FUNC_6_DECIMAL_ASC:
    MOVLW   9
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_NUMBER_PATTERN
    RETURN

;==============================================
; Function 7: Hexadecimal Descendente F-0 (16 patterns)
;==============================================
FUNC_7_HEX_DESC:
    MOVLW   15
    MOVWF   limit, A
    MOVF    counter, W, A
    CALL    GET_NUMBER_DESC
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

;=================================================
;Lockups
;==================================================
GET_COLOR_PATTERN:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(COLOR_TABLE)
    MOVWF   PCLATH, A
    MOVLW   LOW(COLOR_TABLE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A

COLOR_TABLE:
    RETLW   0xEE    ; A - Azul
    RETLW   0xDB    ; Z
    RETLW   0x7C    ; U
    RETLW   0x1C    ; L

GET_HEROES1_PATTERN:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(HEROES1_TABLE)
    MOVWF   PCLATH, A
    MOVLW   LOW(HEROES1_TABLE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A

HEROES1_TABLE:
    RETLW   0x3E    ; B
    RETLW   0xEE    ; A
    RETLW   0x8D    ; T
    RETLW   0x9F    ; M
    RETLW   0xEE    ; A
    RETLW   0x2A    ; N

GET_UPIICSA_PATTERN:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(UPIICSA_TABLE)
    MOVWF   PCLATH, A
    MOVLW   LOW(UPIICSA_TABLE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A

UPIICSA_TABLE:
    RETLW   0x7C    ; U
    RETLW   0xCE    ; P
    RETLW   0x0C    ; I
    RETLW   0x0C    ; I
    RETLW   0x9C    ; C
    RETLW   0xB6    ; S
    RETLW   0xEE    ; A

GET_DISPOSITIVOS_PATTERN:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(DISPOSITIVOS_TABLE)
    MOVWF   PCLATH, A
    MOVLW   LOW(DISPOSITIVOS_TABLE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A

DISPOSITIVOS_TABLE:
    RETLW   0x7A    ; D
    RETLW   0x0C    ; I
    RETLW   0xB6    ; S
    RETLW   0xCE    ; P
    RETLW   0xFD    ; O
    RETLW   0xB6    ; S
    RETLW   0x0C    ; I
    RETLW   0x8D    ; T
    RETLW   0x0C    ; I
    RETLW   0x38    ; V
    RETLW   0xFD    ; O
    RETLW   0xB6    ; S

GET_HEROES2_PATTERN:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(HEROES2_TABLE)
    MOVWF   PCLATH, A
    MOVLW   LOW(HEROES2_TABLE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A

HEROES2_TABLE:
    RETLW   0x8D    ; T
    RETLW   0x2E    ; H
    RETLW   0xFD    ; O
    RETLW   0x0A    ; R

GET_DEPORTE_PATTERN:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(DEPORTE_TABLE)
    MOVWF   PCLATH, A
    MOVLW   LOW(DEPORTE_TABLE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A

DEPORTE_TABLE:
    RETLW   0xBE    ; G
    RETLW   0x0C    ; I
    RETLW   0x9F    ; M
    RETLW   0x2A    ; N
    RETLW   0xEE    ; A
    RETLW   0xB6    ; S
    RETLW   0x0C    ; I
    RETLW   0xEE    ; A

GET_NUMBER_PATTERN:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(NUMBER_TABLE)
    MOVWF   PCLATH, A
    MOVLW   LOW(NUMBER_TABLE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A
    
NUMBER_TABLE:
    RETLW   0b11111100    ; 0
    RETLW   0b01100000    ; 1
    RETLW   0b11011010    ; 2
    RETLW   0b11110010    ; 3
    RETLW   0b01100110    ; 4
    RETLW   0b10110110    ; 5
    RETLW   0b10111110    ; 6
    RETLW   0b11100000    ; 7
    RETLW   0b11111110    ; 8
    RETLW   0b11100110    ; 9
    RETLW   0b11101110    ; A
    RETLW   0b00111110    ; B
    RETLW   0b10011100    ; C
    RETLW   0b01111010    ; D
    RETLW   0b10011110    ; E
    RETLW   0b10001110    ; F

GET_NUMBER_DESC:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(NUMBER_TABLE_DESC)
    MOVWF   PCLATH, A
    MOVLW   LOW(NUMBER_TABLE_DESC)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A
    
NUMBER_TABLE_DESC:
    RETLW   0x8E    ; F
    RETLW   0x9E    ; E
    RETLW   0x7A    ; D
    RETLW   0x9C    ; C
    RETLW   0x3E    ; B
    RETLW   0xEE    ; A
    RETLW   0xE2    ; 9
    RETLW   0xFE    ; 8
    RETLW   0xE0    ; 7
    RETLW   0xBE    ; 6
    RETLW   0xB6    ; 5
    RETLW   0x66    ; 4
    RETLW   0xF2    ; 3
    RETLW   0xDA    ; 2
    RETLW   0x60    ; 1
    RETLW   0xFE    ; 0

END     resetVec