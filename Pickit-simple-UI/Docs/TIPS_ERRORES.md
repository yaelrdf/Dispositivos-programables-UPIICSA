# 💡 TIPS Y SOLUCIÓN DE ERRORES PERSONALIZABLES

Este archivo contiene todos los mensajes de error y tips que usa el programa. Puede editarlos directamente en `error_handler.py` para customizar según su experiencia.

## 📋 Guía de Edición

Todos los errores están definidos en `error_handler.py` en el diccionario `ERROR_MESSAGES`. 

**Estructura de cada error:**
```python
ErrorType.ERROR_NAME: ErrorMessage(
    title="Título del error",
    message="Descripción breve",
    tips=[
        "Tip 1",
        "Tip 2",
        "Tip 3",
        ...
    ],
    error_type=ErrorType.ERROR_NAME,
),
```

---

## 🔴 ERRORES ACTUALES Y SUS TIPS

### 1. **FILE_NOT_FOUND** - Archivo no encontrado

**Mensaje:** "El archivo HEX seleccionado no existe o fue movido."

**Tips actuales:**
- Verifique que el archivo sigue en su ubicación original
- Intente seleccionar el archivo nuevamente
- Asegúrese de que la ruta no contiene caracteres especiales

**Cuándo mejorarlo:**
- Agregue más tips si encuentra nuevas causas
- Ej: paths muy largos, carpetas eliminadas, etc.

---

### 2. **FILE_INVALID** - Archivo HEX inválido

**Mensaje:** "El archivo seleccionado no es un archivo HEX válido."

**Tips actuales:**
- Asegúrese de que el archivo tiene extensión .hex
- Verifique que el archivo no está corrupto
- Intente recompilar el código en su IDE
- Confirme que el archivo fue generado correctamente

**Cuándo mejorarlo:**
- Si ves archivos corrompidos específicos
- Si ciertas herramientas generan archivos incompatibles
- Añade tips IDE-específicos

---

### 3. **PK2CMD_NOT_FOUND** - pk2cmd.exe no encontrado

**Mensaje:** "No se pudo localizar el ejecutable pk2cmd.exe en el sistema."

**Tips actuales:**
- Instale las herramientas de Microchip PICkit
- Agregue pk2cmd.exe a la carpeta del programa
- Incluya la ruta de instalación en las variables de entorno del sistema
- Descargue pk2cmd desde: https://www.microchip.com/...

**Cuándo mejorarlo:**
- Cuando hayas encontrado dónde se instala realmente
- Si hay versiones específicas que funcionan mejor
- Paths específicos para tu región/empresa

---

### 4. **PICKIT_NOT_DETECTED** - Programador PICkit no detectado

**Mensaje:** "Por favor, verifique que el programador PICkit está conectado al USB."

**Tips actuales:**
- Conecte el programador PICkit por USB
- Espere 2-3 segundos para que se estabilice la conexión
- Intente conectar a otro puerto USB
- Verifique que el cable USB no está dañado
- Instale los drivers de PICkit
- Reinicie el programa después de conectar el dispositivo

**Cuándo mejorarlo:**
- Agregue: "Verifique que el LED del PICkit está encendido"
- Agregue: "En Windows, busque el dispositivo en Administrador de dispositivos"
- Si usa hubs USB: "Intente conectar directamente al puerto USB de la computadora"

---

### 5. **PIC_NOT_DETECTED** - Dispositivo PIC no detectado

**Mensaje:** "El programador no pudo detectar el dispositivo PIC en la placa objetivo."

**Tips actuales:**
- Verifique que el PIC está insertado correctamente
- Verifique las conexiones ICSP (6 pines)
- Asegúrese de que el PIC tiene alimentación
- Intente con una velocidad de programación más lenta
- Verifique que el PIC no está protegido contra escritura

**Cuándo mejorarlo:**
- Agregue: "Verifique que la orientación del PIC es correcta (revise el datasheet)"
- Agregue: "Un PIC previamente programado puede estar en un estado especial"
- Agregue: "Intente con un PIC nuevo o uno que sabe que funcionaba"
- Agregue: "En velocidad lenta, intente: -L8 o -L16"

---

### 6. **COMMUNICATION_ERROR** - Error de comunicación

**Mensaje:** "Falló la comunicación con el dispositivo PIC."

**Tips actuales:**
- Verifique todas las conexiones ICSP
- Intente acercar el programador PICkit al dispositivo
- Reduzca la longitud del cable ICSP si es posible
- Intente con una velocidad de programación más lenta
- Apague y encienda el programador
- Reinicie el programa

**Cuándo mejorarlo:**
- Agregue: "Verifique que no hay cortocircuitos en las conexiones"
- Agregue: "Asegúrese que la placa no tiene interferencia (lejos de fuentes EM)"
- Agregue: "Intente sin otro dispositivo USB conectado"

---

### 7. **PROGRAMMING_FAILED** - Error durante la programación

**Mensaje:** "No se pudo escribir el código en el dispositivo PIC."

**Tips actuales:**
- Verifique que el dispositivo tiene alimentación adecuada
- Asegúrese de que el archivo HEX es compatible con el PIC
- Intente borrar el dispositivo manualmente primero
- Verifique que no hay conflictos de permisos de lectura/escritura

**Cuándo mejorarlo:**
- Agregue: "Algunos PICs salen con protección de lectura"
- Agregue: "Intente programar un archivo HEX simple (blanco) primero"
- Agregue: "Consulte el datasheet para los fuses/configuration bits"

