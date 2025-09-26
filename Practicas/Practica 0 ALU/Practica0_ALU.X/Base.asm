; C�digo en ensamblador para PIC18F4550
; An�lisis l�nea por l�nea con comentarios explicativos

LIST p=18f4550        ; Especifica el procesador PIC18F4550

; L�neas 2-11: Definici�n de variables y configuraci�n de puertos
D1      EQU 0x1D      ; Define variable D1 en direcci�n 0x10 (registro de prop�sito general)
D2      EQU 0x0E      ; Define variable D2 en direcci�n 0x0E  
R_3     EQU 0x0F      ; Define variable R_3 en direcci�n 0x0F
R_R     EQU 0x10      ; Define variable R_R en direcci�n 0x10 (misma que D1)
R_AN    EQU 0x11      ; Define variable R_AN en direcci�n 0x11
R_OR    EQU 0x12      ; Define variable R_OR en direcci�n 0x12
R_XOR   EQU 0x13      ; Define variable R_XOR en direcci�n 0x13
R_NOT1  EQU 0x11      ; Define variable R_NOT1 en direcci�n 0x11 (misma que R_AN)
R_NOT2  EQU 0x15      ; Define variable R_NOT2 en direcci�n 0x15
        ORG 0         ; Establece el contador de programa en direcci�n 0 (vector de reset)

;Operaciones logicas
MOVLW 0x34           ; Mueve literal 0x34 (52 decimal) al registro W
                     ; W = 0x34
MOVWF D1             ; Mueve contenido de W a la variable D1
                     ; D1 = 0x34
MOVLW 0x12           ; Mueve literal 0x12 (18 decimal) al registro W  
                     ; W = 0x12
MOVWF D2             ; Mueve contenido de W a la variable D2
                     ; D2 = 0x12
ADDWF D1,W           ; Suma D1 + W, resultado en W
                     ; W = 0x34 + 0x12 = 0x46 (70 decimal)
MOVWF R_3            ; Mueve resultado de suma a R_3
                     ; R_3 = 0x46

;Operaciones logicas
MOVWF D2,W           ; Mueve D2 a W (sintaxis incorrecta, deber�a ser MOVF D2,W)
SUBWF D1,W           ; Resta W de D1, resultado en W
                     ; W = D1 - W = 0x34 - 0x12 = 0x22
MOVWF R_R            ; Mueve resultado de resta a R_R
                     ; R_R = 0x22

MOVWF D2,W           ; Mueve D2 a W
ANDWF D1,W           ; AND l�gico entre D1 y W, resultado en W
                     ; W = 0x34 AND 0x12 = 0x10
MOVWF R_AN           ; Almacena resultado AND en R_AN

MOVWF D2,W           ; Mueve D2 a W  
IORWF D1,W           ; OR inclusivo entre D1 y W, resultado en W
                     ; W = 0x34 OR 0x12 = 0x36
MOVWF R_OR           ; Almacena resultado OR en R_OR

MOVWF D2,W           ; Mueve D2 a W
XORWF D1,W           ; XOR entre D1 y W, resultado en W  
                     ; W = 0x34 XOR 0x12 = 0x26
MOVWF R_XOR          ; Almacena resultado XOR en R_XOR

MOVWF D1,W           ; Mueve D1 a W
COMF D1,W            ; Complemento (NOT) de D1, resultado en W
                     ; W = NOT 0x34 = 0xCB  
MOVWF R_NOT1         ; Almacena complemento en R_NOT1

MOVWF D2,W           ; Mueve D2 a W
COMF D2,W            ; Complemento (NOT) de D2, resultado en W
                     ; W = NOT 0x12 = 0xED
MOVWF R_NOT2         ; Almacena complemento en R_NOT2

END                  ; Fin del programa