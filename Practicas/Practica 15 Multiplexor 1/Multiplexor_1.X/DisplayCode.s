#include <xc.inc>
PSECT code

GET_COMBINACION:
    ;Funcion para lookup table
    MULLW   2               
    MOVF    PRODL, W, a     
    MOVLW   HIGH(TABLE)
    MOVWF   PCLATH, a
    MOVLW   LOW(TABLE)	    
    ADDWF   PRODL, W, a     
    BTFSC   STATUS, 0, a    
    INCF    PCLATH, F, a    
    MOVWF   PCL, a          

    
TABLE:
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
    RETLW   0xE0    ; 7
    RETLW   0xBE    ; 6
    RETLW   0xB6    ; 5
    RETLW   0x66    ; 4
    RETLW   0xF2    ; 3
    RETLW   0xDA    ; 2
    RETLW   0x60    ; 1
    RETLW   0b11111100    ; 0