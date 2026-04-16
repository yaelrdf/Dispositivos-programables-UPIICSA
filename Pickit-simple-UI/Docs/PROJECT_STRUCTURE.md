# 📋 ESTRUCTURA DEL PROYECTO - Programador PIC

## Descripción General

Interfaz moderna y simple (estilo Balena Etcher) para programar dispositivos PIC usando pk2cmd.exe, con:
- UI intuitiva en español
- Detección automática de hardware
- Manejo completo de errores con tips para el usuario
- Programación automatizada con erase y verificación
- Arquitectura modular y extensible

---

## 📁 Estructura de Archivos

```
Pickit-simple-UI/
│
├── 🐍 CÓDIGO PRINCIPAL
│   ├── main.py                    # Aplicación UI principal con PyQt5
│   ├── pk2cmd_handler.py          # Wrapper para comunicación con pk2cmd.exe
│   ├── error_handler.py           # Sistema completo de errores y mensajes
│   └── config.py                  # Configuración centralizada
│
├── 📦 DEPENDENCIAS & CONFIGURACIÓN
│   ├── requirements.txt           # Dependencias Python (PyQt5)
│   └── run.bat                    # Lanzador para Windows (doble click)
│
├── 📖 DOCUMENTACIÓN
│   ├── README.md                  # Documentación completa del proyecto
│   ├── INICIO_RAPIDO.md           # Guía de inicio rápido para nuevos usuarios
│   ├── CONEXIONES_ICSP.md         # Guía detallada de conexiones física
│   ├── PROJECT_STRUCTURE.md       # Este archivo
│   └── HOW_TO_USE.pdf             # PDF con instrucciones (existente)
│
├── 🛠️ UTILIDADES
│   ├── .gitignore                 # Archivos ignorados por Git
│   ├── pk2cmd.exe                 # Ejecutable pk2cmd (si está presente)
│   └── PK2DeviceFile.dat          # Base de datos de dispositivos pk2cmd
│
└── (+ carpetas generadas en tiempo de ejecución)
    ├── venv/                      # Ambiente virtual (si se crea)
    └── __pycache__/               # Cache de Python
```

---

## 🔧 DESCRIPCIÓN DE MÓDULOS

### `main.py` - Interfaz Principal
**Responsabilidad:** Presenta la UI y maneja interacción del usuario

**Componentes:**
- `PICProgrammerUI`: Ventana principal
  - Sección de selección de archivo
  - Sección de estado
  - Barra de progreso
  - Botón de programación
- `ProgrammingWorker`: Thread para programación no-bloqueante
  - Maneja señales de progreso
  - Ejecuta el workflow de programación
  - Comunica errores

**Características:**
- Diseño limpio tipo Balena Etcher
- 100% en español
- Colores y fuentes modernos
- Responde sin freezing durante programación

---

### `pk2cmd_handler.py` - Motor de Programación
**Responsabilidad:** Ejecutar comandos pk2cmd y gestionar el flujo

**Componentes:**
- `PK2CMDHandler`: Clase principal
  - Busca pk2cmd.exe automáticamente
  - Ejecuta comandos con captura de salida
  - Parsea errores
  - Implementa el flujo completo

**Métodos Principales:**
```python
is_available()                    # Verifica disponibilidad
detect_pickit()                   # Detecta programador
detect_pic_device()               # Detecta PIC objetivo
erase_device()                    # Borra el dispositivo
blank_check()                     # Verifica si está en blanco
program_device(hex_file)          # Programa
complete_programming_workflow()   # Ejecuta todo
power_on_and_release_mclr()       # Libera para ejecución
```

**Secuencia de Programación:**
1. Detecta PICkit
2. Detecta PIC
3. Borra dispositivo
4. Verifica en blanco
5. Programa con verificación
6. Libera MCLR

---

### `error_handler.py` - Sistema de Errores
**Responsabilidad:** Proporcionar mensajes de error completos con tips

**Componentes:**
- `ErrorType`: Enum de 15+ tipos de error
- `ErrorMessage`: Estructura con título, mensaje, y tips
- `ERROR_MESSAGES`: Diccionario completo en español

**Tipos de Error:**
```
• FILE_NOT_FOUND, FILE_INVALID, FILE_NOT_SELECTED
• PK2CMD_NOT_FOUND, PK2CMD_EXECUTION_FAILED
• PICKIT_NOT_DETECTED, PIC_NOT_DETECTED
• COMMUNICATION_ERROR, PROGRAMMING_FAILED
• VERIFICATION_FAILED, ERASE_FAILED
• BLANK_CHECK_FAILED, VOLTAGE_ERROR
• CONNECTION_ERROR, TIMEOUT_ERROR
• UNKNOWN_ERROR
```

**Cada error incluye:**
- ✅ Título claro
- 📝 Descripción de qué salió mal
- 💡 3-5 tips para resolver
- 🏷️ Tipo para clasificación

**Ejemplo:**
```python
ErrorType.PICKIT_NOT_DETECTED → 
  "Programador PICkit no detectado"
  Tips:
  - Conecte el programador por USB
  - Espere 2-3 segundos
  - Intente otro puerto USB
  - Verifique drivers
  - Reinstale los drivers si es necesario
```

---

### `config.py` - Configuración Centralizada
**Responsabilidad:** Centralizar todas las constantes

**Secciones:**
- **UI Strings**: Todos los textos en español
- **PK2CMD Config**: Rutas de búsqueda, opciones
- **Programming Settings**: Voltaje, velocidad, verificación
- **Timeouts**: Límites de tiempo
- **File Extensions**: Extensiones permitidas

