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
    RETLW   0x2E ; H - 0010 1110
    RETLW   0xFD ; O - 1111 1101  
    RETLW   0x1C ; L - 0001 1100
    RETLW   0xEC ; A - 1110 1100
    RETLW   0x02 ; - - 0000 0010
    RETLW   0x0C ; I - 0000 1100
    RETLW   0xCE ; P - 1100 1110
    RETLW   0x2A ; N - 0010 1010