---

### 8. **VERIFICATION_FAILED** - Error en la verificación

**Mensaje:** "El contenido programado no coincide con el archivo HEX."

**Tips actuales:**
- El dispositivo podría estar defectuoso
- Intente programar de nuevo
- Si el error persiste, reemplace el dispositivo PIC

**Cuándo mejorarlo:**
- Agregue: "Intente con velocidad más lenta: -L8"
- Agregue: "Verifique que el archivo HEX no fue modificado después de programar"
- Agregue: "El PIC podría estar dañado - intente con otro"

---

## ✏️ CÓMO EDITAR LOS TIPS

### Paso 1: Abrir el archivo
```
Abra: error_handler.py
```

### Paso 2: Encontrar el error
```python
# Busque por el nombre del error, ej:
ErrorType.PIC_NOT_DETECTED: ErrorMessage(
    title="Dispositivo PIC no detectado",
    message="El programador no pudo detectar el dispositivo PIC en la placa objetivo.",
    tips=[
        "Verifique que el PIC está insertado correctamente",
        "Verifique las conexiones ICSP (6 pines):",
        # ... más tips
    ],
    error_type=ErrorType.PIC_NOT_DETECTED,
),
```

### Paso 3: Modificar tips
```python
# Reemplace o agregue tips:
tips=[
    "Tip 1 nuevo",
    "Tip 2 nuevo",
    "Tip 3 nuevo",
    # Puede agregar tantos como necesite
],
```

### Paso 4: Guardar y probar
```bash
python main.py
# Teste el error para ver el nuevo tip
```

---

## 🎯 TIPS SEGÚN SU EXPERIENCIA

Basándome en la documentación de pk2cmd, aquí hay tips adicionales que puede considerar:

### Para **ERASE_FAILED**
- El PIC podría estar en modo especial de protección
- Puede necesitar software especial para desbloquear [verdeleteQuickly
- Intente múltiples ciclos de borrado

### Para **BLANK_CHECK_FAILED**
- Esto es no-critical, puede continuar programando
- Significa que habrá overwrite de memoria existente

### Para **VOLTAGE_ERROR**
- PIC16F y PIC18F típicamente usan 5.0V
- PIC24 y dsPIC33 típicamente usan 3.3V
- Verifique el datasheet para el voltaje exacto

### Para **CONNECTION_ERROR**
- Si usa cable largo: agregue "-L8" para velocidad lenta
- Si usa hub USB: conecte directamente al puerto
- Algunos cables USB de calidad baja no funcionan

---

## 🛠️ AGREGAR UN ERROR NUEVO

Si encuentra una situación que no está cubierta:

### 1. Agregue el tipo al enum (si es necesario)
```python
class ErrorType(Enum):
    # ... otros errores
    NEW_ERROR_TYPE = "new_error_type"
```

### 2. Agregue el mensaje
```python
ERROR_MESSAGES = {
    # ... otros errores
    ErrorType.NEW_ERROR_TYPE: ErrorMessage(
        title="Título del error",
        message="Descripción breve de qué salió mal",
        tips=[
            "Primer tip para resolver",
            "Segundo tip",
            "Tercer tip",
        ],
        error_type=ErrorType.NEW_ERROR_TYPE,
    ),
}
```

### 3. Detecte el error en pk2cmd_handler.py
```python
# En la función parse_pk2cmd_error():
if "patrón de búsqueda en la salida" in output_lower:
    return ErrorType.NEW_ERROR_TYPE
```

---

## 📝 TEMPLATE PARA NUEVO ERROR

```python
ErrorType.NUEVO_ERROR: ErrorMessage(
    title="[Título claro y conciso]",
    message="[Descripción en 1-2 líneas de qué salió mal]",
    tips=[
        "[Tip 1 - Lo más probable o fácil]",
        "[Tip 2 - Alternativa común]",
        "[Tip 3 - Causa técnica profunda]",
        "[Tip 4 - Referencia a documentación]",
    ],
    error_type=ErrorType.NUEVO_ERROR,
),
```

---

## ✅ CHECKLIST PARA ESCRIBIR BUENOS TIPS

- [ ] Cada tip comienza con verbo en imperativo ("Verifique", "Intente", etc.)
- [ ] Los tips van de simple a complejo
- [ ] Incluyen referencias a configuración (`config.py`)
- [ ] Son técnicos pero comprensibles para usuarios finales
- [ ] Incluye referencias a documentación si es relevante
- [ ] No son redundantes con el mensaje principal
- [ ] Son accionables (no dicen solo "intente de nuevo")

---

## 🎓 EJEMPLO DE BUEN TIP vs MALO

❌ **MALO**: "Verifique las conexiones"
✅ **BUENO**: "Verifique que el cable Pin 4 (PGD) está correctamente conectado entre el PICkit Pin 4 y el PIC (típicamente RC6)"

❌ **MALO**: "El dispositivo podría estar roto"
✅ **BUENO**: "Si el error persiste después de verificar conexiones, intente con otro PIC para descartar defecto del dispositivo"

---

## 📚 REFERENCIAS ÚTILES

- **Documentación pk2cmd**: Ver PDF adjunto
- **Datasheets PIC**: https://www.microchip.com/
- **Guía de conexiones**: CONEXIONES_ICSP.md
- **README completo**: README.md

---

**Última revisión:** 2026
**Versión:** 1.0
