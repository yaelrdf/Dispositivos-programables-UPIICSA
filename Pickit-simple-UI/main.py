"""
Programador PIC - Interfaz minimalista estilo Balena Etcher
"""
import sys
from pathlib import Path
from typing import Optional
import threading

from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QLabel, QFileDialog, QMessageBox, QProgressBar
)
from PyQt5.QtCore import Qt, pyqtSignal, QObject, QThread
from PyQt5.QtGui import QFont

from config import APP_TITLE, UI_STRINGS, ALLOWED_FILE_EXTENSIONS
from error_handler import ErrorType, get_error_message
from pk2cmd_handler import PK2CMDHandler


class ColorScheme:
    """Colores simples y sólidos"""
    PRIMARY = "#0066CC"      # Azul principal
    SUCCESS = "#00CC00"      # Verde éxito
    ERROR = "#CC0000"        # Rojo error
    WARNING = "#FFAA00"      # Amarillo advertencia
    BACKGROUND = "#FFFFFF"   # Blanco
    LIGHT_GRAY = "#F5F5F5"   # Gris muy claro
    DARK_TEXT = "#333333"    # Texto oscuro
    LIGHT_TEXT = "#888888"   # Texto claro
    BORDER = "#DDDDDD"       # Borde gris



class WorkerSignals(QObject):
    """Señales del worker thread"""
    progress = pyqtSignal(str)
    completed = pyqtSignal(bool, list)
    error = pyqtSignal(ErrorType, str)


class ProgrammingWorker(QThread):
    """Ejecuta programación en background"""
    
    signals = WorkerSignals()
    
    def __init__(self, handler: PK2CMDHandler, hex_file: str):
        super().__init__()
        self.handler = handler
        self.hex_file = hex_file
    
    def run(self):
        try:
            self.signals.progress.emit("Detectando PICkit...")
            pickit_found, firmware = self.handler.detect_pickit()
            
            if not pickit_found:
                self.signals.error.emit(ErrorType.PICKIT_NOT_DETECTED, "")
                return
            
            self.signals.progress.emit(f"Detectado: {firmware}")
            self.signals.progress.emit("Programando...")
            
            success, steps = self.handler.complete_programming_workflow(self.hex_file)
            
            if success:
                self.signals.progress.emit("Liberando dispositivo...")
                self.signals.completed.emit(True, steps)
            else:
                error_step = next((s for s in steps if not s[1]), None)
                if error_step:
                    from error_handler import parse_pk2cmd_error
                    error_type = parse_pk2cmd_error(error_step[2]) or ErrorType.PROGRAMMING_FAILED
                    self.signals.error.emit(error_type, error_step[2])
                else:
                    self.signals.error.emit(ErrorType.PROGRAMMING_FAILED, "")
        
        except Exception as e:
            self.signals.error.emit(ErrorType.UNKNOWN_ERROR, str(e))


