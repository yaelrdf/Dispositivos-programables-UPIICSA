TABLA:
    retlw   b'01110111'         ; A 
    retlw   b'01111100'         ; b 
    retlw   b'00111001'         ; C
    retlw   b'01011110'         ; d 
    retlw   b'01111001'         ; E 
    retlw   b'01110001'         ; F 
 
    
Display_A:
    movlw   b'01110111'     
    movwf   PORTB
    goto    MAIN

Display_b:
    movlw   b'01111100'     
    movwf   PORTB
    goto    MAIN

Display_C:
    movlw   b'00111001'     
    movwf   PORTB
    goto    MAIN

Display_d:
    movlw   b'01011110'     
    movwf   PORTB
    goto    MAIN

Display_E:
    movlw   b'01111001'     
    movwf   PORTB
    goto    MAIN

Display_F:
    movlw   b'01110001'     
    movwf   PORTB
    goto    MAIN