**Customización Fácil:**
```python
PROGRAMMING_VOLTAGE = "5.0"      # Cambiar a 3.3 si es necesario
PROGRAMMING_SPEED = "1"          # 1=rápido, 16=lento
ENABLE_VERIFICATION = True       # Desactivar si es necesario
PRESERVE_EEPROM = False          # Preservar datos EEPROM
```

---

## 📚 DOCUMENTACIÓN DISPONIBLE

### Para Usuarios Nuevos
1. **INICIO_RAPIDO.md** ← Empezar aquí
   - Instalación paso a paso
   - Requisitos mínimos
   - Primera ejecución

### Para Configuración
2. **CONEXIONES_ICSP.md**
   - Diagramas de conexión
   - Pinout del PICkit
   - Ejemplos específicos
   - Troubleshooting de hardware

### Referencia Completa
3. **README.md**
   - Características
   - Instalación completa
   - Uso detallado
   - Personalización
   - Tips de programación

---

## 🚀 FLUJO DE EJECUCIÓN

```
┌─────────────────────┐
│  python main.py     │
└──────────┬──────────┘
           │
           ▼
    ┌──────────────────┐
    │ PICProgrammerUI  │ ← Crea la ventana
    │  __init__()      │   Carga config
    └────────┬─────────┘   Verifica pk2cmd
             │
    ┌────────▼────────────┐
    │  Usuario selecciona │
    │  archivo .hex       │
    └────────┬────────────┘
             │
    ┌────────▼────────────┐
    │  Usuario hace click │
    │  en "Programar"     │
    └────────┬────────────┘
             │
    ┌────────▼──────────────────┐
    │ ProgrammingWorker (thread)│
    │  run() → workflow()       │
    └────────┬──────────────────┘
             │
    ┌────────▼───────────────┐
    │ pk2cmd_handler         │
    │ 1. Detectar PICkit    │
    │ 2. Detectar PIC       │
    │ 3. Borrar             │
    │ 4. Blank check        │
    │ 5. Programar          │
    │ 6. Liberar MCLR       │
    └────────┬───────────────┘
             │
    ┌────────▼─────────────┐
    │  Éxito o Error       │
    │  Mostrar resultado   │
    └──────────────────────┘
```

---

## 🔄 CICLO DE PROGRAMACIÓN

El programa ejecuta internamente:

```bash
# 1. Detectar PICkit
pk2cmd -B. -S#

# 2. Detectar y obtener info del PIC
pk2cmd -B. -P -I

# 3. Borrar, programar y verificar (en un solo comando)
pk2cmd -B. -P -A5.0 -E -Fprogram.hex -M -Y -L1 -T -R

Donde:
-B.       = Buscar en directorio actual
-P        = Auto-detectar PIC
-A5.0     = Voltaje 5.0V
-E        = Erase (borrar)
-Fprogram.hex = Archivo a programar
-M        = Program memory (programar)
-Y        = Verificar programación
-L1       = Velocidad rápida
-T        = Mantener power
-R        = Release MCLR
```

---

## 🛡️ MANEJO DE ERRORES

Para cada error, el sistema:

1. **Clasifica** el error por tipo
2. **Obtiene** el mensaje en español
3. **Genera tips** específicos
4. **Muestra** al usuario con opciones

```python
Ejemplo: Si falla el "Auto-detect"
├─ Detecta: "No known part found"
├─ Clasifica: ErrorType.PIC_NOT_DETECTED  
├─ Obtiene mensaje con 8 tips
└─ Muestra: Diálogo con title + message + tips
```

---

## 💡 EXTENSIONES POSIBLES

El código es modular, puedes fácilmente:

1. **Agregar más tipos de error**
   - Editar `error_handler.py`

2. **Cambiar voltaje/velocidad**
   - Editar `config.py`

3. **Customizar UI**
   - Editar `main.py` estilos CSS

4. **Agregar features**
   - Crear new methods en `pk2cmd_handler.py`

5. **Cambiar idioma**
   - Cambiar `UI_STRINGS` en `config.py`

---

## 📊 DEPENDENCIAS

```
PyQt5 (5.15.9)          # Framework UI
└─ PyQt5-sip (12.13.0)  # SIP binding

+ Sistema:
  └─ pk2cmd.exe         # Ejecutable Microchip
  └─ Python 3.8+        # Runtime
```

---

## ✅ CHECKLIST DE INSTALACIÓN

- [ ] Python 3.8+ instalado
- [ ] pip install -r requirements.txt ejecutado
- [ ] pk2cmd.exe disponible en sistema
- [ ] PICkit conectado por USB
- [ ] Archivo .hex compilado y listo
- [ ] Verificar conexiones ICSP (ver CONEXIONES_ICSP.md)
- [ ] Ejecutar: python main.py o double-click run.bat

---

## 🎯 PRÓXIMAS FUNCIONALIDADES (Sugerencias)

1. Histórico de programaciones
2. Backup automático del dispositivo antes de programar
3. Soporte para múltiples tipos de dispositivos
4. Estadísticas de programación
5. Interfaz dark mode
6. Guardado de preferencias de archivo

---

## 📞 SOPORTE

Si encuentras problemas:

1. Revisa **INICIO_RAPIDO.md** para setup
2. Revisa **README.md** sección "Resolución de problemas"
3. Revisa los tips mostrados en los mensajes de error
4. Verifica hardware con **CONEXIONES_ICSP.md**

---

**Versión:** 1.0.0  
**Última actualización:** 2026  
**Autor:** Desarrollador  
**Licencia:** Libre
