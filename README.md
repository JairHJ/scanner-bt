| Targets Soportados | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-H21 | ESP32-H4 | ESP32-P4 | ESP32-S2 | ESP32-S3 | Linux |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | --------- | -------- | -------- | -------- | -------- | ----- |

# Ejemplo: Hola Mundo

Este proyecto es un ejemplo básico que imprime "Hola Mundo" utilizando FreeRTOS.

## Contenido del proyecto

El proyecto contiene los siguientes archivos principales:

```
├── CMakeLists.txt
├── pytest_hello_world.py      Script en Python para pruebas automatizadas
├── main
│   ├── CMakeLists.txt
│   └── hello_world_main.c     Código fuente principal
└── README.md                  Este archivo
```

## Cómo usar este ejemplo

1. Asegúrate de tener configurado el entorno de desarrollo ESP-IDF.
2. Compila y sube el proyecto a tu placa de desarrollo.
3. Abre el monitor serie para ver el mensaje "Hola Mundo".

## Solución de problemas

- **Error al subir el programa:**
  - Verifica las conexiones de hardware.
  - Reduce la velocidad de baudios en la configuración si es necesario.

## Más información

Para más detalles, consulta la [guía de inicio de ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/index.html).
