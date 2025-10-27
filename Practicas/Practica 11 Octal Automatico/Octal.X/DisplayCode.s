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
    RETLW   0b11111100    ; 0
    RETLW   0b01100000    ; 1
    RETLW   0b11011010    ; 2
    RETLW   0b11110010    ; 3
    RETLW   0b01100110    ; 4
    RETLW   0b10110110    ; 5
    RETLW   0b10111110    ; 6
    RETLW   0b11100000    ; 7