# Documentación - Librería LCD para X8C

## Configuración de Hardware

### Conexiones por Defecto (PORTD)

| Pin LCD | Pin PIC18F4550 | Función |
|---------|----------------|---------|
| RS      | RD0            | Selección Registro |
| RW      | RD1            | Lectura/Escritura |
| EN      | RD2            | Enable |
| D4      | RD4            | Datos bit 4 |
| D5      | RD5            | Datos bit 5 |
| D6      | RD6            | Datos bit 6 |
| D7      | RD7            | Datos bit 7 |
| VSS     | GND            | Tierra |
| VDD     | +5V            | Alimentación |
| V0      | Potenciómetro  | Contraste |
| A       | +5V (con resistencia) | Backlight+ |
| K       | GND            | Backlight- |

### Modificar Pines

Para cambiar los pines, edita las definiciones en `lcd.h`:

```c
#define LCD_RS_PIN      PORTDbits.RD0
#define LCD_RW_PIN      PORTDbits.RD1
#define LCD_EN_PIN      PORTDbits.RD2
// ... etc
```

---

## Funciones de Inicialización

### `lcd_begin()`

Inicializa la pantalla LCD y configura sus parámetros básicos.

**Sintaxis:**
```c
void lcd_begin(uint8_t cols, uint8_t rows)
```

**Parámetros:**
- `cols`: Número de columnas del LCD (normalmente 16)
- `rows`: Número de filas del LCD (normalmente 2)

**Retorna:** Nada

**Descripción:**
Configura los pines como salidas, inicializa el LCD en modo 4 bits, limpia la pantalla y establece el modo de entrada.

**Ejemplo:**
```c
void main(void) {
    // Configuración básica del PIC
    ADCON1 = 0x0F;  // Todo digital
    
    // Inicializar LCD de 16x2
    lcd_begin(16, 2);
    
    lcd_print("LCD Iniciado!");
    
    while(1);
}
```

---

## Funciones de Control Básico

### `lcd_clear()`

Limpia toda la pantalla y coloca el cursor en la posición (0,0).

**Sintaxis:**
```c
void lcd_clear(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_print("Texto viejo");
__delay_ms(2000);
lcd_clear();  // Limpia todo
lcd_print("Texto nuevo");
```

---

### `lcd_home()`

Regresa el cursor a la posición inicial (0,0) sin borrar el contenido.

**Sintaxis:**
```c
void lcd_home(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_setCursor(10, 1);
lcd_print("Hola");
__delay_ms(1000);
lcd_home();  // Cursor vuelve a (0,0)
lcd_print("Inicio");  // Sobrescribe desde el inicio
```

---

### `lcd_setCursor()`

Posiciona el cursor en una columna y fila específica.

**Sintaxis:**
```c
void lcd_setCursor(uint8_t col, uint8_t row)
```

**Parámetros:**
- `col`: Número de columna (0-15 para LCD 16x2)
- `row`: Número de fila (0-1 para LCD 16x2)

**Retorna:** Nada

**Descripción:**
Las posiciones comienzan desde 0. Para un LCD 16x2:
- Columnas: 0 a 15
- Filas: 0 a 1

**Ejemplo:**
```c
// Escribir en la esquina superior izquierda
lcd_setCursor(0, 0);
lcd_print("Fila 1");

// Escribir en la esquina inferior derecha
lcd_setCursor(10, 1);
lcd_print("Fila 2");

// Escribir en el centro
lcd_setCursor(6, 0);
lcd_print("Hola");
```

---

## Funciones de Escritura

### `lcd_print()`

Imprime una cadena de texto en la posición actual del cursor.

**Sintaxis:**
```c
void lcd_print(const char* str)
```

**Parámetros:**
- `str`: Puntero a cadena de texto terminada en null

**Retorna:** Nada

**Ejemplo:**
```c
lcd_clear();
lcd_setCursor(0, 0);
lcd_print("Hola Mundo!");

lcd_setCursor(0, 1);
lcd_print("PIC18F4550");
```

---

### `lcd_printNum()`

Imprime un número entero con signo.

**Sintaxis:**
```c
void lcd_printNum(int32_t num)
```

**Parámetros:**
- `num`: Número entero de 32 bits con signo (-2147483648 a 2147483647)

**Retorna:** Nada

