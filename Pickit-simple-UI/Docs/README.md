# Programador PIC - Interfaz Balena Etcher

Una interfaz simple e intuitiva para programar dispositivos PIC usando pk2cmd.exe, inspirada en el diseño limpio de Balena Etcher.

## Características

- ✅ Interfaz moderna y fácil de usar en español
- ✅ Selección intuitiva de archivo HEX
- ✅ Detección automática de PICkit y dispositivo PIC
- ✅ Programación automática con:
  - Borrado de dispositivo
  - Verificación en blanco
  - Verificación de programación
  - Liberación de MCLR para ejecución
- ✅ Manejo de errores completo con tips y recomendaciones
- ✅ Diseño modular y extensible

## Requisitos previos

### 1. Python 3.8+
Descargue Python desde https://www.python.org/

### 2. pk2cmd.exe
Debe descargar e instalar las herramientas de Microchip:
- Descargue desde: https://www.microchip.com/en-us/development-tools/pic-and-dspic-downloads-archive
- O instale completo desde MPLAB X IDE

**Opciones para ubicar pk2cmd.exe:**
- Copie `pk2cmd.exe` a la carpeta del programa
- Instálelo en `C:\Program Files\Microchip\pk2cmd`
- Añádalo a la variable de entorno PATH del sistema

### 3. Hardware
- Programador PICkit2 o PICkit3 conectado por USB
- Placa objetivo con dispositivo PIC y conexiones ICSP:
  - Pin 1: VPP/MCLR (Rojo/Naranja)
  - Pin 2: VDD (Rojo)
  - Pin 3: VSS (Negro/GND)
  - Pin 4: PGD (Blanco/Amarillo)
  - Pin 5: PGC (Marrón/Verde)
  - Pin 6: AUX (no usado)

## Instalación

### Paso 1: Clonar o descargar el proyecto
```bash
cd Pickit-simple-UI
```

### Paso 2: Crear un ambiente virtual (recomendado)
```bash
python -m venv venv
venv\Scripts\activate
```

### Paso 3: Instalar dependencias
```bash
pip install -r requirements.txt
```

## Uso

### Ejecutar la aplicación
```bash
python main.py
```

### Pasos de uso:
1. **Conectar hardware**
   - Conecte el PICkit por USB
   - Conecte el dispositivo objetivo con cables ICSP

2. **Seleccionar archivo**
   - Haga clic en "Seleccionar archivo HEX"
   - Navegue y seleccione su archivo .hex compilado

3. **Programar**
   - Haga clic en el botón "Programar"
   - La aplicación:
     - Detecta el PICkit
     - Detecta el dispositivo PIC
     - Borra el dispositivo
     - Verifica que esté en blanco
     - Programa con verificación
     - Libera el dispositivo para ejecución

4. **Resultado**
   - Verá un mensaje de éxito o error
   - En caso de error, se mostrarán recomendaciones

## Resolución de problemas

### "pk2cmd.exe no encontrado"
- Instale pk2cmd desde Microchip
- O copie pk2cmd.exe a la carpeta del programa
- O añádalo a la variable PATH

### "PICkit no detectado"
- Verifique la conexión USB
- Intente otro puerto USB
- Reinicie el programa
- Verifique los drivers (instale desde Microchip si es necesario)

### "Dispositivo PIC no detectado"
- Verifique todas las conexiones ICSP (6 pines)
- Compruebe que el PIC tiene alimentación (3.3V o 5V)
- Intente mover el cable más cerca para reducir interferencia
- Verifique el datasheet del PIC para otros requisitos

### "Error de programación"
- Verifique que el archivo .hex es válido
- Intente programar nuevamente
- Compruebe que el dispositivo no está protegido
- Si persiste, intente con otro dispositivo PIC

## Estructura del proyecto

```
Pickit-simple-UI/
├── main.py                 # Aplicación principal con UI
├── pk2cmd_handler.py       # Wrapper para pk2cmd.exe
├── error_handler.py        # Manejo de errores y mensajes
├── config.py              # Configuración y constantes
├── requirements.txt       # Dependencias Python
└── README.md             # Este archivo
```

## Módulos

### `main.py`
Interfaz gráfica principal usando PyQt5. Define:
- `PICProgrammerUI`: Ventana principal
- `ProgrammingWorker`: Worker thread para programación no bloqueante

### `pk2cmd_handler.py`
Comunicación con pk2cmd.exe:
- `PK2CMDHandler`: Clase para ejecutar comandos pk2cmd
- Búsqueda automática de pk2cmd en el sistema
- Parsing de salida y errores
- Flujo completo de programación

### `error_handler.py`
Gestión de errores y mensajes:
- `ErrorType`: Enum con tipos de error
- `ErrorMessage`: Estructura con mensaje, tips y tipo
- Diccionario de errores con recomendaciones
- `parse_pk2cmd_error()`: Detecta errores por salida

### `config.py`
Configuración global:
- Rutas de búsqueda de pk2cmd
- Opciones de programación (voltaje, velocidad)
- Strings en español
- Timeouts y extensiones permitidas

## Personalización

### Cambiar voltaje de programación
En `config.py`:
```python
PROGRAMMING_VOLTAGE = "3.3"  # o "5.0"
```

### Cambiar velocidad de programación
En `config.py`:
```python
PROGRAMMING_SPEED = "8"  # 1=fastest, 16=slowest
```

### Desactivar verificación
En `config.py`:
```python
ENABLE_VERIFICATION = False
```

### Agregar tips personalizados
En `error_handler.py`, modifique el diccionario `ERROR_MESSAGES`.

## Licencia

Este proyecto es de uso libre.

## Soporte

Para problemas:
1. Revise la sección de "Resolución de problemas"
2. Verifique que pk2cmd.exe está instalado correctamente
3. Intente ejecutar las pruebas de conexión (ver Consola de salida)
4. Conéctese con git issues si persisten los problemas

## Tips de programación

- **Usar 5V**: Recomendado para PIC16F y PIC18F
- **Usar 3.3V**: Necesario para PIC24 y dsPIC33
- **Cable corto**: Reduzca la longitud del cable ICSP
- **Alimentación estable**: Asegure buenas conexiones de VDD y VSS
- **Pull-up en MCLR**: Algunos PICs necesitan resistor de 10kΩ en MCLR

## Comandos pk2cmd equivalentes

La aplicación ejecuta internamente:
```bash
pk2cmd -B. -P -A5.0 -E -Fprogram.hex -M -Y -L1 -T -R
```

Donde:
- `-B.`: Buscar en directorio actual
- `-P`: Detectar PIC
- `-A5.0`: Voltaje 5.0V
- `-E`: Borrar
- `-Fprogram.hex`: Archivo a programar
- `-M`: Programar memoria
- `-Y`: Verificar
- `-L1`: Velocidad máxima
- `-T`: Mantener poder encendido
- `-R`: Liberar MCLR
