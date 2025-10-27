#include <xc.inc>
    
PSECT code

;GLOBAL GET_COMBINACION      ;Funcion accesible desde el archivo principal

GET_COMBINACION:
    ;Ajuste para evitar que el program counter se vaya a direcciones no previstas
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
    RETLW   0xAA    ; Ñ
    RETLW   0xEE    ; A
    RETLW   0x02    ; -
    RETLW   0x8E    ; F
    RETLW   0x1C    ; L
    RETLW   0xFD    ; O
    RETLW   0x0A    ; R
    RETLW   0x9E    ; E
    RETLW   0xB6    ; S