**Ejemplo:**
```c
int contador = 0;
int temperatura = -15;

lcd_setCursor(0, 0);
lcd_print("Contador: ");
lcd_printNum(contador);

lcd_setCursor(0, 1);
lcd_print("Temp: ");
lcd_printNum(temperatura);
lcd_print("C");

// Salida:
// Contador: 0
// Temp: -15C
```

---

### `lcd_write()`

Escribe un solo carácter o código ASCII en la posición actual.

**Sintaxis:**
```c
void lcd_write(uint8_t value)
```

**Parámetros:**
- `value`: Código ASCII del carácter o índice de carácter personalizado (0-7)

**Retorna:** Nada

**Ejemplo:**
```c
// Escribir caracteres ASCII
lcd_write('A');  // Escribe 'A'
lcd_write(65);   // También escribe 'A' (código ASCII)

// Escribir carácter personalizado
lcd_createChar(0, heart);  // Crear corazón en posición 0
lcd_write(0);              // Mostrar el corazón

// Escribir símbolos especiales
lcd_write(0xDF);  // Símbolo de grado (°)
```

---

## Funciones de Visualización

### `lcd_display()`

Enciende la pantalla (muestra el contenido).

**Sintaxis:**
```c
void lcd_display(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_print("Mensaje");
__delay_ms(1000);
lcd_noDisplay();  // Apagar
__delay_ms(1000);
lcd_display();    // Encender nuevamente
```

---

### `lcd_noDisplay()`

Apaga la pantalla sin borrar el contenido.

