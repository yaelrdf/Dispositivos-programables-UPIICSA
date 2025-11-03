# Practicas

> [!TIP]
> En caso de tener problemas de loops o encendidos intermitentes, el causante mas posible son los "config bits" o "palabra de configuracion" por lo que es recomendable deshabilitar todas las caracteristicas no nesesarias, incluido el MTCLR. [Failsafe Config Bits](/Practicas/Practica%201%20PORT/Practica1PORT.X/configbits.s)

## Indice de practicas

- [Practica 0 ALU](/Practicas/Practica%200%20ALU/base.asm)
- [Practica 1 PORTS](/Practicas/Practica%201%20PORT/Practica1PORT.X/main.s)
- [Practica 2 Fecha](Practicas/Practica%2%Fecha/Fecha.X/main.s)
- [Practica 3 Contrasena](/Practicas/Practica%203%20Contrasena/Contrasena.X/main.s)
---
#### Para las practicas con manejo de display se utiliza un archivo .s separado para manejo de dispay.
- [Practica 4 Hola IPN](/Practicas/Practica%204%20Hola%20IPN/Hola_IPN.X/)
- [Practica 6 Frase (Electroencefalografista)](/Practicas/Practica%206%20Frase/Frase.X/)
- [Practica 7 Hexadecimal en display de 7 segmentos](/Practicas/Practica%207%20Hex%207%20Segmentos/7Seg-hex.X)
- [Practica 8 Timers(1ms, 1seg y 3seg en uno)](/Practicas/Practica%208%20TIMERS1/Timers_multi.X/)

----
#### Practicas con timing (1seg delay)
- [Practica 9 Contador Hexadecimal automatico](/Practicas/Practica%209%20Hex%20Automatico/Hex_auto.X/)
- [Practica 11 Contador Octal automatico](/Practicas/Practica%2011%20Octal%20Automatico/Octal.X/)
- [Practica 12 Contador Decimal automatico](/Practicas/Practica%2012%20Decimal%20Automatico/Decimal.X/)

> [!WARNING]
> Se utiliza el boton conectado al pin MCLR que corresponde a RE3, con comportamiento de flanco de bajada o (Pull low) para evitar modificaciones extras.

- [Practica 14 Contador 0-9 Activado por boton](/Practicas/Practica%2014%20Boton%20Decimal/Button.X/)

-  [Practica 16 Multiplexor especificacion](/Practicas/Practica%2016%20Multiplexor%202/Multiplex_2.X/)

| Seleccion  | Funcion |
| ------------- | ------------- |
|000 |color favorito|
|001 |superheroe1|
|010 |upiicsa|
|011 |dispositivos|
|100 |superheroe2|
|101 |deporte favorito|
|110 |cont_ dec ASC|
|111 |cont_hex desc|

> [!WARNING]
> Se utiliza el boton conectado al pin MCLR que corresponde a RE3, con comportamiento de flanco de bajada o (Pull low) para evitar modificaciones extras. Se ultilizo el puerto D como salida para el 2do display a la izquierda.

- [Practica 17 Contador del 0-99 con boton](/Practicas/Practica%2017%20Boton%200-99/Boton_99.X/)