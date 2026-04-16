"""
Configuración y constantes para Programador PIC
"""
import os
from pathlib import Path

# Directorio base del proyecto
BASE_DIR = Path(__file__).resolve().parent

# Configuración de UI
APP_TITLE = "Programador PIC"
APP_VERSION = "1.0.0"
WINDOW_WIDTH = 600
WINDOW_HEIGHT = 500

# Texto en español para la interfaz
UI_STRINGS = {
    # Títulos y etiquetas
    "app_title": "Programador PIC",
    "select_file": "Seleccionar archivo",
    "flash_button": "Programar",
    "select_hex_file": "Seleccionar archivo HEX",
    "file_selected": "Archivo seleccionado",
    "no_file_selected": "Sin archivo seleccionado",
    
    # Estados
    "stage_idle": "Listo",
    "stage_detecting": "Detectando dispositivo...",
    "stage_erasing": "Borrando dispositivo...",
    "stage_programming": "Programando...",
    "stage_verifying": "Verificando...",
    "stage_complete": "¡Listo!",
    "stage_error": "Error",
    
    # Mensajes de éxito
    "success_title": "Éxito",
    "success_message": "El dispositivo ha sido programado correctamente.",
    
    # Mensajes de error (ver error_handler.py para más detalle)
    "error_title": "Error",
}

# Configuración de pk2cmd
PK2CMD_EXECUTABLE = "pk2cmd.exe"
PK2CMD_SEARCH_PATHS = [
    Path.cwd(),
    Path("C:\\Program Files\\Microchip\\pk2cmd"),
    Path("C:\\Program Files (x86)\\Microchip\\pk2cmd"),
    Path.home() / "AppData" / "Local" / "Microchip" / "pk2cmd",
]

# Configuración de programación
PROGRAMMING_VOLTAGE = "5.0"  # Voltaje del dispositivo
PROGRAMMING_SPEED = "1"  # 1=fastest, 16=slowest
ENABLE_VERIFICATION = True
PRESERVE_EEPROM = False

# Configuración de timeout
DETECTION_TIMEOUT = 10  # segundos
PROGRAMMING_TIMEOUT = 60  # segundos

# Extensiones permitidas
ALLOWED_FILE_EXTENSIONS = [".hex", ".HEX"]
