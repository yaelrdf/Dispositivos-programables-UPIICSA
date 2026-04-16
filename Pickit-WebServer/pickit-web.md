# Pickit Web server
**El programador debe estar conectado a la maquina servidor**

Script desarrollado en python, que despliega un servidor web accesible en toda la red local que permite programar cualquier microcontrolador soportado por el programador PicKit o DisPic.

Esta diseñado para multiples clientes y cuenta con manejo de cola, por lo que solo un cliente a la vez podra programar un solo microcontrolador.

Se recomienda para esenarios donde es nesesario tener un programador compartido entre varias personas. **El programador debe estar conectado a la maquina servidor**

## Uso
Desde la [carpeta](/Dispositivos-programables-UPIICSA/Pickit-WebServer/) `Pickit-WebServer`


Ejecute
`python ./main.py`

El servidor web es accesible desde el puerto `8181` como `http://localhost:8181`
