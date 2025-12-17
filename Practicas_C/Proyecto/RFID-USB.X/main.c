/*****************************************************************************
 * SISTEMA DE CONTROL DE ACCESO RFID CON RC522 (SPI)
 * Microcontrolador: PIC18F4550
 * Compilador: XC8
 * 
 * CONFIGURACIÓN DE CLOCK CORREGIDA:
 * =================================
 * Opción 1 (RECOMENDADA): Oscilador Interno
 *   - 8MHz interno sin PLL = 8MHz CPU (_XTAL_FREQ = 8000000)
 *   - Más estable y simple
 * 
 * Opción 2: Oscilador Interno con PLL
 *   - 8MHz interno con PLL = 48MHz / CPUDIV = variable
 *   - Requiere configuración más cuidadosa
 *****************************************************************************/

// ===== OPCIÓN 1: SIN PLL (RECOMENDADA PARA DEPURACIÓN) =====
#define _XTAL_FREQ 8000000  // 8MHz sin PLL

// ===== OPCIÓN 2: CON PLL (descomenta si quieres 32MHz) =====
// #define _XTAL_FREQ 32000000  // 8MHz * 4 = 32MHz

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "config_bits.h"
#include "lcd.h"
#include "users.h"

// ===================== DEBUG MODE =====================
#define DEBUG_MODE 1  // Cambiar a 0 para deshabilitar debug

// ===================== DEFINICIONES DE PINES =====================
// RC522 SPI
#define RC522_RST_TRIS  TRISCbits.TRISC1
#define RC522_RST       LATCbits.LATC1
#define RC522_CS_TRIS   TRISCbits.TRISC2
#define RC522_CS        LATCbits.LATC2

// Indicadores
#define BUZZER_TRIS     TRISBbits.TRISB4
#define BUZZER          LATBbits.LATB4
#define LED_GREEN_TRIS  TRISBbits.TRISB5
#define LED_GREEN       LATBbits.LATB5
#define LED_RED_TRIS    TRISBbits.TRISB6
#define LED_RED         LATBbits.LATB6

// ===================== COMANDOS Y REGISTROS RC522 =====================
// Comandos
#define PCD_IDLE           0x00
#define PCD_AUTHENT        0x0E
#define PCD_RECEIVE        0x08
#define PCD_TRANSMIT       0x04
#define PCD_TRANSCEIVE     0x0C
#define PCD_RESETPHASE     0x0F
#define PCD_CALCCRC        0x03

// Registros importantes
#define REG_COMMAND        0x01
#define REG_COM_IRQ        0x04
#define REG_DIV_IRQ        0x05
#define REG_ERROR          0x06
#define REG_STATUS2        0x08
#define REG_FIFO_DATA      0x09
#define REG_FIFO_LEVEL     0x0A
#define REG_CONTROL        0x0C
#define REG_BIT_FRAMING    0x0D
#define REG_MODE           0x11
#define REG_TX_CONTROL     0x14
#define REG_TX_ASK         0x15
#define REG_CRC_RESULT_L   0x21
#define REG_CRC_RESULT_H   0x22
#define REG_MOD_WIDTH      0x24
#define REG_T_MODE         0x2A
#define REG_T_PRESCALER    0x2B
#define REG_T_RELOAD_H     0x2C
#define REG_T_RELOAD_L     0x2D

// Comandos PICC
#define PICC_REQIDL        0x26
#define PICC_REQALL        0x52
#define PICC_ANTICOLL      0x93
#define PICC_SELECTTAG     0x93
#define PICC_HALT          0x50

// Status
#define MI_OK              0
#define MI_NOTAGERR        1
#define MI_ERR             2

// ===================== VARIABLES GLOBALES =====================
char lcdBuffer[17];

// ===================== PROTOTIPOS =====================
void system_init(void);
void spi_init(void);
uint8_t spi_transfer(uint8_t data);
void debug_showMessage(const char *msg, uint8_t line);
void debug_showByte(const char *label, uint8_t value);

// RC522
void rc522_init(void);
uint8_t rc522_readReg(uint8_t addr);
void rc522_writeReg(uint8_t addr, uint8_t val);
void rc522_setBitMask(uint8_t reg, uint8_t mask);
void rc522_clearBitMask(uint8_t reg, uint8_t mask);
void rc522_antennaOn(void);
uint8_t rc522_request(uint8_t reqMode, uint8_t *tagType);
uint8_t rc522_toCard(uint8_t command, uint8_t *sendData, uint8_t sendLen, 
                     uint8_t *backData, uint16_t *backLen);
