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
    RETLW   0x76    ; Y
    RETLW   0xEE    ; A
    RETLW   0x9E    ; E
    RETLW   0x1C    ; L
    RETLW   0x02    ; -
    RETLW   0xB6    ; S
    RETLW   0xEE    ; A
    RETLW   0x1C    ; L
    RETLW   0x7A    ; D
    RETLW   0xEE    ; A
    RETLW   0xAA    ; N
    RETLW   0xEE    ; A
    RETLW   0x02    ; -
    RETLW   0x8E    ; F
    RETLW   0x1C    ; L
    RETLW   0xFD    ; O
    RETLW   0x0A    ; R
    RETLW   0x9E    ; E
    RETLW   0xB6    ; S