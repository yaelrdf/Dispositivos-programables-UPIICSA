# 🔐 Sistema de Control de Acceso RFID - Guía Completa

## 📋 Tabla de Contenidos
1. [Hardware Necesario](#hardware-necesario)
2. [Conexiones](#conexiones)
3. [Configuración del Software](#configuración-del-software)
4. [Cómo Usar](#cómo-usar)
5. [Monitor Serial](#monitor-serial)
6. [Solución de Problemas](#solución-de-problemas)

---

## 🛠️ Hardware Necesario

### Componentes Principales
- **PIC18F4550** con cristal de 20MHz
- **LCD 16x2** (compatible con HD44780)
- **Módulo RC522 RFID** (versión SPI)
- **Tarjetas/Tags RFID** (13.56MHz compatible con RC522)

### Componentes Adicionales
- **LEDs**: 1 verde, 1 rojo
- **Buzzer** activo (5V)
- **Resistencias**: 
  - 2x 220Ω (para LEDs)
  - 1x 330Ω (para buzzer, opcional)
  - 1x 10kΩ potenciómetro (contraste LCD)
- **Regulador 3.3V** (AMS1117-3.3 o similar) para RC522
- **Cable USB** para programación y comunicación serial

### ⚠️ IMPORTANTE - Alimentación
El **RC522 opera a 3.3V**, NO conectes directamente a 5V. Usa un regulador de voltaje.

---

## 🔌 Conexiones

### 1️⃣ LCD (PORTD) → 5V
```
LCD Pin  →  PIC Pin
─────────────────────
RS       →  RD0
RW       →  RD1  (o a GND si solo escribes)
EN       →  RD2
D4       →  RD4
D5       →  RD5
D6       →  RD6
D7       →  RD7
VSS      →  GND
VDD      →  +5V
V0       →  Potenciómetro (contraste)
A        →  +5V con resistencia 220Ω
K        →  GND
```

### 2️⃣ RC522 RFID (SPI) → 3.3V ⚠️
```
RC522    →  PIC Pin  →  Nivel
──────────────────────────────
SDA      →  RC7      →  3.3V (usar level shifter o resistor divider)
SCK      →  RB1      →  3.3V
MISO     →  RB2      →  3.3V (directo OK, es entrada)
SS       →  RB3      →  3.3V (usar level shifter)
RST      →  RB0      →  3.3V (usar level shifter)
3.3V     →  Regulador 3.3V
GND      →  GND común
IRQ      →  No conectar
```

**Level Shifter Simple (Resistor Divider):**
```
PIC 5V ──┬── 2kΩ ──┬── RC522 3.3V
         │         │
       GND      3kΩ
                  │
                GND
```

### 3️⃣ Indicadores
```
Componente  →  PIC Pin  →  Conexión
────────────────────────────────────
LED Verde   →  RB5      →  PIC → 220Ω → LED+ → GND
LED Rojo    →  RB6      →  PIC → 220Ω → LED+ → GND
Buzzer      →  RB4      →  PIC → Buzzer+ → GND
```

### 4️⃣ USB (Comunicación Serial)
```
USB Cable  →  PIC Pin
──────────────────────
D+         →  RC5
D-         →  RC4
GND        →  GND
```

**Alternativa UART (si no usas USB):**
```
FTDI/CH340 →  PIC Pin
──────────────────────
TX         →  RC7 (RX PIC)
RX         →  RC6 (TX PIC)
GND        →  GND
```

---

## 💻 Configuración del Software

### Estructura de Archivos del Proyecto
```
Proyecto_RFID/
│
├── main.c          ← Código principal
├── users.h         ← Base de datos de usuarios (EDITABLE)
├── lcd.c           ← Tu librería LCD
└── lcd.h           ← Header de LCD
```

### Pasos de Configuración

#### 1. **Instalar MPLAB X IDE y XC8 Compiler**
- Descarga desde: [microchip.com](https://www.microchip.com)
- Instala ambos programas

#### 2. **Crear Nuevo Proyecto**
1. File → New Project
2. Microchip Embedded → Standalone Project
3. Device: **PIC18F4550**
4. Tool: Tu programador (PICkit3, ICD3, etc.)
5. Compiler: XC8

#### 3. **Agregar Archivos**
1. Copia `main.c`, `users.h`, `lcd.c`, `lcd.h` a la carpeta del proyecto
2. En MPLAB: Right-click Source Files → Add Existing Item
3. Agrega todos los archivos `.c`
4. Right-click Header Files → Add Existing Item  
5. Agrega todos los archivos `.h`

#### 4. **Verificar Configuración**
En `main.c`, confirma:
```c
#define _XTAL_FREQ 48000000  // Debe coincidir con tu cristal + PLL
```

---

## 🚀 Cómo Usar

### Primera Vez - Obtener UIDs

1. **Compila y carga el programa** al PIC18F4550

2. **Abre el Monitor Serial:**
   - MPLAB X: Tools → Configure Plugins → Available Plugins → Data Visualizer
   - O usa cualquier terminal serial (PuTTY, Termite, Arduino IDE Serial Monitor)
   - Configuración: **9600 baud, 8N1**

3. **Coloca una tarjeta RFID** en el lector

4. **Observa la salida:**
   ```
   >>> Tarjeta detectada
   UID: A1B2C3D4
   Estado: ACCESO DENEGADO - UID no registrado
   ```

5. **Copia el UID** mostrado

6. **Abre `users.h`** y agrega el usuario:
   ```c
   {"A1B2C3D4",   "Nombre Usuario"},
   ```

7. **Actualiza `USER_COUNT`:**
   ```c
   #define USER_COUNT 11  // Si agregaste un usuario
   ```

8. **Recompila y carga** el programa

9. **Prueba la tarjeta** - ahora debería dar acceso

### Operación Normal

#### Pantalla LCD Estados:

**Idle (Esperando):**
```
  Control de
     Acceso
```

**Acceso Concedido:**
```
     Acceso
   Concedido
  ─────────────
   Bienvenido:
   Juan Perez
```
- ✅ LED verde ON
- 🔊 1 beep corto

**Acceso Denegado:**
```
     Acceso
    Denegado
  ─────────────
      UID:
    A1B2C3D4
```
- ❌ LED rojo ON
- 🔊 3 beeps cortos

---

## 📊 Monitor Serial

### Mensajes del Sistema

#### Inicio:
```
=================================
Sistema Control de Acceso v1.0
=================================
Inicializando RC522... OK
Usuarios cargados: 10
Sistema listo. Esperando tarjetas...
```

#### Tarjeta Detectada - ACCESO:
```
>>> Tarjeta detectada
UID: A1B2C3D4
Estado: ACCESO CONCEDIDO - Usuario: Juan Perez
[LOG] ACCESO | UID: A1B2C3D4 | Usuario: Juan Perez
Esperando siguiente tarjeta...
```

#### Tarjeta Detectada - DENEGADO:
```
>>> Tarjeta detectada
UID: DEADBEEF
Estado: ACCESO DENEGADO - UID no registrado
[LOG] DENEGADO | UID: DEADBEEF | Usuario: Desconocido
Esperando siguiente tarjeta...
```

#### Errores:
```
Error: Fallo en anticolisión
```

---

## 🔧 Solución de Problemas

### ❌ LCD no muestra nada

**Causas:**
1. Contraste mal ajustado → Gira el potenciómetro
2. Alimentación incorrecta → Verifica 5V en VDD y GND en VSS
3. Pines mal conectados → Revisa conexiones PORTD
4. `ADCON1` no configurado → Debe ser `0x0F` (todo digital)

**Solución:**
```c
ADCON1 = 0x0F;  // En system_init()
```

### ❌ RC522 no detecta tarjetas

**Causas:**
1. **Voltaje incorrecto** → RC522 necesita 3.3V
2. Antena dañada → Revisa visualmente el módulo
3. Conexiones SPI incorrectas → Verifica MOSI, MISO, SCK, SS
4. Tarjetas incompatibles → Usa tarjetas 13.56MHz (Mifare Classic, NTAG, etc.)

**Diagnóstico:**
- LED en RC522 debe encender (indica alimentación)
- Coloca tarjeta muy cerca (< 3cm)
- Prueba con diferentes tarjetas

### ❌ Monitor Serial no muestra nada

**Causas:**
1. Cable USB no conectado
2. Baud rate incorrecto → Debe ser 9600
3. Driver USB no instalado
4. Puerto COM incorrecto

**Solución:**
1. Verifica Device Manager (Windows) → Ports (COM & LPT)
2. Configura terminal: 9600, 8, N, 1, sin control de flujo
3. Prueba con otro cable USB

### ❌ Caracteres extraños en Serial

**Causa:** Baud rate incorrecto

**Solución:**
Verifica en `usb_init()`:
```c
// Para 48MHz y 9600 baud:
SPBRGH = 0x04;
SPBRG = 0xE1;
```

### ❌ Acceso siempre denegado

**Causas:**
1. UID en `users.h` no coincide
2. UID debe ser MAYÚSCULAS
3. Formato incorrecto

**Solución:**
1. Lee el UID del monitor serial
2. Cópialo EXACTAMENTE como aparece
3. Verifica que tenga 8 caracteres
4. Debe ser mayúsculas: `"A1B2C3D4"` ✅ NO `"a1b2c3d4"` ❌

### ❌ Sistema se congela

**Causas:**
1. `_XTAL_FREQ` incorrecto
2. Watchdog Timer activo
3. Configuración de fusibles incorrecta

**Solución:**
Verifica fusibles:
```c
#pragma config WDT = OFF        // Watchdog desactivado
#pragma config _XTAL_FREQ 48000000
```

---

## 📝 Ejemplo Completo - Agregar Usuario

### Paso a Paso:

1. **Usuario tiene tarjeta nueva con UID: `CAFE1234`**

2. **Coloca tarjeta** → Monitor serial muestra:
   ```
   >>> Tarjeta detectada
   UID: CAFE1234
   Estado: ACCESO DENEGADO
   ```

3. **Edita `users.h`:**
   ```c
   #define USER_COUNT 11  // Era 10, ahora 11
   
   const UserEntry userDatabase[USER_COUNT] = {
       {"A1B2C3D4",   "Juan Perez"},
       {"E5F6A7B8",   "Maria Lopez"},
       // ... usuarios existentes ...
       {"CAFE1234",   "Nuevo Usuario"}  // ← AGREGAR AQUÍ
   };
   ```

4. **Guarda el archivo**

5. **Recompila:** Production → Clean and Build

6. **Carga al PIC:** Production → Make and Program Device

7. **Prueba:** Coloca la tarjeta → ¡Acceso concedido!

---

## 🎯 Consejos y Mejores Prácticas

### ✅ Recomendaciones

1. **Documenta tus tarjetas:**
   - Marca físicamente cada tarjeta
   - Mantén una tabla: UID ↔ Tarjeta física ↔ Usuario

2. **Backup regular:**
   - Guarda `users.h` en un lugar seguro
   - Versiona con Git si es posible

3. **Tarjetas de respaldo:**
   - Programa 2-3 tarjetas "master" de emergencia

4. **Testing:**
   - Prueba TODAS las tarjetas después de agregar usuarios
   - Verifica los LEDs y buzzer

5. **Seguridad:**
   - No compartas los UIDs públicamente
   - Tarjetas RFID básicas son clonables
   - Para alta seguridad, considera cifrado adicional

### ⚡ Optimizaciones Futuras

- Agregar reloj de tiempo real (RTC) para log con timestamp
- Guardar historial en EEPROM externa
- Pantalla OLED para mejor visualización
- Modo de programación sin recompilar (EEPROM)
- Comunicación Wi-Fi (con ESP8266/ESP32)
- Base de datos remota
- Teclado para PIN adicional

---

## 📞 Soporte

### Recursos Útiles
- Datasheet PIC18F4550: [microchip.com](https://www.microchip.com)
- Datasheet RC522: Buscar "MFRC522 datasheet"
- Foro Microchip: [microchip.com/forums](https://www.microchip.com/forums)

### Debugging
Si tienes problemas, recopila esta información:
- Versión de MPLAB X
- Versión de XC8 Compiler
- Voltajes medidos (5V y 3.3V)
- Mensajes del monitor serial
- Foto del circuito (si es posible)

---

## ⚖️ Licencia

Este código es de código abierto y puede ser utilizado libremente en proyectos personales y comerciales.

**Autor:** Sistema RFID PIC18F4550  
**Versión:** 1.0  
**Fecha:** 2024

---

¡Buena suerte con tu proyecto! 🚀