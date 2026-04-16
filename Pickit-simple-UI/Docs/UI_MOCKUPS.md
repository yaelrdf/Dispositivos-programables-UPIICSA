# 🎬 MOCKUPS DE LA UI - PROGRAMADOR PIC v2.0

## ESTADO 1: INICIO - Sin archivo seleccionado

```
┌─────────────────────────────────────────────────────────────┐
│ ╔═════════════════════════════════════════════════════════╗ │
│ ║                                                         ║ │
│ ║  ⚡  Programador PIC                                   ║ │
│ ║      Interfaz intuitiva para programar dispositivos    ║ │
│ ║                                                         ║ │
│ ╚═════════════════════════════════════════════════════════╝ │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ 📁 Seleccionar Archivo                                  │
│  │ Seleccione un archivo HEX compilado                    │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐   │
│  │ │                                                   │   │
│  │ │                   📄                              │   │
│  │ │                                                   │   │
│  │ │      Ningún archivo seleccionado               │   │
│  │ │                                                   │   │
│  │ └─────────────────────────────────────────────────┘   │
│  │                                                         │
│  │  ┌─────────────────────────────────────────────────┐  │
│  │  │ Seleccionar archivo HEX                     │  │  │
│  │  └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ ✓ Estado                                              │
│  │                                                         │
│  │ ● Listo                                               │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐  │
│  │ │ Sistema listo para programar                    │  │
│  │ └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ▶ Programar Dispositivo                        │  │  │
│  │  [DESHABILITADO - Espera archivo]               │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## ESTADO 2: Archivo Seleccionado

```
┌─────────────────────────────────────────────────────────────┐
│ ╔═════════════════════════════════════════════════════════╗ │
│ ║            ⚡ Programador PIC                          ║ │
│ ║   Interfaz intuitiva para programar dispositivos       ║ │
│ ╚═════════════════════════════════════════════════════════╝ │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ 📁 Seleccionar Archivo                                  │
│  │ Seleccione un archivo HEX compilado                    │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐   │
│  │ │                                                   │   │
│  │ │                   📄                              │   │
│  │ │                                                   │   │
│  │ │      ✓ programa.hex                             │   │
│  │ │      Tamaño: 25.5 KB                            │   │
│  │ │                                                   │   │
│  │ └─────────────────────────────────────────────────┘   │
│  │                                                         │
│  │  ┌─────────────────────────────────────────────────┐  │
│  │  │ Seleccionar archivo HEX                     │  │  │
│  │  └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ ✓ Estado                                              │
│  │                                                         │
│  │ ● Listo                                               │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐  │
│  │ │ 📂 Archivo listo: programa.hex                 │  │
│  │ │                                                   │  │
│  │ │ ¡Puede comenzar a programar!                   │  │
│  │ └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ▶ Programar Dispositivo                        │  │  │
│  │  [HABILITADO - Azul gradiente]                  │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## ESTADO 3: Detectando Dispositivo

```
┌─────────────────────────────────────────────────────────────┐
│ ╔═════════════════════════════════════════════════════════╗ │
│ ║            ⚡ Programador PIC                          ║ │
│ ║   Interfaz intuitiva para programar dispositivos       ║ │
│ ╚═════════════════════════════════════════════════════════╝ │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ 📁 Seleccionar Archivo                                  │
│  │ Seleccione un archivo HEX compilado                    │
│  │                                                         │
│  │ ✓ programa.hex                        25.5 KB         │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ ✓ Estado                                              │
│  │                                                         │
│  │ ◌ Detectando...                                       │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐  │
│  │ │ ◆ Verificando programador PICkit...           │  │
│  │ └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  Detectando dispositivo...                                  │
│  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ 25%   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ▶ Programar Dispositivo                        │  │  │
│  │  [DESHABILITADO TEMPORALMENTE]                  │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ✕ Cancelar                                    │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## ESTADO 4: Programando

```
┌─────────────────────────────────────────────────────────────┐
│ ╔═════════════════════════════════════════════════════════╗ │
│ ║            ⚡ Programador PIC                          ║ │
│ ║   Interfaz intuitiva para programar dispositivos       ║ │
│ ╚═════════════════════════════════════════════════════════╝ │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ 📁 Seleccionar Archivo                                  │
│  │ Seleccione un archivo HEX compilado                    │
│  │                                                         │
│  │ ✓ programa.hex                        25.5 KB         │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ ✓ Estado                                              │
│  │                                                         │
│  │ ⟳ Programando...                                      │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐  │
│  │ │ ◆ PICkit detectado - Firmware: 2.32.00       │  │
│  │ │ ◆ Detectar dispositivo: ✓ PIC16F877A         │  │
│  │ │ ◆ Borrar dispositivo en progreso...          │  │
│  │ └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  Borrando dispositivo...                                    │
│  ▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░ 50%   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ▶ Programar Dispositivo                        │  │  │
│  │  [DESHABILITADO]                                │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ✕ Cancelar                                    │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## ESTADO 5: ¡ÉXITO!

