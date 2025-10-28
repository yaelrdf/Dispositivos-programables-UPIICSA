#include "configbits.s"
#include <xc.inc>

; Variables
PSECT udata_acs
counter:    DS 1 
limit:      DS 1
current_func: DS 1  ; Store current function index

; Reset vector
PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO    CONF

PSECT code
CONF:
    ; PORTA input
    CLRF    LATA, A 
    MOVLW   0xFF    
    MOVWF   TRISA, A
    
    ; PORTB Output
    CLRF    LATB, A         ; Clear LATB output latch
    MOVLW   0x00            ; Load W with 0x00 (all outputs)
    MOVWF   TRISB, A        ; Set TRISB = 0x00 (all bits as outputs)
    BCF     PORTB, 5, A
    
    ; Pines digitales
    MOVLW   0x0F            ; Configure all pins as digital (ADCON1 = 0x0F)
    MOVWF   ADCON1, A       ; Write to ADCON1

    ; Oscillator config
    MOVLW   0b01110010      ; IRCF = 111 (8 MHz), SCS = 10 (internal oscillator)
    MOVWF   OSCCON, A
    
    ; Initialize variables
    CLRF    counter, A
    CLRF    current_func, A
    
    ; Main Program
    GOTO    LOOP

LOOP:
    MOVF    PORTA, W, A     ; Read Puerto A
    ANDLW   0x0F            ; Mask lower 4 bits
    MOVWF   current_func    ; Save current function index
    CALL    Multiplex       ; Call multiplex function
    GOTO    LOOP

Multiplex:
    ; Adjust to prevent program counter from going to unexpected addresses
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(CASE)
    MOVWF   PCLATH, A
    MOVLW   LOW(CASE)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A    

CASE:
    GOTO     Color
    GOTO     SuperHeroe1
    GOTO     Upiicsa
    GOTO     Dispositivos
    GOTO     SuperHeroe2
    GOTO     Deporte
    GOTO     Decimal_Asendente
    GOTO     Hexadecimal_Desendente

; Common display function with loop checking
; Input: WREG = display pattern
Display_Loop:
    MOVWF   PORTB           ; Display update first
    CALL    delay_1sec
    
    ; Check if function changed
    MOVF    PORTA, W, A     ; Read PORTA
    ANDLW   0x0F            ; Mask lower 4 bits
    CPFSEQ  current_func    ; Compare with saved function index
    GOTO     Function_Changed ; If changed, exit
    RETURN                  ; Continue if same function
    
Function_Changed:
    CLRF    counter         ; Reset counter
    RETURN

Color:
    MOVLW   4               ; Limit = 4
    MOVWF   limit
Color_Loop:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_Colour      ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    CALL    Counter_increment
    RETURN                  ; Return to main if function changed
    GOTO     Color_Loop

SuperHeroe1:
    MOVLW   6               ; Limit = 6
    MOVWF   limit
SuperHeroe1_Loop:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_Heroes1     ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    RETURN                  ; Return to main if function changed
    CALL    Counter_increment
    GOTO     SuperHeroe1_Loop

SuperHeroe2:
    MOVLW   4               ; Limit = 4
    MOVWF   limit
SuperHeroe2_Loop:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_Heroes2     ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    RETURN                  ; Return to main if function changed
    CALL    Counter_increment
    GOTO     SuperHeroe2_Loop

Deporte:
    MOVLW   8               ; Limit = 8
    MOVWF   limit
Deporte_Loop:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_Sport       ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    RETURN                  ; Return to main if function changed
    CALL    Counter_increment
    GOTO     Deporte_Loop

Decimal_Asendente:
    MOVLW   10              ; Limit = 10
    MOVWF   limit
Decimal_Loop:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_number      ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    RETURN                  ; Return to main if function changed
    CALL    Counter_increment
    GOTO     Decimal_Loop

Hexadecimal_Desendente:
    MOVLW   16              ; Limit = 16 (0-F)
    MOVWF   limit
Hexadecimal_Loop:
    MOVLW   15              ; Start from 15 (F)
    MOVF    counter, W      ; Counter to W
    SUBWF   WREG, W         ; W = 15 - counter
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_number      ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    RETURN                  ; Return to main if function changed
    
    ; Decrement counter with wrap-around from F to 0
    MOVF    counter, W
    ADDLW   0xFF            ; Decrement by adding -1
    ANDLW   0x0F            ; Keep only lower 4 bits
    MOVWF   counter
    GOTO     Hexadecimal_Loop

Upiicsa:
    MOVLW   6               ; Limit = 6 (U P I I C S A)
    MOVWF   limit
Upiicsa_Loop:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_Upiicsa     ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    RETURN                  ; Return to main if function changed
    CALL    Counter_increment
    GOTO     Upiicsa_Loop

Dispositivos:
    MOVLW   11              ; Limit = 11 (D I S P O S I T I V O S)
    MOVWF   limit
