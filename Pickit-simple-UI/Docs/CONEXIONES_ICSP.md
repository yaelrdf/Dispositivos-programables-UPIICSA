# Guía de Conexiones ICSP - Programador PIC

## Conector PICkit (6-pin ICSP)

Mirando el conector desde el extremo del programador:

```
╔═════════════════════╗
║  1   2   3   4   5  6 │
╚═════════════════════╝
```

### Pinout Estándar

```
Pin 1: VPP/MCLR → Voltaje de programación / Reset
Pin 2: VDD      → Poder positivo (+3.3V o +5V)
Pin 3: VSS      → Tierra (GND)
Pin 4: PGD      → Datos de programación
Pin 5: PGC      → Clock de programación
Pin 6: AUX      → Auxiliar (no usado en programación básica)
```

### Colores de Cables Típicos

```
Pin 1 → Rojo/Naranja    (VPP/MCLR)
Pin 2 → Rojo            (VDD)
Pin 3 → Negro           (VSS/GND)
Pin 4 → Blanco/Amarillo (PGD)
Pin 5 → Marrón/Verde    (PGC)
Pin 6 → Gris/Morado     (AUX)
```

## Conexión a la Placa Objetivo

### Dispositivo PIC Típico (DIP-8 a DIP-40)

Consulte el datasheet de su PIC específico para identificar:

```
Búsqueda típica en datasheet:
- MCLR/VPP (Master Clear / Programming Voltage)
- VSS (Ground)
- VDD (Power Supply)
- RA4/RB6/RC6 (varía por PIC - dados de programación)
- RA5/RB7/RC7 (varía por PIC - clock de programación)
```

### Ejemplo: PIC16F877A (40 pines DIP)

```
        ┌─────────────\─────────────┐
        │           40-pin DIP         │
    MCLR→│ 1                       40  │←VDD
    RA0 │ 2                       39  │ RA7
    RA1 │ 3                       38  │ RA6
    RA2 │ 4                       37  │ RA5 (PGM)
    RA3 │ 5                       36  │ RA4
    RA4 │ 6                       35  │ RC7 (PGC - Clock)
    RA5 │ 7                       34  │ RC6 (PGD - Data)
    RA6 │ 8                       33  │ RC5
    RA7 │ 9                       32  │ RC4
GND(VSS)→│ 10                      31  │ RC3
    ...
```

**Para PIC16F877A:**
- Pin 1 → MCLR (a Pin 1 de PICkit)
- Pin 12 → VSS/GND (a Pin 3 de PICkit)
- Pin 40 → VDD (a Pin 2 de PICkit)
- Pin 34 → RC6/PGD (a Pin 4 de PICkit)
- Pin 35 → RC7/PGC (a Pin 5 de PICkit)

## Resistencias Recomendadas

### MCLR Pull-up (opcional, pero recomendado)

```
     +5V
      │
      R (10kΩ)
      │
     ─┴─────→ MCLR del PIC
```

Algunos PICs requieren una resistencia pull-up de 10kΩ entre VDD y MCLR.

### Capacitor de Desacoplamiento (recomendado)

```
         VDD
          │
          ├─────────→ VDD del PIC
          │
         ===  100nF (0.1µF)
          │
         GND ────────→ GND del PIC
```

Agregue un capacitor de 100nF (0.1µF) entre VDD y GND proche al PIC.

## Opciones de Alimentación

### 1. Power desde PICkit (Recomendado para prototipado)

El PICkit proporciona power a través del Pin 2. 
- Conexión simple
- Voltaje limitado (~250mA típicamente)

```
PICkit Pin 2 (VDD) → VDD de la placa
PICkit Pin 3 (GND) → GND de la placa
```

### 2. Power Externo (Recomendado para circuitos reales)

Use la fuente de poder de la placa objetivo:

```
PICkit Pin 3 (GND) → GND de la placa (COMÚN)
Fuente externa      → VDD de la placa

⚠️ IMPORTANTE: GND debe ser COMÚN entre PICkit y la placa
```

## Verificación de Conexiones

Antes de programar, verifique:

- [ ] **MCLR**: Conectado a Pin 1 del PICkit
- [ ] **VDD**: Conectado a Pin 2 del PICkit (o fuente externa conectada)
- [ ] **VSS/GND**: Conectado a Pin 3 del PICkit (TIERRA COMÚN)
- [ ] **PGD**: Conectado a Pin 4 del PICkit (correcto PIC pin)
- [ ] **PGC**: Conectado a Pin 5 del PICkit (correcto PIC pin)
- [ ] **Alimentación**: El PIC está recibiendo power (ver LED o verificar con multímetro)
- [ ] **Resistencias**: MCLR pull-up está presente si es requerida
- [ ] **Capacitores**: Desacoplamiento presente

## Solución de Problemas de Conexión

### "Dispositivo PIC no detectado"

1. **Verificar alimentación**
   - Use multímetro: VDD debe ser 3.3V o 5.0V
   - GND debe ser 0V (referencia)

2. **Verificar conexiones ICSP**
   ```bash
   # Con multímetro en continuidad
   - Mida entre cada pin del PICkit y el PIC
   - No debe haber cortes/rupturas
   - Cables no deben estar dañados
   ```

3. **Verificar resistencias y capacitores**
   - Compruebe visualmente por soldaduras frías
   - Use multímetro para verificar valores

4. **Verificar PIC y socket**
   - PIC está completamente insertado en socket
   - Socket no está dañado
   - Pines no están doblados

### "Verificación falla" después de programar

1. Verifique el datasheet del PIC para **Configuration Bits**
   - Algunos PICs necesitan bits específicos configurados
   - Asegúrese que el .hex compila con configuración correcta

2. Intente baja velocidad de programación:
   - En config.py: `PROGRAMMING_SPEED = "8"`

## References

- **PIC16F Series**: Consulte datasheet en https://www.microchip.com/
- **PIC18F Series**: Consulte datasheet específico 
- **PICkit Manual**: Con el software de Microchip
- **MPLAB X**: Documentación incluida en instalación

## Diagrama General de Programación

```
┌──────────────┐
│   Computadora│
│   (USB)      │
└────────┬─────┘
         │
    ┌────▼──────┐
    │  PICkit 2/3│
    │ (6 pines)  │
    └────┬───────┘
         │ ICSP (6 cables)
         │
    ┌────▼──────────┐
    │ Placa Objetivo │
    │   (con PIC)    │
    │                │
    │  ┌──────────┐  │
    │  │   PIC    │  │
    │  │ 40-pin   │  │
    │  │   DIP    │  │
    │  └──────────┘  │
    │                │
    │  Decoupling:   │
    │  C1: 100nF     │
    │  R1: 10k MCLR  │
    └────────────────┘
```

## Checklist Final

```
□ Python 3.8+ instalado
□ PyQt5 instalado (pip install PyQt5)
□ pk2cmd.exe disponible
□ PICkit conectado y detectado
□ Archivo .hex compilado
□ Conexiones ICSP verificadas
□ Voltaje correcto (3.3V o 5.0V)
□ Tierra común entre PICkit y placa
□ Ready to program! 🚀
```