```
┌─────────────────────────────────────────────────────────────┐
│ ╔═════════════════════════════════════════════════════════╗ │
│ ║            ⚡ Programador PIC                          ║ │
│ ║   Interfaz intuitiva para programar dispositivos       ║ │
│ ╚═════════════════════════════════════════════════════════╝ │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ 📁 Seleccionar Archivo                                  │
│  │ Seleccione un archivo HEX compilado                    │
│  │                                                         │
│  │ ✓ programa.hex                        25.5 KB         │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ ✓ Estado                                              │
│  │                                                         │
│  │ ✓ ¡Éxito!                                            │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐  │
│  │ │ ✓ Pasos completados:                            │  │
│  │ │   ✓ Detectar dispositivo                        │  │
│  │ │   ✓ Borrar dispositivo                          │  │
│  │ │   ✓ Verificar en blanco                         │  │
│  │ │   ✓ Programar                                   │  │
│  │ │   ✓ Verificar                                   │  │
│  │ └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ 100%   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ▶ Programar Dispositivo                        │  │  │
│  │  [HABILITADO]                                   │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
│  [Muestra popup de éxito con detalles]                     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## ESTADO 6: ERROR

```
┌─────────────────────────────────────────────────────────────┐
│ ╔═════════════════════════════════════════════════════════╗ │
│ ║            ⚡ Programador PIC                          ║ │
│ ║   Interfaz intuitiva para programar dispositivos       ║ │
│ ╚═════════════════════════════════════════════════════════╝ │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ 📁 Seleccionar Archivo                                  │
│  │ Seleccione un archivo HEX compilado                    │
│  │                                                         │
│  │ ✓ programa.hex                        25.5 KB         │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ═══════════════════════════════════════════════════════   │
│  │ ✓ Estado                                              │
│  │                                                         │
│  │ ✕ Error                                               │
│  │                                                         │
│  │ ┌─────────────────────────────────────────────────┐  │
│  │ │ ❌ Error: Dispositivo PIC no detectado         │  │
│  │ │                                                   │  │
│  │ │ Vea las recomendaciones arriba para resolver   │  │
│  │ │ el problema.                                     │  │
│  │ └─────────────────────────────────────────────────┘  │
│  ═══════════════════════════════════════════════════════   │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐  │
│  │  ▶ Programar Dispositivo                        │  │  │
│  │  [HABILITADO]                                   │  │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                             │
│  [Muestra popup de error con tips]                         │
│                                                             │
│  ┌──────────────────────────────────────────────────┐     │
│  │ ✕ Dispositivo PIC no detectado                  │     │
│  │                                                  │     │
│  │ El programador no pudo detectar el dispositivo  │     │
│  │ PIC en la placa objetivo.                       │     │
│  │                                                  │     │
│  │ ⚙ Detalles técnicos:                            │     │
│  │ Auto-Detect: No known part found                │     │
│  │                                                  │     │
│  │ ✎ Recomendaciones:                              │     │
│  │   1. Verifique que el PIC está insertado       │     │
│  │      correctamente                              │     │
│  │   2. Verifique las conexiones ICSP (6 pines)   │     │
│  │   3. Asegúrese de que el PIC tiene             │     │
│  │      alimentación (3.3V o 5V)                   │     │
│  │   4. Intente con una velocidad de              │     │
│  │      programación más lenta                     │     │
│  │   5. Verifique que el PIC no está protegido    │     │
│  │      contra escritura                           │     │
│  │                                                  │     │
│  │                                  [OK]           │     │
│  └──────────────────────────────────────────────────┘     │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## DIMENSIONES Y LAYOUTS