Dispositivos_Loop:
    MOVF    counter, W      ; Counter to W
    ANDLW   0x0F            ; Mask lower 4 bits
    CALL    Get_Dispositivos ; Call lookup routine
    CALL    Display_Loop    ; Display and check for change
    BTFSC   STATUS, 2       ; Check if Z flag set (function changed)
    RETURN                  ; Return to main if function changed
    CALL    Counter_increment
    GOTO     Dispositivos_Loop

; Lookup tables with proper program counter adjustment
Get_Colour:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(Colour_Table)
    MOVWF   PCLATH, A
    MOVLW   LOW(Colour_Table)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A   

Colour_Table:
    RETLW   0xEE    ; A
    RETLW   0xDB    ; Z
    RETLW   0x7C    ; U
    RETLW   0x1C    ; L

Get_Heroes1:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(Heroes1_Table)
    MOVWF   PCLATH, A
    MOVLW   LOW(Heroes1_Table)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A   

Heroes1_Table:
    RETLW   0x3E    ; B
    RETLW   0xEE    ; A
    RETLW   0x8D    ; T
    RETLW   0x9F    ; M
    RETLW   0xEE    ; A
    RETLW   0x2A    ; N

Get_Heroes2:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(Heroes2_Table)
    MOVWF   PCLATH, A
    MOVLW   LOW(Heroes2_Table)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A 

Heroes2_Table:
    RETLW   0x8D    ; T
    RETLW   0x2E    ; H
    RETLW   0xFD    ; O
    RETLW   0x0A    ; R

Get_Sport:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(Sport_Table)
    MOVWF   PCLATH, A
    MOVLW   LOW(Sport_Table)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A 

Sport_Table:
    RETLW   0xBE    ; G
    RETLW   0x0C    ; I
    RETLW   0x9F    ; M
    RETLW   0x2A    ; N
    RETLW   0xEE    ; A
    RETLW   0xB6    ; S
    RETLW   0x0C    ; I
    RETLW   0xEE    ; A

Get_Upiicsa:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(Upiicsa_Table)
    MOVWF   PCLATH, A
    MOVLW   LOW(Upiicsa_Table)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A 

Upiicsa_Table:
    RETLW   0x1C    ; U - Replace with correct segment pattern
    RETLW   0x9F    ; P - Replace with correct segment pattern  
    RETLW   0x0C    ; I - Replace with correct segment pattern
    RETLW   0x0C    ; I - Replace with correct segment pattern
    RETLW   0x9C    ; C - Replace with correct segment pattern
    RETLW   0xB6    ; S - Replace with correct segment pattern
    RETLW   0xEE    ; A - Replace with correct segment pattern

Get_Dispositivos:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(Dispositivos_Table)
    MOVWF   PCLATH, A
    MOVLW   LOW(Dispositivos_Table)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A 

Dispositivos_Table:
    RETLW   0x5E    ; D - Replace with correct segment pattern
    RETLW   0x0C    ; I - Replace with correct segment pattern
    RETLW   0xB6    ; S - Replace with correct segment pattern
    RETLW   0x9F    ; P - Replace with correct segment pattern
    RETLW   0xFD    ; O - Replace with correct segment pattern
    RETLW   0xB6    ; S - Replace with correct segment pattern
    RETLW   0x0C    ; I - Replace with correct segment pattern
    RETLW   0x8D    ; T - Replace with correct segment pattern
    RETLW   0x0C    ; I - Replace with correct segment pattern
    RETLW   0x5F    ; V - Replace with correct segment pattern
    RETLW   0xFD    ; O - Replace with correct segment pattern
    RETLW   0xB6    ; S - Replace with correct segment pattern

Get_number:
    MULLW   2
    MOVF    PRODL, W, A    
    MOVLW   HIGH(Number_Table)
    MOVWF   PCLATH, A
    MOVLW   LOW(Number_Table)	    
    ADDWF   PRODL, W, A     
    BTFSC   STATUS, 0, A    
    INCF    PCLATH, F, A    
    MOVWF   PCL, A    

Number_Table:
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

; Global Functions
Counter_increment:
    INCF    counter, F      ; Increment counter in place
    MOVF    limit, W        ; Limit value
    CPFSLT  counter         ; Compare: Skip next if counter < limit
    CLRF    counter         ; Reset counter to 0 if >= limit
    RETURN

; Delay
delay_1sec:
    ; Disable Timer0
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    MOVLW   0b10000101  
    MOVWF   T0CON
    MOVLW   0x85            ; High byte
    MOVWF   TMR0H
    MOVLW   0xEE            ; Low byte
    MOVWF   TMR0L
    ; Clear overflow flag
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    ; Start Timer0
    BSF     T0CON, 7        ; TMR0ON bit (bit 7)
    
delay_wait:
    ; Wait for overflow flag
    BTFSS   INTCON, 2       ; TMR0IF bit (bit 2)
    GOTO     delay_wait
    ; Clear flag and stop timer
    BCF     INTCON, 2       ; TMR0IF bit (bit 2)
    BCF     T0CON, 7        ; TMR0ON bit (bit 7)
    RETURN

END     resetVec