class PICProgrammerUI(QMainWindow):
    """Interfaz principal - diseño minimalista"""
    
    def __init__(self):
        super().__init__()
        self.handler = PK2CMDHandler()
        self.selected_file: Optional[Path] = None
        self.worker: Optional[ProgrammingWorker] = None
        
        self.init_ui()
        self.apply_styles()
        self.check_pk2cmd()
    
    def init_ui(self):
        """Inicializa interfaz"""
        self.setWindowTitle(APP_TITLE)
        self.setGeometry(100, 100, 1000, 500)
        self.setMinimumSize(900, 450)
        
        # Widget central
        central = QWidget()
        self.setCentralWidget(central)
        
        main_layout = QHBoxLayout(central)
        main_layout.setContentsMargins(40, 40, 40, 40)
        main_layout.setSpacing(60)
        
        # Columna izquierda - Selección de archivo
        left = self.create_left_column()
        main_layout.addLayout(left, 1)
        
        # Columna derecha - Estado y programación
        right = self.create_right_column()
        main_layout.addLayout(right, 1)
    
    def create_left_column(self) -> QVBoxLayout:
        """Columna izquierda con selección de archivo"""
        layout = QVBoxLayout()
        layout.setSpacing(15)
        
        # Número y título
        title = QLabel("1. Seleccionar archivo")
        title.setFont(QFont("Segoe UI", 16, QFont.Bold))
        title.setStyleSheet(f"color: {ColorScheme.DARK_TEXT};")
        layout.addWidget(title)
        
        # Descripción
        desc = QLabel("Seleccione un archivo HEX compilado para programar el dispositivo")
        desc.setFont(QFont("Segoe UI", 11))
        desc.setStyleSheet(f"color: {ColorScheme.LIGHT_TEXT};")
        desc.setWordWrap(True)
        layout.addWidget(desc)
        
        # Espacio
        layout.addSpacing(20)
        
        # Nombre del archivo
        self.file_name = QLabel("Ningún archivo seleccionado")
        self.file_name.setFont(QFont("Segoe UI", 11))
        self.file_name.setStyleSheet(f"color: {ColorScheme.LIGHT_TEXT};")
        layout.addWidget(self.file_name)
        
        # Tamaño
        self.file_size = QLabel("")
        self.file_size.setFont(QFont("Segoe UI", 10))
        self.file_size.setStyleSheet(f"color: {ColorScheme.LIGHT_TEXT};")
        layout.addWidget(self.file_size)
        
        # Botón
        self.select_btn = QPushButton("Seleccionar archivo")
        self.select_btn.setFixedHeight(48)
        self.select_btn.setFont(QFont("Segoe UI", 11))
        self.select_btn.setCursor(Qt.PointingHandCursor)
        self.select_btn.clicked.connect(self.select_file)
        layout.addWidget(self.select_btn)
        
        layout.addStretch()
        return layout
    
    def create_right_column(self) -> QVBoxLayout:
        """Columna derecha con estado y programación"""
        layout = QVBoxLayout()
        layout.setSpacing(15)
        
        # Número y título
        title = QLabel("2. Programar dispositivo")
        title.setFont(QFont("Segoe UI", 16, QFont.Bold))
        title.setStyleSheet(f"color: {ColorScheme.DARK_TEXT};")
        layout.addWidget(title)
        
        # Descripción
        desc = QLabel("Conecte el PICkit y el dispositivo PIC, luego haga clic en Programar")
        desc.setFont(QFont("Segoe UI", 11))
        desc.setStyleSheet(f"color: {ColorScheme.LIGHT_TEXT};")
        desc.setWordWrap(True)
        layout.addWidget(desc)
        
        # Espacio
        layout.addSpacing(20)
        
        # Estado
        state_label = QLabel("Estado:")
        state_label.setFont(QFont("Segoe UI", 11, QFont.Bold))
        state_label.setStyleSheet(f"color: {ColorScheme.DARK_TEXT};")
        layout.addWidget(state_label)
        
        self.status = QLabel("Listo")
        self.status.setFont(QFont("Segoe UI", 11))
        self.status.setStyleSheet(f"color: {ColorScheme.LIGHT_TEXT};")
        layout.addWidget(self.status)
        
        # Barra de progreso
        self.progress_bar = QProgressBar()
        self.progress_bar.setVisible(False)
        self.progress_bar.setFixedHeight(4)
        self.progress_bar.setTextVisible(False)
        self.progress_bar.setStyleSheet(f"""
            QProgressBar {{
                border: none;
                background-color: {ColorScheme.LIGHT_GRAY};
            }}
            QProgressBar::chunk {{
                background-color: {ColorScheme.PRIMARY};
            }}
        """)
        layout.addWidget(self.progress_bar)
        
        # Mensaje
        self.message = QLabel("")
        self.message.setFont(QFont("Segoe UI", 10))
        self.message.setStyleSheet(f"color: {ColorScheme.ERROR};")
        self.message.setWordWrap(True)
        layout.addWidget(self.message)
        
        layout.addStretch()
        
        # Botones
        btn_layout = QHBoxLayout()
        btn_layout.setSpacing(10)
        
        self.program_btn = QPushButton("Programar")
        self.program_btn.setFixedHeight(48)
        self.program_btn.setFont(QFont("Segoe UI", 12, QFont.Bold))
        self.program_btn.setCursor(Qt.PointingHandCursor)
        self.program_btn.setEnabled(False)
        self.program_btn.clicked.connect(self.start_programming)
        btn_layout.addWidget(self.program_btn)
        
        self.cancel_btn = QPushButton("Cancelar")
        self.cancel_btn.setFixedHeight(48)
        self.cancel_btn.setFont(QFont("Segoe UI", 11))
        self.cancel_btn.setCursor(Qt.PointingHandCursor)
        self.cancel_btn.setVisible(False)
        self.cancel_btn.clicked.connect(self.cancel_programming)
        btn_layout.addWidget(self.cancel_btn)
        
        layout.addLayout(btn_layout)
        
        return layout
    
    def apply_styles(self):
        """Aplica estilos a toda la aplicación"""
        self.setStyleSheet(f"""
            QMainWindow {{
                background-color: {ColorScheme.BACKGROUND};
            }}
        """)
        
        # Botón seleccionar (gris claro)
        self.select_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: {ColorScheme.LIGHT_GRAY};
                color: {ColorScheme.DARK_TEXT};
                border: 1px solid {ColorScheme.BORDER};
                border-radius: 4px;
                padding: 8px;
                font-weight: bold;
            }}
            QPushButton:hover {{
                background-color: #EEEEEE;
            }}
            QPushButton:pressed {{
                background-color: #DDDDDD;
            }}
        """)
        
        # Botón programar (azul)
        self.program_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: {ColorScheme.PRIMARY};
                color: white;
                border: none;
                border-radius: 4px;
                padding: 8px;
                font-weight: bold;
            }}
            QPushButton:hover:!pressed {{
                background-color: #0052A3;
            }}
            QPushButton:pressed {{
                background-color: #003F7A;
            }}
            QPushButton:disabled {{
                background-color: {ColorScheme.LIGHT_GRAY};
                color: {ColorScheme.LIGHT_TEXT};
            }}
        """)
        
        # Botón cancelar (rojo)
        self.cancel_btn.setStyleSheet(f"""
            QPushButton {{
                background-color: {ColorScheme.ERROR};
                color: white;
                border: none;
                border-radius: 4px;
                padding: 8px;
                font-weight: bold;
            }}
            QPushButton:hover:!pressed {{
                background-color: #BB0000;
            }}
            QPushButton:pressed {{
                background-color: #990000;
            }}
        """)
    
    def select_file(self):
        """Abre diálogo para seleccionar archivo"""
        file_path, _ = QFileDialog.getOpenFileName(
            self,
            "Seleccionar archivo HEX",
            "",
            "Archivos HEX (*.hex *.HEX);;Todos (*.*)"
        )
        
        if file_path:
            self.selected_file = Path(file_path)
            
            # Actualizar labels
            self.file_name.setText(f"✓ {self.selected_file.name}")
            self.file_name.setStyleSheet(f"color: {ColorScheme.SUCCESS};")
            
            # Tamaño
            size_kb = self.selected_file.stat().st_size / 1024
            self.file_size.setText(f"{size_kb:.1f} KB")
            
            # Habilitar programar
            self.program_btn.setEnabled(True)
            self.message.setText("")
    
    def start_programming(self):
        """Inicia la programación"""
        if not self.selected_file:
            self.message.setText("Seleccione un archivo primero")
            return
        
        self.program_btn.setEnabled(False)
        self.program_btn.setVisible(False)
        self.cancel_btn.setVisible(True)
        self.progress_bar.setVisible(True)
        self.progress_bar.setValue(0)
        
        self.status.setText("Programando...")
        self.status.setStyleSheet(f"color: {ColorScheme.PRIMARY};")
        self.message.setText("")
        
        # Worker
        self.worker = ProgrammingWorker(self.handler, str(self.selected_file))
        self.worker.signals.progress.connect(self.update_progress)
        self.worker.signals.completed.connect(self.on_completed)
        self.worker.signals.error.connect(self.on_error)
        self.worker.start()
    
    def cancel_programming(self):
        """Cancela la programación"""
        if self.worker and self.worker.isRunning():
            self.worker.quit()
            self.worker.wait()
        
        self.progress_bar.setVisible(False)
        self.cancel_btn.setVisible(False)
        self.program_btn.setVisible(True)
        self.program_btn.setEnabled(True)
        
        self.status.setText("Cancelado")
        self.status.setStyleSheet(f"color: {ColorScheme.WARNING};")
        self.message.setText("Programación cancelada por el usuario")
        self.message.setStyleSheet(f"color: {ColorScheme.WARNING};")
    
    def update_progress(self, msg: str):
        """Actualiza el progreso"""
        self.status.setText(msg)
        self.progress_bar.setValue(min(self.progress_bar.value() + 15, 90))
    
    def on_completed(self, success: bool, steps: list):
        """Se ejecuta al terminar"""
        self.progress_bar.setValue(100)
        self.progress_bar.setVisible(False)
        self.cancel_btn.setVisible(False)
        self.program_btn.setVisible(True)
        self.program_btn.setEnabled(True)
        
        self.status.setText("✓ Completado")
        self.status.setStyleSheet(f"color: {ColorScheme.SUCCESS};")
        self.message.setText("Dispositivo programado exitosamente")
        self.message.setStyleSheet(f"color: {ColorScheme.SUCCESS};")
    
    def on_error(self, error_type: ErrorType, extra: str):
        """Se ejecuta si hay error"""
        self.progress_bar.setVisible(False)
        self.cancel_btn.setVisible(False)
        self.program_btn.setVisible(True)
        self.program_btn.setEnabled(True)
        
        error_msg = get_error_message(error_type)
        
        self.status.setText("✗ Error")
        self.status.setStyleSheet(f"color: {ColorScheme.ERROR};")
        self.message.setText(error_msg.message)
        self.message.setStyleSheet(f"color: {ColorScheme.ERROR};")
    
    def check_pk2cmd(self):
        """Verifica pk2cmd.exe"""
        if not self.handler.is_available():
            QMessageBox.warning(
                self,
                "Error",
                "pk2cmd.exe no encontrado. Instale Microchip Tools primero."
            )


def main():
    app = QApplication(sys.argv)
    window = PICProgrammerUI()
    window.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
