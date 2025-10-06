; PIC18F4550 Seven Segment Display Driver with Input Limiter
; Compiler: PIC-AS (MPASM compatible syntax)
; Description: Reads value from PORTA and displays corresponding digit on 7-segment display connected to PORTB
;              If input exceeds MAX_VALUE, display holds last valid value

    PROCESSOR 18F4550
    #include <xc.inc>
    
; Configuration bits
    CONFIG FOSC = INTOSC_EC     ; Internal oscillator
    CONFIG WDT = OFF            ; Watchdog Timer disabled
    CONFIG LVP = OFF            ; Low Voltage Programming disabled
    CONFIG PBADEN = OFF         ; PORTB pins configured as digital I/O on reset

; *** CHANGE THIS VALUE TO MODIFY THE INPUT LIMIT ***
; Current limit: 8 (accepts 0-8, rejects 9 and above)
MAX_VALUE EQU 8                 ; Maximum allowed input value
    
; Reset vector
    PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto    main

; Main program
    PSECT code
main:
    ; Configure PORTA as input
    MOVLW   0xFF                ; Load 0xFF (all bits set to 1)
    MOVWF   TRISA,ACCESS        ; Set all PORTA pins as inputs
    
    ; Configure PORTB as output
    CLRF    TRISB,ACCESS        ; Clear TRISB to make all PORTB pins outputs
    
    ; Configure PORTA as digital (disable analog)
    MOVLW   0x0F                ; Load value to configure AN0-AN11 as digital
    MOVWF   ADCON1,ACCESS       ; Write to ADCON1 to disable analog on PORTA
    
    CLRF    PORTB,ACCESS        ; Clear PORTB initially (all segments off)

; Main loop - continuously read PORTA and update display
loop:
    MOVF    PORTA,W,ACCESS      ; Read PORTA value into W register
    ANDLW   0x0F                ; Mask lower 4 bits (keep only values 0-15)
    
    ; Compare input with MAX_VALUE
    MOVWF   WREG                ; Ensure value is in W for comparison
    SUBLW   MAX_VALUE           ; Subtract W from MAX_VALUE (result in W)
                                ; If W <= MAX_VALUE, result is positive or zero (no borrow, C=1)
                                ; If W > MAX_VALUE, result is negative (borrow occurs, C=0)
    
    BC      valid_input         ; Branch if Carry set (input <= MAX_VALUE)
    
    ; If we reach here, input is out of range - skip display update
    GOTO    loop                ; Return to loop without updating display
    
valid_input:
    MOVF    PORTA,W,ACCESS      ; Read PORTA value again into W
    ANDLW   0x0F                ; Mask lower 4 bits
    
    ; Use W as offset to fetch 7-segment pattern from lookup table
    CALL    get_segment         ; Call lookup routine (returns pattern in W)
    
    MOVWF   PORTB,ACCESS        ; Write 7-segment pattern to PORTB (update display)
    
    GOTO    loop                ; Repeat forever

; Lookup table for 7-segment display patterns (common cathode)
; Segments: ABCDEFG (bit 7 unused, bit 0 = segment A)
; Pattern format: .GFEDCBA
get_segment:
    ADDWF   PCL,F,ACCESS        ; Add W to Program Counter Low (jump to corresponding entry)
    RETLW   0x3F                ; 0: displays "0" (segments A,B,C,D,E,F on)
    RETLW   0x06                ; 1: displays "1" (segments B,C on)
    RETLW   0x5B                ; 2: displays "2" (segments A,B,D,E,G on)
    RETLW   0x4F                ; 3: displays "3" (segments A,B,C,D,G on)
    RETLW   0x66                ; 4: displays "4" (segments B,C,F,G on)
    RETLW   0x6D                ; 5: displays "5" (segments A,C,D,F,G on)
    RETLW   0x7D                ; 6: displays "6" (segments A,C,D,E,F,G on)
    RETLW   0x07                ; 7: displays "7" (segments A,B,C on)
    RETLW   0x7F                ; 8: displays "8" (all segments on)
    RETLW   0x6F                ; 9: displays "9" (segments A,B,C,D,F,G on)
    RETLW   0x00                ; 10-15: blank display (for invalid inputs)
    RETLW   0x00
    RETLW   0x00
    RETLW   0x00
    RETLW   0x00
    RETLW   0x00

    END     resetVec