#include <xc.inc>
    PROCESSOR 18F4550
    
    ; Define variables in data memory
    PSECT udata_acs
	datoA:  DS 1    ; Reserve 1 byte for datoA
	datoB:  DS 1    ; Reserve 1 byte for datoB (though not used in your code)

    PSECT code
	INICIO:
	    CLRF    ADCON0, a    ; limpiar el adcon 0, para que quede el registro como digital
	    MOVLW   0x0F         ; mueve el 15 a W
	    MOVWF   ADCON1, a    ; de w al ADCON1, MUEVO EL 15
	    MOVLW   00000111B    ; MUEVO EL VALOR DE 7 A W ; CONFIGURA LOS COMPARADORES PARA ENTRAR
	    MOVWF   CMCON, A     ; /PARA LOS PINES RA0-RA3
	    MOVLW   11111111B    ; MUEVE FF A W
	    MOVWF   TRISA, a     ; EL 0 ES EL ACCESS BANK , habilito al puerto a como entrada
	    CLRF    PORTA, a     ; 0 EN EL ACCES BANK , limpio
	    MOVLW   00000000B    ; habilito al puerto b como salida
	    MOVWF   TRISB, 0
	    CLRF    PORTB, a     ; limpio

	MAIN:
	    ; etiqueta para el programa principal
	    MOVFF   PORTA, datoA ; mueve del registro porta a dato a para bankearlo
	    MOVF    datoA, W, a  ; mueve datos al registro w
	    MOVWF   PORTB, a     ; mueve w a portb bankeado
	    GOTO    MAIN         ;
	    END