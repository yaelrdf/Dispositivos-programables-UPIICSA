@echo off
REM Programador PIC - Launcher para Windows
REM Este archivo ejecuta la aplicación con Python

cd /d "%~dp0"

REM Verificar si venv existe
if exist venv\Scripts\activate.bat (
    call venv\Scripts\activate.bat
    python main.py
) else (
    REM Si no existe venv, ejecutar con Python directamente
    echo.
    echo ========================================
    echo Programador PIC v1.0
    echo ========================================
    echo.
    echo Para mejor experiencia, se recomienda crear un ambiente virtual
    echo Ejecutando con Python del sistema...
    echo.
    python main.py
)

if errorlevel 1 (
    echo.
    echo ERROR: No se pudo iniciar la aplicacion.
    echo.
    echo Verifique que:
    echo 1. Python 3.8+ esta instalado
    echo 2. Las dependencias estan instaladas: pip install -r requirements.txt
    echo 3. pk2cmd.exe esta disponible
    echo.
    pause
)
