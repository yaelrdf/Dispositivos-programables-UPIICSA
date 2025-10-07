#include <xc.inc>
    
PSECT code

;GLOBAL GET_COMBINACION      ; Make function accessible from main file

GET_COMBINACION:
    ; Simple and reliable approach - use computed GOTO with fixed table
    MULLW   2               ; Multiply by 2 (each GOTO is 2 bytes)
    MOVF    PRODL, W, a     ; Get low byte of offset
    ; Load the high byte of table base address
    MOVLW   HIGH(TABLE)
    MOVWF   PCLATH, a
    MOVLW   LOW(TABLE)	    ; Load low byte of table base address
    ADDWF   PRODL, W, a     ; Add offset to table base low byte
    BTFSC   STATUS, 0, a    ; Check for carry
    INCF    PCLATH, F, a    ; Adjust high byte if carry
    MOVWF   PCL, a          ; Jump to the table entry

    
TABLE:
    ; Lookup table - 7-segment patterns for H, O, L, A, -, I, P, N
    RETLW   0b00101110 ; H 
    RETLW   0b11111101 ; O 
    RETLW   0b00011100 ; L 
    RETLW   0b11101110 ; A 
    RETLW   0b00000010 ; - 
    RETLW   0b00001100 ; I 
    RETLW   0b11001110 ; P 
    RETLW   0b00101010; N 