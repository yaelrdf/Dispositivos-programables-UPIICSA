# INICIO RÁPIDO - Programador PIC

## 1️⃣ Instalación de Python
Si no tiene Python instalado:
1. Visite https://www.python.org/downloads/
2. Descargue Python 3.8 o superior
3. **Importante**: Marque "Add Python to PATH" durante la instalación

## 2️⃣ Instalación de pk2cmd.exe
El programa usa pk2cmd.exe para comunicarse con el PICkit. Opción recomendada:

### Opción A: Copiar pk2cmd.exe aquí (FÁCIL)
1. Descargue pk2cmd.exe desde Microchip
2. Copie el archivo a esta carpeta
3. Rename a `pk2cmd.exe` si es necesario

### Opción B: Instalar en Program Files
1. Descargue MPLAB X IDE desde https://www.microchip.com/
2. Instale en ubicación por defecto
3. pk2cmd.exe se encontrará automáticamente

### Opción C: Agregar a Path de Windows
1. Coloque pk2cmd.exe en una carpeta
2. Agregue esa carpeta a la variable PATH de Windows
3. Ver guía en README.md

## 3️⃣ Instalar dependencias Python

Abra terminal/CMD en esta carpeta y ejecute:

```bash
pip install -r requirements.txt
```

## 4️⃣ Ejecutar la aplicación

### Opción 1: Doble clic en run.bat (RECOMENDADO)
- Solo haga doble clic en `run.bat`

### Opción 2: Por línea de comandos
```bash
python main.py
```

### Opción 3: Crear ambiente virtual (AVANZADO)
```bash
python -m venv venv
venv\Scripts\activate
pip install -r requirements.txt
python main.py
```

## 5️⃣ Primera ejecución

1. Conecte el PICkit por USB
2. Espere 2 segundos para que se detected
3. Haga clic en "Seleccionar archivo HEX"
4. Elija un archivo .hex válido
5. Haga clic en "Programar"
6. ¡Listo! El programa hará el resto automáticamente

## ⚠️ Si ve mensajes de error:

### "pk2cmd.exe no encontrado"
→ Instale pk2cmd.exe (ver paso 2 arriba)

### "PICkit no detectado"
→ Verifique la conexión USB del PICkit

### "Dispositivo PIC no detectado"
→ Verifique las conexiones ICSP de 6 pines

Ver **README.md** para más detalles sobre resolución de problemas.

## 📞 Ayuda

Si necesita ayuda, revise:
1. README.md (documentación completa)
2. Los mensajes de error mostrados en la aplicación
3. Incluyen tips de que verificar

## 🎯 Conexiones ICSP típicas

```
PICkit      Placa Target
Pin 1 (Rojo/Naranja)  → MCLR/VPP
Pin 2 (Rojo)          → VDD (+3.3V o +5V)
Pin 3 (Negro)         → VSS (GND)
Pin 4 (Blanco/Amarillo) → PGD
Pin 5 (Marrón/Verde)  → PGC
Pin 6                 → No se conecta (AUX)
```

¡Listo para programar PICs! 🚀
