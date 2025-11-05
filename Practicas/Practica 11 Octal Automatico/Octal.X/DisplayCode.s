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
    RETLW   0xE0    ; 7
    RETLW   0xBE    ; 6
    RETLW   0xB6    ; 5
    RETLW   0x66    ; 4
    RETLW   0xF2    ; 3
    RETLW   0xDA    ; 2
    RETLW   0x60    ; 1
    RETLW   0b11111100    ; 0