uint8_t rc522_anticoll(uint8_t *serNum);
void rc522_calculateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData);
uint8_t rc522_selectTag(uint8_t *serNum);
void rc522_halt(void);
uint8_t rc522_testConnection(void);

// Utilidades
void beep(uint8_t times);
void showAccess(uint8_t granted, const char *name);
const char* findUser(const char *uid);

// ===================== FUNCIÓN PRINCIPAL =====================
void main(void) {
    uint8_t status;
    uint8_t tagType[2];
    uint8_t serNum[5];
    char uidString[9];
    const char *userName;
    uint8_t lastCardPresent = 0;
    
    system_init();
    
    __delay_ms(500);  // Esperar estabilización del sistema
    
    lcd_begin(16, 2);
    
    // Pantalla de inicio
    lcd_clear();
    lcd_printCenter("Sistema RFID", 0);
    lcd_printCenter("v1.0", 1);
    __delay_ms(1500);
    
    // Mostrar frecuencia de reloj
    lcd_clear();
    lcd_print("Clock:");
    lcd_setCursor(0, 1);
    #if _XTAL_FREQ == 8000000
        lcd_print("8MHz Internal");
    #elif _XTAL_FREQ == 32000000
        lcd_print("32MHz (PLL)");
    #else
        lcd_print("Custom");
    #endif
    __delay_ms(1500);
    
    // Inicializar SPI
    lcd_clear();
    debug_showMessage("Init SPI...", 0);
    spi_init();
    debug_showMessage("SPI OK", 1);
    __delay_ms(1000);
    
    // Test SPI/RC522 connection
    lcd_clear();
    debug_showMessage("Test RC522...", 0);
    
    if (rc522_testConnection()) {
        debug_showMessage("Conexion OK!", 1);
        beep(1);
    } else {
        debug_showMessage("Sin respuesta!", 1);
        LED_RED = 1;
        beep(3);
        while(1) {
            // Quedarse aquí si no hay conexión
            __delay_ms(1000);
        }
    }
    __delay_ms(1500);
    
    // Inicializar RC522
    lcd_clear();
    debug_showMessage("Init RC522...", 0);
    rc522_init();
    debug_showMessage("RC522 OK", 1);
    beep(1);
    __delay_ms(1500);
    
    // Cargar usuarios
    lcd_clear();
    lcd_printCenter("Usuarios:", 0);
    lcd_setCursor(0, 1);
    lcd_printNum(USER_COUNT);
    lcd_print(" cargados");
    beep(1);
    __delay_ms(2000);
    
    // Pantalla idle
    lcd_clear();
    lcd_printCenter("Control de", 0);
    lcd_printCenter("Acceso", 1);
    
    // Loop principal
    while(1) {
        // Buscar tarjeta
        status = rc522_request(PICC_REQIDL, tagType);
        
        if (status == MI_OK) {
            if (!lastCardPresent) {
                // Nueva tarjeta detectada
                lcd_clear();
                lcd_printCenter("Tarjeta", 0);
                lcd_printCenter("detectada", 1);
                LED_GREEN = 1;
                __delay_ms(300);
                LED_GREEN = 0;
                
                status = rc522_anticoll(serNum);
                
                if (status == MI_OK) {
                    // Convertir UID a string hexadecimal
                    sprintf(uidString, "%02X%02X%02X%02X", 
                            serNum[0], serNum[1], serNum[2], serNum[3]);
                    
                    // Mostrar UID en LCD
                    lcd_clear();
                    lcd_print("UID:");
                    lcd_setCursor(0, 1);
                    lcd_print(uidString);
                    __delay_ms(1500);
                    
                    // Buscar usuario
                    userName = findUser(uidString);
                    
                    if (userName != NULL) {
                        // Acceso concedido
                        showAccess(1, userName);
                        LED_GREEN = 1;
                        beep(1);
                        
                        __delay_ms(2000);
                        LED_GREEN = 0;
                    } else {
                        // Acceso denegado
                        showAccess(0, uidString);
                        LED_RED = 1;
                        beep(3);
                        
                        __delay_ms(2000);
                        LED_RED = 0;
                    }
                    
                    // Halt
                    rc522_halt();
                    
                    // Volver a pantalla idle
                    lcd_clear();
                    lcd_printCenter("Control de", 0);
                    lcd_printCenter("Acceso", 1);
                    
                    lastCardPresent = 1;
                    __delay_ms(1000);
                } else {
                    lcd_clear();
                    lcd_printCenter("Error:", 0);
                    lcd_printCenter("Lectura", 1);
                    beep(2);
                    __delay_ms(1000);
                    lcd_clear();
                    lcd_printCenter("Control de", 0);
                    lcd_printCenter("Acceso", 1);
                }
            }
        } else {
            lastCardPresent = 0;
        }
        
        __delay_ms(100);
    }
}

