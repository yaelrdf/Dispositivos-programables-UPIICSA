/*LO QUE TENEMOS EN EL PUERTO A LO VISUALIZAMOS EN EL PUERTO B*/
    PROCESSOR 18F4550 ; inicio 
    ; PIC18F4550 Configuration Bit Settings

; Assembly source line config statements

; CONFIG1L
  CONFIG  PLLDIV = 1            ; PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
  CONFIG  CPUDIV = OSC1_PLL2    ; System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
  CONFIG  USBDIV = 2            ; USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes from the 96 MHz PLL divided by 2)

; CONFIG1H
  CONFIG  FOSC = INTOSCIO_EC    ; Oscillator Selection bits (Internal oscillator, port function on RA6, EC used by USB (INTIO))
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOR = OFF             ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
  CONFIG  BORV = 3              ; Brown-out Reset Voltage bits (Minimum setting 2.05V)
  CONFIG  VREGEN = ON           ; USB Voltage Regulator Enable bit (USB voltage regulator enabled)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 32768         ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
  CONFIG  CCP2MX = OFF          ; CCP2 MUX bit (CCP2 input/output is multiplexed with RB3)
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = OFF           ; MCLR Pin Enable bit (RE3 input pin enabled; MCLR pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  CONFIG  ICPRT = OFF           ; Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)
#include<xc.inc>
				/* declarar variables*/
				;TODAS LAS ETIQUETAS VAN CON :
DECLARAR_VARIABLES:
PSECT 	udata_acs ;SECCION DE PROGRAMACION, las variables que estan aqui guardalas en acces bank
datoA: DS 1 ; el numero es el numero de bytes que contiene 
datoB: DS 1
;PSECT	udata ;variables en el bnk que elija el mplab
;PSECT udata_bnk7 colocala variable en el acces bank numero 7
;-------------------------------------------------------
; instrucciones}
VECTOR_RESET:
PSECT CODE,RELOC=2,ABS
; ORG 0X00 PUEDE IR O NO EN COMENTARIO
REINICIO:
    GOTO INICIO
;-------------------------------------------------
CONFIGURACION_SFR:; MAIN:
PSECT CODE,RELOC=2
ORG 0X20 ;PARA SALTAR COMPLETAMENTE EL VECTOR INTERRUPCION

INICIO:
    CLRF    ADCON0,a ; limpiar el adcon 0, para que quede el registro como digital
    MOVLW   0X0F ; mueve el 15 a W
    MOVWF   ADCON1,a ;de w al ADCON1, MUEVO EL 15
    MOVLW   00000111B ; MUEVO EL VALOR DE 7 A W ; CONFIGURA LOS COMPARADORES PARA ENTRADA DIGITAL
    MOVWF   CMCON,A ;  PARA LOS PINES RA0-RA3
    MOVLW   1111111B ; MUEVE FF A W
    MOVWF   TRISA,a ; EL 0 ES EL ACCESS BANK , habilito al puerto a como entrada
    CLRF    PORTA,a; 0 EN EL ACCES BANK , limpio
    MOVLW   00000000B; habilito al puerto b como salida
    MOVWF   TRISB,0
    CLRF    PORTB,a ; limpio
MAIN: ; etiqueta para el programa principal
    MOVFF PORTA,datoA ; mueve del registro porta a dato a para bankearlo
    MOVF  datoA,W,a ; mueve datoa al registro w
    MOVWF PORTB,a ; mueve w a portb bankeado
    GOTO MAIN ; 
    END
    


