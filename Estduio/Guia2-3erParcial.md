# 📝 Simulador de Examen Integrado:  
**LCD + Teclado Matricial 4x4 + Sensor LM35 + Microcontroladores**


---

## **Parte 1: Opción Múltiple**  
*Elige la respuesta correcta.*

1. **¿Cuál es la función principal de la DDRAM en un LCD?**  
   a) Guardar caracteres personalizados  
   b) Almacenar comandos del sistema  
   c) Guardar los caracteres que se muestran en pantalla y sus posiciones  
   d) Contener el set de caracteres ASCII  

2. **En un teclado matricial 4x4, ¿qué se hace para identificar una tecla?**  
   a) Se lee un valor analógico  
   b) Se barre cada fila y se detecta la columna activa  
   c) Se usa un decodificador BCD  
   d) Se mide la corriente en cada pin  

3. **Si el ADC entrega un valor Radc = 400 con Vref = 2.5V, ¿qué voltaje representa?**  
   a) 1.0V  
   b) ~0.98V  
   c) 2.5V  
   d) 5V  

---

## **Parte 2: Verdadero o Falso**  
*Indica si la afirmación es verdadera (V) o falsa (F).*

1. **El pin RS del LCD en alto (1) indica que se envía un comando de configuración.**  
   □ Verdadero  
   □ Falso  

2. **El sensor LM35 entrega 10 mV por cada grado Celsius.**  
   □ Verdadero  
   □ Falso  

3. **La CGROM del LCD puede ser modificada por el usuario para guardar caracteres personalizados.**  
   □ Verdadero  
   □ Falso  

4. **En modo de 4 bits, el LCD solo usa los pines D4–D7 para datos.**  
   □ Verdadero  
   □ Falso  

5. **El teclado matricial 4x4 puede detectar múltiples teclas presionadas al mismo tiempo.**  
   □ Verdadero  
   □ Falso  

---

## **Parte 3: Preguntas Abiertas**  
*Responde de forma clara y concisa.*

1. **Explica brevemente la diferencia entre DDRAM y CGRAM en un LCD.**

2. **Describe el proceso de lectura del sensor LM35 usando el ADC de 10 bits de un PIC, incluyendo la fórmula de conversión a temperatura.**

3. **¿Cómo inicializarías un LCD 16x2 en modo de 8 bits? Menciona al menos 3 comandos esenciales.**

4. **¿Qué ventaja tiene usar la librería `Keypad4x4` en MikroC frente a programar el barrido manual del teclado?**

---

## **Clave de Respuestas**

### Parte 1 – Opción Múltiple:
1. **c)** Guardar los caracteres que se muestran en pantalla y sus posiciones  
2. **b)** Se barre cada fila y se detecta la columna activa  
3. **b)** ~0.98V (Vadc = 2.5 × 400 / 1023 ≈ 0.978V)

### Parte 2 – Verdadero o Falso:
1. **Falso** (RS = 0 → comando, RS = 1 → dato)  
2. **Verdadero**  
3. **Falso** (CGROM es solo lectura, CGRAM es para personalizar)  
4. **Verdadero**  
5. **Falso** (Solo una tecla a la vez en la mayoría de las configuraciones básicas)