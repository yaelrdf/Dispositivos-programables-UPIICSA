#include "configbits.s"
#include <xc.inc>

PSECT resetVec,class=CODE,reloc=2
resetVec:
    GOTO CONF

PSECT udata_acs
counter_1s_low:  DS 1    ; Low byte for 1 second counter
counter_1s_high: DS 1    ; High byte for 1 second counter
counter_3s:      DS 1    ; Counter for 3 seconds
    
PSECT code
    ORG 0x0000
    GOTO CONF
 
CONF:
    ; Configure internal oscillator to 8 MHz
    MOVLW 0b01110010        ; IRCF = 111 (8 MHz), SCS = 00 (use config bit selection)
    MOVWF OSCCON, A
    
    ; Wait for oscillator to be stable (optional but recommended)
wait_osc_stable:
    ;BTFSS OSCCON, 2, A      ; Check IOFS bit (Internal Oscillator Frequency Stable)
    ;GOTO wait_osc_stable
    
    ; Configure PORTA as input
    CLRF LATA, 0
    SETF TRISA, 0
    
    ; Configure PORTB as output
    CLRF LATB, 0
    CLRF TRISB, 0
    
    ; Disable analog functions
    MOVLW 0x0F
    MOVWF ADCON1, 0
    
    ; Initialize counters
    MOVLW LOW(1000)      ; Low byte of 1000
    MOVWF counter_1s_low, A
    MOVLW HIGH(1000)     ; High byte of 1000
    MOVWF counter_1s_high, A
    MOVLW 3
    MOVWF counter_3s, A
    
    GOTO LOOP
    
LOOP:
    ; Call 1ms delay
    CALL delay_1ms
    
    ; Toggle RB2 every 1ms
    BTG PORTB, 2, A
    
    ; Decrement 16-bit counter (1000ms)
    MOVF counter_1s_low, W, A
    SUBLW 0
    BTFSS STATUS, 2, A       ; Skip if low byte is zero
    GOTO dec_low_byte
    
    ; Low byte is zero, check high byte
    MOVF counter_1s_high, W, A
    SUBLW 0
    BTFSS STATUS, 2, A       ; Skip if high byte is zero
    GOTO dec_high_byte
    
    ; Both bytes zero - 1 second reached
    BTG PORTB, 4, A          ; Toggle RB4
    
    ; Reset 1-second counter
    MOVLW LOW(1000)
    MOVWF counter_1s_low, A
    MOVLW HIGH(1000)
    MOVWF counter_1s_high, A
    
    ; Decrement 3-second counter
    DECF counter_3s, F, A
    BTFSS STATUS, 2, A
    GOTO LOOP
    
    ; 3 seconds reached
    BTG PORTB, 5, A
    MOVLW 3
    MOVWF counter_3s, A
    GOTO LOOP
    
dec_high_byte:
    DECF counter_1s_high, F, A
    MOVLW 0xFF
    MOVWF counter_1s_low, A
    GOTO LOOP
    
dec_low_byte:
    DECF counter_1s_low, F, A
    GOTO LOOP

; 1ms Delay using Timer0
; Fosc = 8 MHz (internal oscillator)
; Instruction cycle: Tcy = 4/Fosc = 4/8MHz = 0.5 µs
; For 1ms: need 1ms / 0.5µs = 2000 instruction cycles
; With 1:8 prescaler: 2000/8 = 250 counts needed
; TMR0 preload = 65536 - 250 = 65286 = 0xFF06
delay_1ms:
    MOVLW 0xFF        ; High byte
    MOVWF TMR0H, A
    MOVLW 0x06        ; Low byte (65286 = 0xFF06)
    MOVWF TMR0L, A
    BCF INTCON, 2, A  ; Clear TMR0IF flag before starting
    MOVLW 0b10000010  ; Timer ON, 16-bit, internal clock, 1:8 prescaler
    MOVWF T0CON, A
    
wait_1ms:
    BTFSS INTCON, 2, A    ; Check TMR0IF flag
    GOTO wait_1ms
    BCF INTCON, 2, A      ; Clear flag
    BCF T0CON, 7, A       ; Turn off timer (TMR0ON = 0)
    RETURN
    
END