### Tamaño de Ventana

```
Óptimo:         700 × 850 pixels
Mínimo:         600 × 750 pixels
Responsivo:     Adapta a cualquier tamaño
```

### Márgenes

```
Header:         Full ancho, 90px alto
Contenido:      30px margen horizontal
                30px margen vertical
Secciones:      20px espacio entre ellas
Interior sect:  15px margen
```

### Colores por Elemento

```
Header:         Gradiente azul (#2563EB → #1E40AF)
Secciones:      Fondo gris claro (#F9FAFB)
                Borde gris (#E5E7EB)
Botón primary:  Gradiente azul
Botón error:    Rojo (#EF4444)
Indicadores:    Específico por estado
```

---

## TRANSICIONES Y ANIMACIONES

### Cambios de Estado

```
IDLE → DETECTING:      Color gris → amarillo (suave)
DETECTING → PROG:      Amarillo → azul (con spinner ⟳)
PROG → SUCCESS:        Azul → verde (✓)
PROG → ERROR:          Azul → rojo (✕)
ANY → IDLE:            Vuelta al estado inicial
```

### Hover Effects

```
Botones:       Color más claro (smooth transition)
Input fields:  Border highlight
Links:         Underline aparece
```

### Progress Bar

```
Inicialmente:  0%
Detectando:    25% → 35%
Borrando:      40% → 50%
Programando:   55% → 85%
Verificando:   90% → 100%
```

---

## TIPOGRAFÍA

```
ESCALA DE FUENTES:

Header título:      20px Bold
Títulos sección:    12px Bold
Subtítulos:         9px Regular
Texto principal:    11px Regular
Detalles/tech:      9px Regular
Error/warning:      10px
Código:             9px Monospace
```

---

## 🎨 PALETA VISUAL SIMPLIFICADA

```
┌────────────────────────────────┐
│ AZUL (Primario)               │
│ #2563EB ████████████████      │
│ uso: botones, header, acciones│
│                                │
│ VERDE (Éxito)                 │
│ #10B981 ████████████████      │
│ uso: confirmaciones            │
│                                │
│ ROJO (Error)                  │
│ #EF4444 ████████████████      │
│ uso: errores, cancelar         │
│                                │
│ AMARILLO (Espera)             │
│ #F59E0B ████████████████      │
│ uso: en progreso               │
│                                │
│ GRIS (Neutro)                 │
│ #E5E7EB ████████████████      │
│ uso: bordes, inactivo          │
└────────────────────────────────┘
```

---

## ✨ CARACTERÍSTICAS ESPECIALES

1. **Header gradiente** que captura atención
2. **Indicadores de color** únicos por estado
3. **Iconos descriptivos** en cada sección
4. **Progreso visual** con barra gradiente
5. **Botón principal** grande y destacado
6. **Mensajes de error** con tips contextuales
7. **Layout responsivo** que se adapta
8. **Espaciado consistente** en toda la UI
9. **Bordes redondeados** modernos
10. **Feedback visual** en cada acción

---

## 📱 RESPONSIVE DESIGN

```
┌─ PEQUEÑA 600x750 ─────┐
│                         │
│  [Header compacto]      │
│                         │
│  [Secciones apiladas]   │
│                         │
│  [Botones full-width]   │
│                         │
└─────────────────────────┘

┌──── NORMAL 700x850 ────┐
│                         │
│  [Header normal]        │
│                         │
│ [Secciones espaciadas]  │
│                         │
│  [Botones optimizados]  │
│                         │
└─────────────────────────┘

┌────── GRANDE 1000+ ────┐
│                         │
│  [Header grande]        │
│                         │
│ [Secciones con margen]  │
│                         │
│  [Botones proporcional] │
│                         │
└─────────────────────────┘
```

---

**Esta es la interfaz mejorada del Programador PIC v2.0** ✨

Similar a Balena Etcher con:
- ✅ Colores coherentes
- ✅ Iconografía clara
- ✅ Feedback visual
- ✅ Diseño responsivo
- ✅ Mensajes intuitivos
