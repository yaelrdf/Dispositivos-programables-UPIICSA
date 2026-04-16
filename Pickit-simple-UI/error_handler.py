"""
Gestor de errores y mensajes para Programador PIC
"""
from enum import Enum
from dataclasses import dataclass
from typing import Optional


class ErrorType(Enum):
    """Tipos de errores posibles"""
    # Errores de archivo
    FILE_NOT_FOUND = "file_not_found"
    FILE_INVALID = "file_invalid"
    FILE_NOT_SELECTED = "file_not_selected"
    
    # Errores de pk2cmd
    PK2CMD_NOT_FOUND = "pk2cmd_not_found"
    PK2CMD_EXECUTION_FAILED = "pk2cmd_execution_failed"
    
    # Errores de hardware
    PICKIT_NOT_DETECTED = "pickit_not_detected"
    PIC_NOT_DETECTED = "pic_not_detected"
    COMMUNICATION_ERROR = "communication_error"
    
    # Errores de programación
    PROGRAMMING_FAILED = "programming_failed"
    VERIFICATION_FAILED = "verification_failed"
    ERASE_FAILED = "erase_failed"
    BLANK_CHECK_FAILED = "blank_check_failed"
    
    # Errores de configuración
    VOLTAGE_ERROR = "voltage_error"
    CONNECTION_ERROR = "connection_error"
    
    # Otros
    UNKNOWN_ERROR = "unknown_error"
    TIMEOUT_ERROR = "timeout_error"


@dataclass
class ErrorMessage:
    """Estructura para mensajes de error"""
    title: str
    message: str
    tips: list
    error_type: ErrorType