// ===================== INICIALIZACIÓN DEL SISTEMA =====================
void system_init(void) {
    // ===== CONFIGURACIÓN DE RELOJ =====
    #if _XTAL_FREQ == 8000000
        // Opción 1: 8MHz interno SIN PLL (más estable)
        OSCCONbits.IRCF = 0b111;  // 8MHz
        OSCCONbits.SCS = 0b00;    // Primary oscillator
        while(!OSCCONbits.IOFS);  // Esperar estabilidad
    #elif _XTAL_FREQ == 32000000
        // Opción 2: Con PLL para 32MHz
        // Requiere config bits correctos (ver abajo)
        OSCCONbits.IRCF = 0b111;  // 8MHz interno
        OSCCONbits.SCS = 0b00;
        while(!OSCCONbits.IOFS);
    #endif
    
    // Configurar puertos
    ADCON1 = 0x0F;  // Todo digital
    CMCON = 0x07;   // Comparadores off
    
    // Configurar pines SPI
    TRISCbits.TRISC7 = 0;  // SDO (MOSI) - salida
    TRISCbits.TRISC6 = 0;  // SCK - salida
    TRISBbits.TRISB0 = 1;  // SDI (MISO) - entrada
    
    // Pines de control RC522
    RC522_RST_TRIS = 0;   // Output
    RC522_CS_TRIS = 0;    // Output
    
    // Indicadores
    BUZZER_TRIS = 0;
    LED_GREEN_TRIS = 0;
    LED_RED_TRIS = 0;
    
    // Estado inicial
    RC522_CS = 1;      // CS alto = inactivo
    RC522_RST = 0;     // Mantener en reset inicialmente
    BUZZER = 0;
    LED_GREEN = 0;
    LED_RED = 0;
}

// ===================== SPI =====================
void spi_init(void) {
    // Configurar MSSP para SPI Master
    
    // Calcular divisor de reloj apropiado
    #if _XTAL_FREQ == 8000000
        // 8MHz / 4 = 2MHz (velocidad segura)
        SSPCON1 = 0x20;   // SSPEN=1, SSPM=0000 (Fosc/4)
    #elif _XTAL_FREQ == 32000000
        // 32MHz / 16 = 2MHz (velocidad segura)
        SSPCON1 = 0x21;   // SSPEN=1, SSPM=0001 (Fosc/16)
    #else
        // Default: Fosc/16
        SSPCON1 = 0x21;
    #endif
    
    // SSPSTAT: CKE=1 (transmit on active to idle transition)
    SSPSTAT = 0x40;   // CKE = 1
    
    __delay_ms(10);
}

uint8_t spi_transfer(uint8_t data) {
    // Limpiar flag de overflow si existe
    if (SSPCON1bits.WCOL) {
        SSPCON1bits.WCOL = 0;
    }
    
    SSPBUF = data;
    
    // Esperar a que se complete la transferencia
    while(!SSPSTATbits.BF);
    
    return SSPBUF;
}

// ===================== DEBUG FUNCTIONS =====================
void debug_showMessage(const char *msg, uint8_t line) {
    #if DEBUG_MODE
        lcd_setCursor(0, line);
        lcd_print((char*)msg);
    #endif
}

void debug_showByte(const char *label, uint8_t value) {
    #if DEBUG_MODE
        char buffer[16];
        sprintf(buffer, "%s:%02X", label, value);
        lcd_clear();
        lcd_print(buffer);
        __delay_ms(1000);
    #endif
}

// ===================== RC522 TEST CONNECTION =====================
uint8_t rc522_testConnection(void) {
    uint8_t version;
    
    // Liberar reset
    RC522_RST = 1;
    __delay_ms(50);
    
    // Intentar leer registro de versión (debería ser 0x91 o 0x92)
    version = rc522_readReg(0x37);  // VersionReg
    
    debug_showByte("Ver", version);
    
    // Versiones válidas del RC522: 0x91, 0x92
    if (version == 0x91 || version == 0x92 || version == 0x88) {
        return 1;  // Conexión OK
    }
    
    // Si leemos 0x00 o 0xFF, probablemente hay problema de conexión
    if (version == 0x00 || version == 0xFF) {
        return 0;  // Sin conexión
    }
    
    // Otra versión, pero intentemos continuar
    return 1;
}

