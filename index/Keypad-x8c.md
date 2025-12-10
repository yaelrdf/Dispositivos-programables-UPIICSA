# Documentación de la Librería Keypad para PIC18F4550

## Índice
1. [Introducción](#introducción)
2. [Configuración de Hardware](#configuración-de-hardware)
3. [Estructura de Datos](#estructura-de-datos)
4. [Funciones Principales](#funciones-principales)
5. [Funciones Auxiliares](#funciones-auxiliares)
6. [Constantes y Definiciones](#constantes-y-definiciones)
7. [Ejemplos Completos](#ejemplos-completos)

---

## Introducción

Esta librería proporciona una interfaz completa para teclados matriciales 4x4 en el microcontrolador PIC18F4550, emulando la funcionalidad de la librería Keypad de Arduino. Está diseñada para el compilador XC8 y utiliza el PORTB del microcontrolador.

---

## Configuración de Hardware

### Conexiones del Teclado 4x4 al PORTB:

| Pin PIC  | Función    | Descripción                    |
|----------|------------|--------------------------------|
| RB0      | Fila 1     | Salida                        |
| RB1      | Fila 2     | Salida                        |
| RB2      | Fila 3     | Salida                        |
| RB3      | Fila 4     | Salida                        |
| RB4      | Columna 1  | Entrada con pull-up interno   |
| RB5      | Columna 2  | Entrada con pull-up interno   |
| RB6      | Columna 3  | Entrada con pull-up interno   |
| RB7      | Columna 4  | Entrada con pull-up interno   |

---

## Estructura de Datos

### `Keypad_t`

Estructura principal que contiene toda la información del teclado.

```c
typedef struct {
    char keys[ROWS][COLS];      // Mapa de teclas
    uint8_t rowPins[ROWS];      // Pines de las filas
    uint8_t colPins[COLS];      // Pines de las columnas
    char currentKey;            // Tecla actualmente presionada
    char lastKey;               // Última tecla presionada
    uint8_t state;              // Estado actual de la tecla
    uint32_t lastUpdateTime;    // Tiempo de última actualización
    uint32_t pressTime;         // Tiempo cuando se presionó la tecla
} Keypad_t;
```

---

## Funciones Principales

### 1. `Keypad_Init()`

Inicializa el teclado matricial y configura los pines del PORTB.

**Prototipo:**
```c
void Keypad_Init(Keypad_t* keypad, char keymap[ROWS][COLS]);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado
- `keymap`: Arreglo bidimensional con el mapa de caracteres del teclado

**Retorna:** Nada (void)

**Ejemplo:**
```c
char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

Keypad_t miTeclado;
Keypad_Init(&miTeclado, teclas);
```

---

### 2. `Keypad_GetKey()`

Lee el teclado y retorna la tecla presionada. Equivalente a `getKey()` de Arduino.

**Prototipo:**
```c
char Keypad_GetKey(Keypad_t* keypad);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado

**Retorna:** 
- Carácter de la tecla presionada
- `NO_KEY` ('\0') si no hay tecla presionada

**Ejemplo:**
```c
while (1) {
    char tecla = Keypad_GetKey(&miTeclado);
    
    if (tecla != NO_KEY) {
        printf("Tecla presionada: %c\n", tecla);
    }
    
    delay_ms(10);
}
```

---

### 3. `Keypad_GetState()`

Obtiene el estado actual de la tecla. Equivalente a `getState()` de Arduino.

**Prototipo:**
```c
uint8_t Keypad_GetState(Keypad_t* keypad);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado

**Retorna:** 
- `IDLE` (0): Sin actividad
- `PRESSED` (1): Tecla presionada
- `HOLD` (2): Tecla mantenida presionada
- `RELEASED` (3): Tecla liberada

**Ejemplo:**
```c
Keypad_GetKey(&miTeclado);
uint8_t estado = Keypad_GetState(&miTeclado);

switch (estado) {
    case PRESSED:
        printf("Tecla presionada: %c\n", miTeclado.currentKey);
        break;
    case HOLD:
        printf("Tecla mantenida: %c\n", miTeclado.currentKey);
        break;
    case RELEASED:
        printf("Tecla liberada: %c\n", miTeclado.lastKey);
        break;
}
```

---

### 4. `Keypad_IsPressed()`

Verifica si una tecla específica está presionada. Equivalente a `isPressed()` de Arduino.

**Prototipo:**
```c
bool Keypad_IsPressed(Keypad_t* keypad, char key);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado
- `key`: Carácter de la tecla a verificar

**Retorna:** 
- `true`: La tecla está presionada
- `false`: La tecla no está presionada

**Ejemplo:**
```c
// Esperar hasta que se presione la tecla '#'
while (!Keypad_IsPressed(&miTeclado, '#')) {
    Keypad_GetKey(&miTeclado);
    delay_ms(10);
}

printf("Tecla # presionada!\n");
```

---

### 5. `Keypad_WaitForKey()`

Espera de forma bloqueante hasta que se presione una tecla. Equivalente a `waitForKey()` de Arduino.

**Prototipo:**
```c
char Keypad_WaitForKey(Keypad_t* keypad);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado

**Retorna:** 
- Carácter de la tecla presionada

**Ejemplo:**
```c
printf("Presione cualquier tecla para continuar...\n");
char tecla = Keypad_WaitForKey(&miTeclado);
printf("Presionaste: %c\n", tecla);

// Útil para menús
printf("Seleccione una opción (1-3): ");
char opcion = Keypad_WaitForKey(&miTeclado);
```

---

### 6. `Keypad_SetDebounceTime()`

Configura el tiempo de antirrebote. Equivalente a `setDebounceTime()` de Arduino.

**Prototipo:**
```c
void Keypad_SetDebounceTime(Keypad_t* keypad, uint16_t ms);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado
- `ms`: Tiempo de antirrebote en milisegundos

**Retorna:** Nada (void)

**Ejemplo:**
```c
// Configurar antirrebote de 50ms
Keypad_SetDebounceTime(&miTeclado, 50);

// Para teclados con mucho rebote, aumentar el tiempo
Keypad_SetDebounceTime(&miTeclado, 100);
```

---

### 7. `Keypad_SetHoldTime()`

Configura el tiempo para detectar que una tecla está siendo mantenida. Equivalente a `setHoldTime()` de Arduino.

**Prototipo:**
```c
void Keypad_SetHoldTime(Keypad_t* keypad, uint16_t ms);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado
- `ms`: Tiempo en milisegundos para considerar una tecla como "mantenida"

**Retorna:** Nada (void)

**Ejemplo:**
```c
// Configurar 2 segundos para detectar tecla mantenida
Keypad_SetHoldTime(&miTeclado, 2000);

// Detectar teclas mantenidas
while (1) {
    Keypad_GetKey(&miTeclado);
    if (Keypad_GetState(&miTeclado) == HOLD) {
        printf("Tecla %c mantenida!\n", miTeclado.currentKey);
    }
    delay_ms(10);
}
```

---

## Funciones Auxiliares

### 8. `Keypad_GetString()`

Obtiene una cadena de caracteres del teclado hasta que se presione una tecla de terminación.

**Prototipo:**
```c
uint8_t Keypad_GetString(Keypad_t* keypad, char* buffer, uint8_t maxLength, char terminateKey);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado
- `buffer`: Buffer donde se almacenará la cadena
- `maxLength`: Longitud máxima de la cadena (sin incluir '\0')
- `terminateKey`: Tecla que finaliza la entrada

**Retorna:** 
- Número de caracteres ingresados

**Ejemplo:**
```c
char nombre[20];
printf("Ingrese su nombre (termine con #): ");
uint8_t longitud = Keypad_GetString(&miTeclado, nombre, 20, '#');
printf("\nNombre ingresado: %s (%d caracteres)\n", nombre, longitud);

// Ejemplo para código postal
char codigoPostal[6];
printf("Código postal (5 dígitos + #): ");
Keypad_GetString(&miTeclado, codigoPostal, 5, '#');
printf("\nCódigo: %s\n", codigoPostal);
```

---

### 9. `Keypad_GetNumber()`

Obtiene un número entero del teclado.

**Prototipo:**
```c
int32_t Keypad_GetNumber(Keypad_t* keypad, char terminateKey);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado
- `terminateKey`: Tecla que finaliza la entrada

**Retorna:** 
- Número entero ingresado (puede ser negativo)

**Ejemplo:**
```c
printf("Ingrese su edad (termine con #): ");
int32_t edad = Keypad_GetNumber(&miTeclado, '#');
printf("\nEdad ingresada: %ld años\n", edad);

// Números negativos (presionar '-' primero)
printf("Ingrese temperatura (termine con #): ");
int32_t temperatura = Keypad_GetNumber(&miTeclado, '#');
printf("\nTemperatura: %ld°C\n", temperatura);

// Calculadora simple
printf("Primer número: ");
int32_t num1 = Keypad_GetNumber(&miTeclado, '#');
printf("\nSegundo número: ");
int32_t num2 = Keypad_GetNumber(&miTeclado, '#');
int32_t suma = num1 + num2;
printf("\n%ld + %ld = %ld\n", num1, num2, suma);
```

---

### 10. `Keypad_GetPassword()`

Obtiene una contraseña del teclado (similar a GetString pero pensado para contraseñas).

**Prototipo:**
```c
uint8_t Keypad_GetPassword(Keypad_t* keypad, char* buffer, uint8_t maxLength, char terminateKey);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado
- `buffer`: Buffer donde se almacenará la contraseña
- `maxLength`: Longitud máxima de la contraseña
- `terminateKey`: Tecla que finaliza la entrada

**Retorna:** 
- Número de caracteres ingresados

**Ejemplo:**
```c
char password[9];
printf("Ingrese contraseña (termine con #): ");

// Obtener contraseña
uint8_t longitud = Keypad_GetPassword(&miTeclado, password, 8, '#');

// Mostrar asteriscos
printf("\nContraseña: ");
for (uint8_t i = 0; i < longitud; i++) {
    printf("*");
}
printf("\n");

// Verificar contraseña
if (strcmp(password, "1234") == 0) {
    printf("Acceso concedido\n");
} else {
    printf("Acceso denegado\n");
}
```

---

### 11. `Keypad_IsDigit()`

Verifica si una tecla es un dígito numérico.

**Prototipo:**
```c
bool Keypad_IsDigit(char key);
```

**Parámetros:**
- `key`: Carácter a verificar

**Retorna:** 
- `true`: El carácter es un dígito ('0'-'9')
- `false`: El carácter no es un dígito

**Ejemplo:**
```c
char tecla = Keypad_WaitForKey(&miTeclado);

if (Keypad_IsDigit(tecla)) {
    printf("%c es un número\n", tecla);
    int valor = Keypad_KeyToInt(tecla);
    printf("Valor numérico: %d\n", valor);
} else {
    printf("%c no es un número\n", tecla);
}
```

---

### 12. `Keypad_KeyToInt()`

Convierte un carácter de tecla a su valor entero.

**Prototipo:**
```c
int8_t Keypad_KeyToInt(char key);
```

**Parámetros:**
- `key`: Carácter a convertir

**Retorna:** 
- Valor entero (0-9) si es un dígito
- -1 si no es un dígito

**Ejemplo:**
```c
char tecla = Keypad_WaitForKey(&miTeclado);
int8_t valor = Keypad_KeyToInt(tecla);

if (valor >= 0) {
    printf("Valor: %d\n", valor);
    int doble = valor * 2;
    printf("El doble es: %d\n", doble);
} else {
    printf("No es un número válido\n");
}

// Ejemplo: Suma de dígitos
int suma = 0;
for (int i = 0; i < 5; i++) {
    printf("Ingrese dígito %d: ", i+1);
    char tecla = Keypad_WaitForKey(&miTeclado);
    int valor = Keypad_KeyToInt(tecla);
    if (valor >= 0) {
        suma += valor;
    }
}
printf("\nSuma total: %d\n", suma);
```

---

### 13. `Keypad_Flush()`

Limpia el buffer del teclado y espera a que todas las teclas sean liberadas.

**Prototipo:**
```c
void Keypad_Flush(Keypad_t* keypad);
```

**Parámetros:**
- `keypad`: Puntero a la estructura del teclado

**Retorna:** Nada (void)

**Ejemplo:**
```c
// Limpiar antes de esperar nueva entrada
printf("Limpiando buffer...\n");
Keypad_Flush(&miTeclado);
printf("Presione una tecla: ");
char tecla = Keypad_WaitForKey(&miTeclado);

// Útil después de una operación
printf("Procesando...\n");
delay_ms(2000);
Keypad_Flush(&miTeclado);  // Ignorar teclas durante el procesamiento
printf("Listo para nueva entrada\n");
```

---

## Constantes y Definiciones

### Estados de Tecla
```c
#define IDLE       0  // Sin actividad
#define PRESSED    1  // Tecla presionada
#define HOLD       2  // Tecla mantenida
#define RELEASED   3  // Tecla liberada
```

### Valores Especiales
```c
#define NO_KEY     '\0'  // Sin tecla presionada
```

### Dimensiones
```c
#define ROWS 4  // Número de filas
#define COLS 4  // Número de columnas
```

### Tiempos por Defecto
```c
#define DEBOUNCE_TIME  10    // ms de antirrebote
#define HOLD_TIME      1000  // ms para detectar tecla mantenida
```

---

## Ejemplos Completos

### Ejemplo 1: Lectura Simple

```c
#include "keypad.h"

char teclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

void main(void) {
    Keypad_t miTeclado;
    Keypad_Init(&miTeclado, teclas);
    
    while (1) {
        char tecla = Keypad_GetKey(&miTeclado);
        if (tecla != NO_KEY) {
            printf("Tecla: %c\n", tecla);
        }
        delay_ms(10);
    }
}
```

### Ejemplo 2: Sistema de Menú

```c
void menu_principal(void) {
    char opcion;
    
    while (1) {
        printf("\n=== MENÚ PRINCIPAL ===\n");
        printf("1. Ver temperatura\n");
        printf("2. Configurar alarma\n");
        printf("3. Ver historial\n");
        printf("*. Salir\n");
        printf("Opción: ");
        
        opcion = Keypad_WaitForKey(&miTeclado);
        printf("%c\n", opcion);
        
        switch (opcion) {
            case '1':
                mostrar_temperatura();
                break;
            case '2':
                configurar_alarma();
                break;
            case '3':
                ver_historial();
                break;
            case '*':
                return;
            default:
                printf("Opción inválida\n");
        }
    }
}
```

### Ejemplo 3: Sistema de Acceso con PIN

```c
void sistema_acceso(void) {
    char pin[5];
    const char PIN_CORRECTO[] = "1234";
    uint8_t intentos = 3;
    
    while (intentos > 0) {
        printf("\n=== SISTEMA DE ACCESO ===\n");
        printf("Intentos restantes: %d\n", intentos);
        printf("Ingrese PIN (4 dígitos + #): ");
        
        Keypad_GetString(&miTeclado, pin, 4, '#');
        printf("\n");
        
        if (strcmp(pin, PIN_CORRECTO) == 0) {
            printf("¡Acceso concedido!\n");
            LED_VERDE = 1;
            delay_ms(2000);
            return;
        } else {
            printf("PIN incorrecto\n");
            intentos--;
            LED_ROJO = 1;
            delay_ms(1000);
            LED_ROJO = 0;
        }
    }
    
    printf("¡Sistema bloqueado!\n");
    ALARMA = 1;
}
```

### Ejemplo 4: Calculadora

```c
void calculadora(void) {
    int32_t num1, num2, resultado;
    char operacion;
    
    printf("\n=== CALCULADORA ===\n");
    
    printf("Primer número (termine con #): ");
    num1 = Keypad_GetNumber(&miTeclado, '#');
    printf("\nNumero 1: %ld\n", num1);
    
    printf("Operación (+,-,*,/): ");
    operacion = Keypad_WaitForKey(&miTeclado);
    printf("%c\n", operacion);
    
    printf("Segundo número (termine con #): ");
    num2 = Keypad_GetNumber(&miTeclado, '#');
    printf("\nNumero 2: %ld\n", num2);
    
    switch (operacion) {
        case '+':
            resultado = num1 + num2;
            printf("\n%ld + %ld = %ld\n", num1, num2, resultado);
            break;
        case '-':
            resultado = num1 - num2;
            printf("\n%ld - %ld = %ld\n", num1, num2, resultado);
            break;
        case '*':
            resultado = num1 * num2;
            printf("\n%ld * %ld = %ld\n", num1, num2, resultado);
            break;
        case '/':
            if (num2 != 0) {
                resultado = num1 / num2;
                printf("\n%ld / %ld = %ld\n", num1, num2, resultado);
            } else {
                printf("\n¡Error! División por cero\n");
            }
            break;
        default:
            printf("\nOperación no válida\n");
    }
}
```

### Ejemplo 5: Contador de Productos

```c
void contador_productos(void) {
    int32_t total = 0;
    int32_t cantidad;
    char tecla;
    
    printf("\n=== CONTADOR DE PRODUCTOS ===\n");
    printf("A = Agregar | B = Quitar | # = Finalizar\n\n");
    
    while (1) {
        printf("Total actual: %ld unidades\n", total);
        printf("Opción: ");
        
        tecla = Keypad_WaitForKey(&miTeclado);
        printf("%c\n", tecla);
        
        if (tecla == '#') {
            break;
        }
        
        printf("Cantidad: ");
        cantidad = Keypad_GetNumber(&miTeclado, '#');
        printf("\n");
        
        if (tecla == 'A') {
            total += cantidad;
            printf("Agregados: %ld\n", cantidad);
        } else if (tecla == 'B') {
            total -= cantidad;
            printf("Removidos: %ld\n", cantidad);
        }
        
        printf("\n");
    }
    
    printf("\n=== RESUMEN ===\n");
    printf("Total final: %ld unidades\n", total);
}
```

### Ejemplo 6: Temporizador

```c
void configurar_temporizador(void) {
    int32_t minutos, segundos;
    
    printf("\n=== TEMPORIZADOR ===\n");
    
    printf("Minutos (termine con #): ");
    minutos = Keypad_GetNumber(&miTeclado, '#');
    printf("\n");
    
    printf("Segundos (termine con #): ");
    segundos = Keypad_GetNumber(&miTeclado, '#');
    printf("\n");
    
    int32_t total_segundos = (minutos * 60) + segundos;
    printf("Tiempo configurado: %ld:%02ld\n", minutos, segundos);
    
    printf("Presione * para iniciar...\n");
    Keypad_WaitForKey(&miTeclado);
    
    // Iniciar cuenta regresiva
    while (total_segundos > 0) {
        minutos = total_segundos / 60;
        segundos = total_segundos % 60;
        printf("\r%02ld:%02ld", minutos, segundos);
        delay_ms(1000);
        total_segundos--;
    }
    
    printf("\n¡Tiempo terminado!\n");
    BUZZER = 1;
    delay_ms(500);
    BUZZER = 0;
}
```

---

## Funciones Requeridas del Usuario

### ⚠️ CONFIGURACIÓN NECESARIA

**IMPORTANTE**: Debes definir la frecuencia de tu oscilador al inicio de tu archivo main.c:

```c
// Define la frecuencia de tu oscilador (en Hz)
#define _XTAL_FREQ 48000000  // Para 48MHz (USB)
// #define _XTAL_FREQ 20000000  // Para 20MHz
// #define _XTAL_FREQ 8000000   // Para 8MHz  
// #define _XTAL_FREQ 4000000   // Para 4MHz
```

**¡Eso es todo!** La librería usa `__delay_ms()` internamente, que es una función del compilador XC8 que no requiere configuración de Timer ni interrupciones.

### ✅ Ventajas de usar `__delay_ms()`:

1. **Sin Timer0**: No necesitas configurar ningún timer
2. **Sin interrupciones**: No hay ISR que escribir
3. **Más simple**: Solo define `_XTAL_FREQ` y listo
4. **Menos código**: No necesitas archivos adicionales

### ❌ Lo que NO necesitas hacer:

- ~~No necesitas implementar `millis()`~~
- ~~No necesitas configurar Timer0~~
- ~~No necesitas una ISR~~
- ~~No necesitas archivos timer.h/timer.c~~

---

## Notas Importantes

1. **⚠️ CONFIGURACIÓN OBLIGATORIA**: 
   - Debes definir `_XTAL_FREQ` al inicio de tu archivo main.c
   - Ejemplo: `#define _XTAL_FREQ 48000000` para 48MHz
   - Este valor debe coincidir con tu frecuencia real de oscilador
   
2. **Configuración de PORTB**: La librería configura automáticamente PORTB al inicializar

3. **Pull-ups internos**: Se habilitan automáticamente en las columnas (RB4-RB7)

4. **Antirrebote**: El tiempo por defecto es 10ms, ajustable según el teclado

5. **Bloqueo**: `Keypad_WaitForKey()` es bloqueante, usar con precaución

6. **Delays**: La librería usa `__delay_ms()` del compilador, no necesitas Timer

7. **Integración con LCD**: Totalmente compatible, ver ejemplo en `main.c`

8. **Warnings del compilador**: Los warnings sobre funciones no utilizadas son normales si no usas todas las funciones de la librería

9. **Compatibilidad**: Funciona con cualquier frecuencia de oscilador (4MHz - 48MHz)

---

## Licencia y Créditos

Librería desarrollada para PIC18F4550 con compilador XC8, basada en la librería Keypad de Arduino.
