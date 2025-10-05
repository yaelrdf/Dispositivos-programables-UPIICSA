#include <xc.inc>
    
PSECT code

;GLOBAL GET_COMBINACION      ; Make function accessible from main file

GET_COMBINACION:
    ADDWF PCL  ; Add W to Program Counter Low (jump to corresponding entry)
    RETLW   0x2E ; H
    RETLW   0xFD ; O
    RETLW   0x1C ; L
    RETLW   0xEC ; A
    RETLW   0x02 ; -
    RETLW   0x0C ; I
    RETLW   0xCE ; P
    RETLW   0x2A ; N