// ===================== RC522 FUNCIONES BÁSICAS =====================
uint8_t rc522_readReg(uint8_t addr) {
    uint8_t val;
    
    RC522_CS = 0;
    __delay_us(5);  // Aumentado de 2 a 5us
    spi_transfer(((addr << 1) & 0x7E) | 0x80);
    val = spi_transfer(0x00);
    __delay_us(5);
    RC522_CS = 1;
    
    return val;
}

void rc522_writeReg(uint8_t addr, uint8_t val) {
    RC522_CS = 0;
    __delay_us(5);
    spi_transfer((addr << 1) & 0x7E);
    spi_transfer(val);
    __delay_us(5);
    RC522_CS = 1;
}

void rc522_setBitMask(uint8_t reg, uint8_t mask) {
    uint8_t tmp = rc522_readReg(reg);
    rc522_writeReg(reg, tmp | mask);
}

void rc522_clearBitMask(uint8_t reg, uint8_t mask) {
    uint8_t tmp = rc522_readReg(reg);
    rc522_writeReg(reg, tmp & (~mask));
}

void rc522_antennaOn(void) {
    uint8_t temp = rc522_readReg(REG_TX_CONTROL);
    if (!(temp & 0x03)) {
        rc522_setBitMask(REG_TX_CONTROL, 0x03);
    }
}

void rc522_init(void) {
    // El RC522 ya debería estar fuera de reset del test anterior
    // pero hacemos reset de nuevo por seguridad
    
    // Hardware reset
    RC522_RST = 0;
    __delay_ms(10);
    RC522_RST = 1;
    __delay_ms(50);
    
    // Soft reset
    rc522_writeReg(REG_COMMAND, PCD_RESETPHASE);
    __delay_ms(50);
    
    // Verificar que el reset funcionó
    uint8_t reg = rc522_readReg(REG_COMMAND);
    if (DEBUG_MODE) {
        debug_showByte("Cmd", reg);
    }
    
    // Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    rc522_writeReg(REG_T_MODE, 0x8D);
    rc522_writeReg(REG_T_PRESCALER, 0x3E);
    rc522_writeReg(REG_T_RELOAD_L, 30);
    rc522_writeReg(REG_T_RELOAD_H, 0);
    
    rc522_writeReg(REG_TX_ASK, 0x40);
    rc522_writeReg(REG_MODE, 0x3D);
    
    // Activar antena
    rc522_antennaOn();
    
    __delay_ms(100);
}

// ===================== RC522 FUNCIONES DE COMUNICACIÓN =====================
uint8_t rc522_request(uint8_t reqMode, uint8_t *tagType) {
    uint8_t status;
    uint16_t backBits;
    
    rc522_writeReg(REG_BIT_FRAMING, 0x07);
    
    tagType[0] = reqMode;
    status = rc522_toCard(PCD_TRANSCEIVE, tagType, 1, tagType, &backBits);
    
    if ((status != MI_OK) || (backBits != 0x10)) {
        status = MI_ERR;
    }
    
    return status;
}