**Sintaxis:**
```c
void lcd_noDisplay(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Descripción:**
El contenido permanece en memoria. Usa `lcd_display()` para mostrarlo nuevamente.

**Ejemplo:**
```c
// Efecto de parpadeo
for(int i = 0; i < 5; i++) {
    lcd_noDisplay();
    __delay_ms(500);
    lcd_display();
    __delay_ms(500);
}
```

---

## Funciones de Cursor

### `lcd_cursor()`

Muestra el cursor como una línea subrayada.

**Sintaxis:**
```c
void lcd_cursor(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_clear();
lcd_cursor();  // Mostrar cursor
lcd_print("Escribiendo...");
```

---

### `lcd_noCursor()`

Oculta el cursor.

**Sintaxis:**
```c
void lcd_noCursor(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_cursor();     // Mostrar cursor
__delay_ms(2000);
lcd_noCursor();   // Ocultar cursor
```

---

### `lcd_blink()`

Hace que el cursor parpadee como un bloque.

**Sintaxis:**
```c
void lcd_blink(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Descripción:**
Puede combinarse con `lcd_cursor()` para tener ambos efectos.

**Ejemplo:**
```c
lcd_clear();
lcd_setCursor(0, 0);
lcd_print("Cursor:");
lcd_setCursor(0, 1);
lcd_cursor();  // Línea subrayada
lcd_blink();   // Más parpadeo
```

---

### `lcd_noBlink()`

Desactiva el parpadeo del cursor.

**Sintaxis:**
```c
void lcd_noBlink(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_blink();
__delay_ms(3000);
lcd_noBlink();  // Detener parpadeo
```

---

## Funciones de Desplazamiento

### `lcd_scrollDisplayLeft()`

Desplaza todo el contenido de la pantalla una posición a la izquierda.

**Sintaxis:**
```c
void lcd_scrollDisplayLeft(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_clear();
lcd_print("DESPLAZAMIENTO");

// Desplazar hacia la izquierda
for(int i = 0; i < 10; i++) {
    lcd_scrollDisplayLeft();
    __delay_ms(300);
}
```

---

### `lcd_scrollDisplayRight()`

Desplaza todo el contenido de la pantalla una posición a la derecha.

**Sintaxis:**
```c
void lcd_scrollDisplayRight(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_clear();
lcd_print("HOLA");

// Desplazar derecha e izquierda
for(int i = 0; i < 5; i++) {
    lcd_scrollDisplayRight();
    __delay_ms(200);
}
for(int i = 0; i < 5; i++) {
    lcd_scrollDisplayLeft();
    __delay_ms(200);
}
```

---

## Funciones de Dirección de Texto

### `lcd_leftToRight()`

Establece la dirección de escritura de izquierda a derecha (modo normal).

**Sintaxis:**
```c
void lcd_leftToRight(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Descripción:**
Este es el modo por defecto. El texto se escribe de izquierda a derecha.

**Ejemplo:**
```c
lcd_clear();
lcd_leftToRight();  // Modo normal
lcd_setCursor(0, 0);
lcd_print("Normal");
```

---

### `lcd_rightToLeft()`

Establece la dirección de escritura de derecha a izquierda.

**Sintaxis:**
```c
void lcd_rightToLeft(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Descripción:**
Útil para idiomas que se escriben de derecha a izquierda.

**Ejemplo:**
```c
lcd_clear();
lcd_rightToLeft();
lcd_setCursor(15, 0);  // Comenzar desde la derecha
lcd_print("!aloH");    // Se mostrará como "Hola!"
lcd_leftToRight();     // Restaurar modo normal
```

---

### `lcd_autoscroll()`

Activa el desplazamiento automático del texto.

**Sintaxis:**
```c
void lcd_autoscroll(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Descripción:**
Hace que la pantalla se desplace automáticamente cuando se escribe nuevo texto.

**Ejemplo:**
```c
lcd_clear();
lcd_autoscroll();
lcd_setCursor(7, 0);

// El texto se desplazará automáticamente
for(char c = 'A'; c <= 'Z'; c++) {
    lcd_write(c);
    __delay_ms(200);
}
lcd_noAutoscroll();  // Desactivar
```

---

### `lcd_noAutoscroll()`

Desactiva el desplazamiento automático.

**Sintaxis:**
```c
void lcd_noAutoscroll(void)
```

**Parámetros:** Ninguno

**Retorna:** Nada

**Ejemplo:**
```c
lcd_autoscroll();
// ... código ...
lcd_noAutoscroll();  // Volver a modo normal
```

---

## Funciones de Caracteres Personalizados

### `lcd_createChar()`

Crea un carácter personalizado en una de las 8 ubicaciones disponibles.

**Sintaxis:**
```c
void lcd_createChar(uint8_t location, uint8_t charmap[])
```

**Parámetros:**
- `location`: Ubicación del carácter (0-7)
- `charmap`: Array de 8 bytes que define el patrón del carácter

**Retorna:** Nada

**Descripción:**
Cada carácter es una matriz de 5x8 píxeles. Cada byte del array representa una fila, usando los 5 bits menos significativos.

**Ejemplo:**
```c
// Crear un corazón
uint8_t heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000
};

// Crear símbolo de grado
uint8_t degree[8] = {
    0b00110,
    0b01001,
    0b01001,
    0b00110,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

// Crear una cara sonriente
uint8_t smiley[8] = {
    0b00000,
    0b01010,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
};

// Guardar los caracteres
lcd_createChar(0, heart);
lcd_createChar(1, degree);
lcd_createChar(2, smiley);

// Usar los caracteres
lcd_clear();
lcd_write(0);  // Mostrar corazón
lcd_print(" Te amo ");
lcd_write(0);

lcd_setCursor(0, 1);
lcd_print("Temp: 25");
lcd_write(1);  // Mostrar grado
lcd_print("C ");
lcd_write(2);  // Mostrar carita
```

---

## Funciones Adicionales (Extras)

### `lcd_printCenter()`

Imprime texto centrado en una fila específica.

**Sintaxis:**
```c
void lcd_printCenter(const char* str, uint8_t row)
```

**Parámetros:**
- `str`: Cadena de texto a centrar
- `row`: Número de fila donde centrar el texto (0-1)

**Retorna:** Nada

**Descripción:**
Calcula automáticamente la posición para centrar el texto en la pantalla.

**Ejemplo:**
```c
lcd_clear();
lcd_printCenter("Bienvenido", 0);
lcd_printCenter("PIC18F4550", 1);

// Salida:
//    Bienvenido   
//   PIC18F4550    
```

---

### `lcd_scrollText()`

Desplaza texto largo horizontalmente como un marquesina.

**Sintaxis:**
```c
void lcd_scrollText(const char* str, uint8_t row, uint16_t delay_ms)
```

**Parámetros:**
- `str`: Cadena de texto a desplazar
- `row`: Número de fila donde mostrar el texto (0-1)
- `delay_ms`: Retardo en milisegundos entre cada desplazamiento

**Retorna:** Nada

**Descripción:**
Si el texto es más largo que el ancho de la pantalla, lo desplaza de derecha a izquierda. Si es más corto, simplemente lo muestra.

**Ejemplo:**
```c
lcd_clear();

// Texto corto - solo se muestra
lcd_scrollText("Hola", 0, 300);

// Texto largo - se desplaza
lcd_scrollText("Este es un mensaje muy largo que se desplazara por la pantalla!", 1, 250);

// Mensaje de noticias
while(1) {
    lcd_scrollText("NOTICIAS: El PIC18F4550 es excelente para proyectos embebidos!", 0, 200);
    __delay_ms(1000);
}
```

---

## Ejemplos Completos

### Ejemplo 1: Pantalla de Bienvenida

```c
void main(void) {
    ADCON1 = 0x0F;
    lcd_begin(16, 2);
    
    lcd_clear();
    lcd_printCenter("Sistema", 0);
    lcd_printCenter("Iniciando...", 1);
    __delay_ms(2000);
    
    lcd_clear();
    lcd_printCenter("Bienvenido!", 0);
    __delay_ms(2000);
    
    while(1);
}
```

### Ejemplo 2: Contador con Temperatura

```c
void main(void) {
    ADCON1 = 0x0F;
    lcd_begin(16, 2);
    
    // Crear símbolo de grado
    uint8_t degree[8] = {
        0b00110, 0b01001, 0b01001, 0b00110,
        0b00000, 0b00000, 0b00000, 0b00000
    };
    lcd_createChar(0, degree);
    
    int contador = 0;
    int temp = 25;
    
    while(1) {
        lcd_clear();
        
        lcd_setCursor(0, 0);
        lcd_print("Contador: ");
        lcd_printNum(contador++);
        
        lcd_setCursor(0, 1);
        lcd_print("Temp: ");
        lcd_printNum(temp);
        lcd_write(0);  // Símbolo grado
        lcd_print("C");
        
        __delay_ms(1000);
    }
}
```

### Ejemplo 3: Menú Interactivo

```c
void mostrarMenu(uint8_t opcion) {
    lcd_clear();
    lcd_printCenter("=== MENU ===", 0);
    
    lcd_setCursor(0, 1);
    switch(opcion) {
        case 0:
            lcd_print(">1.Opcion 1");
            break;
        case 1:
            lcd_print(">2.Opcion 2");
            break;
        case 2:
            lcd_print(">3.Opcion 3");
            break;
    }
}

void main(void) {
    ADCON1 = 0x0F;
    lcd_begin(16, 2);
    
    uint8_t opcion = 0;
    
    while(1) {
        mostrarMenu(opcion);
        __delay_ms(2000);
        
        opcion++;
        if(opcion > 2) opcion = 0;
    }
}
```

### Ejemplo 4: Animación con Caracteres Personalizados

```c
void main(void) {
    ADCON1 = 0x0F;
    lcd_begin(16, 2);
    
    // Crear animación de barra de progreso
    uint8_t bar1[8] = {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b10000};
    uint8_t bar2[8] = {0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000};
    uint8_t bar3[8] = {0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100,0b11100};
    uint8_t bar4[8] = {0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110,0b11110};
    uint8_t bar5[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};
    
    lcd_createChar(0, bar1);
    lcd_createChar(1, bar2);
    lcd_createChar(2, bar3);
    lcd_createChar(3, bar4);
    lcd_createChar(4, bar5);
    
    lcd_clear();
    lcd_printCenter("Cargando...", 0);
    
    // Barra de progreso
    lcd_setCursor(0, 1);
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 5; j++) {
            lcd_setCursor(i, 1);
            lcd_write(j);
            __delay_ms(50);
        }
    }
    
    lcd_clear();
    lcd_printCenter("Completo!", 0);
    
    while(1);
}
```

### Ejemplo 5: Reloj Digital Simple

```c
void main(void) {
    ADCON1 = 0x0F;
    lcd_begin(16, 2);
    
    uint8_t horas = 12;
    uint8_t minutos = 0;
    uint8_t segundos = 0;
    
    while(1) {
        lcd_setCursor(0, 0);
        lcd_printCenter("Reloj Digital", 0);
        
        lcd_setCursor(4, 1);
        if(horas < 10) lcd_print("0");
        lcd_printNum(horas);
        lcd_print(":");
        if(minutos < 10) lcd_print("0");
        lcd_printNum(minutos);
        lcd_print(":");
        if(segundos < 10) lcd_print("0");
        lcd_printNum(segundos);
        
        __delay_ms(1000);
        
        segundos++;
        if(segundos >= 60) {
            segundos = 0;
            minutos++;
            if(minutos >= 60) {
                minutos = 0;
                horas++;
                if(horas >= 24) {
                    horas = 0;
                }
            }
        }
    }
}
```
