"""
Manejador de pk2cmd.exe para comunicación con el programador PIC
"""
import subprocess
import sys
from pathlib import Path
from typing import Optional, Tuple, List
import shutil

from config import (
    PK2CMD_EXECUTABLE,
    PK2CMD_SEARCH_PATHS,
    PROGRAMMING_VOLTAGE,
    PROGRAMMING_SPEED,
    ENABLE_VERIFICATION,
    PRESERVE_EEPROM,
)
from error_handler import ErrorType, parse_pk2cmd_error


class PK2CMDHandler:
    """Manejador para pk2cmd.exe"""
    
    def __init__(self):
        """Inicializa el manejador"""
        self.pk2cmd_path: Optional[Path] = None
        self._find_pk2cmd()
    
    def _find_pk2cmd(self) -> bool:
        """
        Busca pk2cmd.exe en el sistema
        
        Returns:
            True si se encontró, False en caso contrario
        """
        # Buscar en rutas predefinidas
        for search_path in PK2CMD_SEARCH_PATHS:
            full_path = search_path / PK2CMD_EXECUTABLE
            if full_path.exists():
                self.pk2cmd_path = full_path
                return True
        
        # Buscar en PATH del sistema
        result = shutil.which(PK2CMD_EXECUTABLE)
        if result:
            self.pk2cmd_path = Path(result)
            return True
        
        return False
    
    def is_available(self) -> bool:
        """
        Verifica si pk2cmd.exe está disponible
        
        Returns:
            True si está disponible, False en caso contrario
        """
        return self.pk2cmd_path is not None and self.pk2cmd_path.exists()
    
    def _run_command(self, args: List[str]) -> Tuple[int, str, str]:
        """
        Ejecuta un comando de pk2cmd
        
        Args:
            args: Argumentos para pk2cmd
            
        Returns:
            Tupla de (código_salida, stdout, stderr)
        """
        if not self.is_available():
            raise FileNotFoundError(f"pk2cmd.exe no encontrado en {PK2CMD_SEARCH_PATHS}")
        
        try:
            cmd = [str(self.pk2cmd_path)] + args
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=120,
            )
            return result.returncode, result.stdout, result.stderr
        except subprocess.TimeoutExpired:
            raise TimeoutError("El comando pk2cmd.exe excedió el tiempo de espera")
        except Exception as e:
            raise RuntimeError(f"Error ejecutando pk2cmd: {e}")
    
    def detect_pickit(self) -> Tuple[bool, Optional[str]]:
        """
        Detecta si PICkit está conectado
        
        Returns:
            Tupla de (detectado, versión_firmware)
        """
        try:
            exit_code, stdout, stderr = self._run_command(["-B.", "-S#"])
            
            # Si detecta al menos una unidad
            if "Unit #" in stdout and exit_code == 0:
                # Extraer versión
                for line in stdout.split("\n"):
                    if "Firmware" in line:
                        return True, line.strip()
                return True, "Desconocida"
            
            return False, None
        except Exception:
            return False, None
    
    def detect_pic_device(self) -> Tuple[bool, Optional[str]]:
        """
        Detecta el dispositivo PIC conectado
        
        Returns:
            Tupla de (detectado, nombre_dispositivo)
        """
        try:
            exit_code, stdout, stderr = self._run_command(["-B.", "-P", "-I"])
            
            if exit_code == 0 and "No known part found" not in stdout:
                # Extraer nombre del dispositivo
                for line in stdout.split("\n"):
                    if "Device ID" in line or "Part" in line:
                        return True, line.strip()
                return True, "Dispositivo detectado"
            
            return False, None
        except Exception:
            return False, None
    
    def erase_device(self) -> Tuple[bool, str]:
        """
        Borra el dispositivo PIC
        
        Returns:
            Tupla de (éxito, mensaje)
        """
        try:
            exit_code, stdout, stderr = self._run_command(["-B.", "-P", "-E"])
            
            if exit_code == 0:
                return True, "Dispositivo borrado correctamente"
            else:
                error_output = stdout + stderr
                error_type = parse_pk2cmd_error(error_output)
                return False, f"Error al borrar: {error_output[:200]}"
        except Exception as e:
            return False, f"Excepción al borrar: {str(e)}"
    
    def blank_check(self) -> Tuple[bool, str]:
        """
        Verifica que el dispositivo esté en blanco
        
        Returns:
            Tupla de (en_blanco, mensaje)
        """
        try:
            exit_code, stdout, stderr = self._run_command(["-B.", "-P", "-C"])
            
            if exit_code == 0:
                return True, "Dispositivo en blanco verificado"
            else:
                return False, "El dispositivo no está completamente en blanco"
        except Exception as e:
            return False, f"Error verificando blanco: {str(e)}"
    
    def program_device(self, hex_file_path: str) -> Tuple[bool, str]:
        """
        Programa el dispositivo PIC con el archivo HEX
        
        Args:
            hex_file_path: Ruta al archivo HEX
            
        Returns:
            Tupla de (éxito, mensaje)
        """
        try:
            # Verificar que el archivo existe
            if not Path(hex_file_path).exists():
                return False, f"Archivo no encontrado: {hex_file_path}"
            
            # Construir comando
            args = ["-B.", "-P", "-A" + PROGRAMMING_VOLTAGE]
            args.extend(["-L" + PROGRAMMING_SPEED])
            args.extend(["-F" + hex_file_path, "-M"])
            
            if ENABLE_VERIFICATION:
                args.append("-Y")
            
            if PRESERVE_EEPROM:
                args.append("-Z")
            
            # Ejecutar programación
            exit_code, stdout, stderr = self._run_command(args)
            
            if exit_code == 0:
                return True, "Dispositivo programado correctamente"
            else:
                error_output = stdout + stderr
                error_type = parse_pk2cmd_error(error_output)
                return False, error_output[:300]
        except Exception as e:
            return False, f"Excepción durante programación: {str(e)}"
    
    def complete_programming_workflow(self, hex_file_path: str) -> Tuple[bool, List[Tuple[str, bool, str]]]:
        """
        Realiza el flujo completo de programación:
        1. Detectar PIC
        2. Borrar dispositivo
        3. Verificar en blanco
        4. Programar con verificación
        
        Args:
            hex_file_path: Ruta al archivo HEX
            
        Returns:
            Tupla de (éxito_general, lista_de_pasos)
            donde cada paso es (nombre, éxito, mensaje)
        """
        steps = []
        
        # Paso 1: Detectar dispositivo
        detected, device_name = self.detect_pic_device()
        steps.append(("Detectar dispositivo", detected, device_name or "No detectado"))
        if not detected:
            return False, steps
        
        # Paso 2: Borrar
        try:
            success, message = self.erase_device()
            steps.append(("Borrar dispositivo", success, message))
            if not success:
                return False, steps
        except Exception as e:
            steps.append(("Borrar dispositivo", False, str(e)))
            return False, steps
        
        # Paso 3: Verificar en blanco
        try:
            success, message = self.blank_check()
            steps.append(("Verificar en blanco", success, message))
            # No interrumpir si falla el blank check
        except Exception as e:
            steps.append(("Verificar en blanco", False, str(e)))
        
        # Paso 4: Programar
        try:
            success, message = self.program_device(hex_file_path)
            steps.append(("Programar", success, message))
            if not success:
                return False, steps
        except Exception as e:
            steps.append(("Programar", False, str(e)))
            return False, steps
        
        return True, steps
    
    def power_on_and_release_mclr(self) -> Tuple[bool, str]:
        """
        Mantiene la alimentación encendida y libera MCLR para que el dispositivo se ejecute
        
        Returns:
            Tupla de (éxito, mensaje)
        """
        try:
            exit_code, stdout, stderr = self._run_command(["-B.", "-P", "-T", "-R"])
            
            if exit_code == 0:
                return True, "Dispositivo liberado para ejecución"
            else:
                return False, "Error liberando dispositivo"
        except Exception as e:
            return False, f"Excepción: {str(e)}"