uint8_t rc522_toCard(uint8_t command, uint8_t *sendData, uint8_t sendLen,
                     uint8_t *backData, uint16_t *backLen) {
    uint8_t status = MI_ERR;
    uint8_t irqEn = 0x00;
    uint8_t waitIRq = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint16_t i;
    
    switch (command) {
        case PCD_AUTHENT:
            irqEn = 0x12;
            waitIRq = 0x10;
            break;
        case PCD_TRANSCEIVE:
            irqEn = 0x77;
            waitIRq = 0x30;
            break;
        default:
            break;
    }
    
    rc522_writeReg(REG_COM_IRQ, 0x7F);
    rc522_clearBitMask(REG_FIFO_LEVEL, 0x80);
    rc522_writeReg(REG_COMMAND, PCD_IDLE);
    
    // Escribir datos al FIFO
    for (i = 0; i < sendLen; i++) {
        rc522_writeReg(REG_FIFO_DATA, sendData[i]);
    }
    
    // Ejecutar comando
    rc522_writeReg(REG_COMMAND, command);
    if (command == PCD_TRANSCEIVE) {
        rc522_setBitMask(REG_BIT_FRAMING, 0x80);
    }
    
    // Esperar respuesta
    i = 2000;
    do {
        n = rc522_readReg(REG_COM_IRQ);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));
    
    rc522_clearBitMask(REG_BIT_FRAMING, 0x80);
    
    if (i != 0) {
        if (!(rc522_readReg(REG_ERROR) & 0x1B)) {
            status = MI_OK;
            
            if (command == PCD_TRANSCEIVE) {
                n = rc522_readReg(REG_FIFO_LEVEL);
                lastBits = rc522_readReg(REG_CONTROL) & 0x07;
                
                if (lastBits) {
                    *backLen = (n - 1) * 8 + lastBits;
                } else {
                    *backLen = n * 8;
                }
                
                if (n == 0) n = 1;
                if (n > 16) n = 16;
                
                for (i = 0; i < n; i++) {
                    backData[i] = rc522_readReg(REG_FIFO_DATA);
                }
            }
        } else {
            status = MI_ERR;
        }
    }
    
    return status;
}

uint8_t rc522_anticoll(uint8_t *serNum) {
    uint8_t status;
    uint8_t i;
    uint8_t serNumCheck = 0;
    uint16_t unLen;
    
    rc522_writeReg(REG_BIT_FRAMING, 0x00);
    
    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    
    status = rc522_toCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
    
    if (status == MI_OK) {
        // Verificar checksum
        for (i = 0; i < 4; i++) {
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i]) {
            status = MI_ERR;
        }
    }
    
    return status;
}

void rc522_calculateCRC(uint8_t *pIndata, uint8_t len, uint8_t *pOutData) {
    uint8_t i, n;
    
    rc522_clearBitMask(REG_FIFO_LEVEL, 0x80);
    
    for (i = 0; i < len; i++) {
        rc522_writeReg(REG_FIFO_DATA, *(pIndata + i));
    }
    
    rc522_writeReg(REG_COMMAND, PCD_CALCCRC);
    
    i = 0xFF;
    do {
        n = rc522_readReg(REG_FIFO_LEVEL);
        i--;
    } while ((i != 0) && !(n & 0x04));
    
    pOutData[0] = rc522_readReg(REG_CRC_RESULT_L);
    pOutData[1] = rc522_readReg(REG_CRC_RESULT_H);
}

uint8_t rc522_selectTag(uint8_t *serNum) {
    uint8_t i;
    uint8_t status;
    uint8_t size;
    uint16_t recvBits;
    uint8_t buffer[9];
    
    buffer[0] = PICC_SELECTTAG;
    buffer[1] = 0x70;
    
    for (i = 0; i < 5; i++) {
        buffer[i + 2] = *(serNum + i);
    }
    
    rc522_calculateCRC(buffer, 7, &buffer[7]);
    status = rc522_toCard(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);
    
    if ((status == MI_OK) && (recvBits == 0x18)) {
        size = buffer[0];
    } else {
        size = 0;
    }
    
    return size;
}

void rc522_halt(void) {
    uint8_t buff[4];
    uint16_t unLen;
    
    buff[0] = PICC_HALT;
    buff[1] = 0;
    rc522_calculateCRC(buff, 2, &buff[2]);
    
    rc522_toCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}

// ===================== FUNCIONES AUXILIARES =====================
void beep(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        BUZZER = 1;
        __delay_ms(100);
        BUZZER = 0;
        if (i < times - 1) {
            __delay_ms(100);
        }
    }
}

void showAccess(uint8_t granted, const char *name) {
    lcd_clear();
    
    if (granted) {
        lcd_printCenter("Acceso", 0);
        lcd_printCenter("Concedido", 1);
        __delay_ms(1000);
        lcd_clear();
        lcd_printCenter("Bienvenido:", 0);
        lcd_printCenter((char*)name, 1);
    } else {
        lcd_printCenter("Acceso", 0);
        lcd_printCenter("Denegado", 1);
        __delay_ms(1000);
        lcd_clear();
        lcd_printCenter("UID:", 0);
        lcd_setCursor(0, 1);
        lcd_print((char*)name);
    }
}

const char* findUser(const char *uid) {
    for (uint8_t i = 0; i < USER_COUNT; i++) {
        if (strcmp(userDatabase[i].uid, uid) == 0) {
            return userDatabase[i].name;
        }
    }
    return NULL;
}