# Diccionario de errores con mensajes y tips en español
ERROR_MESSAGES = {
    ErrorType.FILE_NOT_FOUND: ErrorMessage(
        title="Archivo no encontrado",
        message="El archivo HEX seleccionado no existe o fue movido.",
        tips=[
            "Verifique que el archivo sigue en su ubicación original",
        ],
        error_type=ErrorType.FILE_NOT_FOUND,
    ),
    
    ErrorType.FILE_INVALID: ErrorMessage(
        title="Archivo HEX inválido",
        message="El archivo seleccionado no es un archivo HEX válido.",
        tips=[
            "Asegúrese de que el archivo tiene extensión .hex",
            "Intente recompilar el código en su IDE",
        ],
        error_type=ErrorType.FILE_INVALID,
    ),
    
    ErrorType.FILE_NOT_SELECTED: ErrorMessage(
        title="Archivo no seleccionado",
        message="Por favor, seleccione un archivo HEX antes de programar.",
        tips=[
            "Haga clic en 'Seleccionar archivo' para elegir un archivo HEX",
            "Asegúrese de que el archivo sea válido",
        ],
        error_type=ErrorType.FILE_NOT_SELECTED,
    ),
    
    ErrorType.PK2CMD_NOT_FOUND: ErrorMessage(
        title="pk2cmd.exe no encontrado",
        message="No se pudo localizar el ejecutable pk2cmd.exe en el sistema.",
        tips=[
            "Asegúrese de que pk2cmd.exe está en la carpeta del programa o en una ruta accesible",
        ],
        error_type=ErrorType.PK2CMD_NOT_FOUND,
    ),
    
    ErrorType.PK2CMD_EXECUTION_FAILED: ErrorMessage(
        title="Error ejecutando pk2cmd.exe",
        message="No se pudo ejecutar el comando pk2cmd.exe correctamente.",
        tips=[
            "Verifique que pk2cmd.exe existe en la carpeta del programa",
            "Intente ejecutar el programa como administrador",
        ],
        error_type=ErrorType.PK2CMD_EXECUTION_FAILED,
    ),
    
    ErrorType.PICKIT_NOT_DETECTED: ErrorMessage(
        title="Programador PICkit no detectado",
        message="Por favor, verifique que el programador PICkit está conectado al USB.",
        tips=[
            "Conecte el programador PICkit por USB",
            "Intente conectar a otro puerto USB",
        ],
        error_type=ErrorType.PICKIT_NOT_DETECTED,
    ),
    
    ErrorType.PIC_NOT_DETECTED: ErrorMessage(
        title="Dispositivo PIC no detectado",
        message="El programador no pudo detectar el dispositivo PIC en la placa objetivo.",
        tips=[
            "Verifique que el PIC está insertado correctamente",
        ],
        error_type=ErrorType.PIC_NOT_DETECTED,
    ),
    
    ErrorType.COMMUNICATION_ERROR: ErrorMessage(
        title="Error de comunicación",
        message="Falló la comunicación con el dispositivo PIC.",
        tips=[
            "Verifique todas las conexiones físicas",
            "Intente desconectar y conectar nuevamente",
            "Intente en otro puerto USB",
        ],
        error_type=ErrorType.COMMUNICATION_ERROR,
    ),
    
    ErrorType.PROGRAMMING_FAILED: ErrorMessage(
        title="Error durante la programación",
        message="No se pudo escribir el código en el dispositivo PIC.",
        tips=[
            "Asegúrese de que el archivo HEX es compatible con el PIC",
        ],
        error_type=ErrorType.PROGRAMMING_FAILED,
    ),
    
    ErrorType.VERIFICATION_FAILED: ErrorMessage(
        title="Error en la verificación",
        message="El contenido programado no coincide con el archivo HEX.",
        tips=[
            "El dispositivo podría estar defectuoso",
            "Intente programar de nuevo",
            "Si el error persiste, reemplace el dispositivo PIC",
        ],
        error_type=ErrorType.VERIFICATION_FAILED,
    ),
    
    ErrorType.ERASE_FAILED: ErrorMessage(
        title="Error al borrar dispositivo",
        message="No se pudo borrar el dispositivo PIC.",
        tips=[
            "Verifique las conexiones de alimentación",
            "Compruebe que el MCLR está correctamente conectado",
            "Intente desconectar y reconectar el programador",
        ],
        error_type=ErrorType.ERASE_FAILED,
    ),
    
    ErrorType.BLANK_CHECK_FAILED: ErrorMessage(
        title="Error verificando dispositivo en blanco",
        message="El dispositivo no está completamente borrado.",
        tips=[
            "Intente borrar el dispositivo nuevamente",
            "Si el problema persiste, podría estar protegido",
            "Consulte el datasheet del PIC para opciones de protección",
        ],
        error_type=ErrorType.BLANK_CHECK_FAILED,
    ),
    
    ErrorType.VOLTAGE_ERROR: ErrorMessage(
        title="Error de voltaje",
        message="El voltaje del dispositivo no es válido.",
        tips=[
            "Verifique la alimentación del dispositivo PIC",
        ],
        error_type=ErrorType.VOLTAGE_ERROR,
    ),
    
    ErrorType.CONNECTION_ERROR: ErrorMessage(
        title="Error de conexión",
        message="No se pudo establecer conexión con el dispositivo.",
        tips=[
            "Verifique todas las conexiones físicas",
            "Intente desconectar y conectar nuevamente",
            "Intente en otro puerto USB",
        ],
        error_type=ErrorType.CONNECTION_ERROR,
    ),
    
    ErrorType.TIMEOUT_ERROR: ErrorMessage(
        title="Tiempo de espera agotado",
        message="La operación tardó demasiado tiempo en completarse.",
        tips=[
            "Intente de nuevo",
            "Si el problema persiste, reinicie el programa",
        ],
        error_type=ErrorType.TIMEOUT_ERROR,
    ),
    
    ErrorType.UNKNOWN_ERROR: ErrorMessage(
        title="Error desconocido",
        message="Ocurrió un error inesperado durante la programación.",
        tips=[
            "Intente de nuevo",
            "Si el problema persiste, reinicie el programa",
            "Verifique que todos los dispositivos están conectados correctamente",
        ],
        error_type=ErrorType.UNKNOWN_ERROR,
    ),
}


def get_error_message(error_type: ErrorType) -> ErrorMessage:
    """
    Obtiene el mensaje de error para un tipo de error específico
    
    Args:
        error_type: Tipo de error
        
    Returns:
        ErrorMessage con título, mensaje y tips
    """
    return ERROR_MESSAGES.get(error_type, ERROR_MESSAGES[ErrorType.UNKNOWN_ERROR])


def parse_pk2cmd_error(output: str) -> Optional[ErrorType]:
    """
    Analiza la salida de pk2cmd para identificar tipos de error
    
    Args:
        output: Salida del comando pk2cmd
        
    Returns:
        ErrorType identificado o None
    """
    output_lower = output.lower()
    
    if "no known part found" in output_lower or "auto-detect: no known part" in output_lower:
        return ErrorType.PIC_NOT_DETECTED
    
    if "no target power" in output_lower or "power" in output_lower:
        return ErrorType.COMMUNICATION_ERROR
    
    if "verify failed" in output_lower or "verification" in output_lower:
        return ErrorType.VERIFICATION_FAILED
    
    if "blank check" in output_lower:
        return ErrorType.BLANK_CHECK_FAILED
    
    if "erase" in output_lower:
        return ErrorType.ERASE_FAILED
    
    if "programming failed" in output_lower or "program" in output_lower:
        return ErrorType.PROGRAMMING_FAILED
    
    return None
