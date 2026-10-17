# Leccion 8 - Tarea 2: analisis de cobertura

## Resultado

Las ocho pruebas pasan en `qemu_x86/atom`, con `CONFIG_ZTEST_SHUFFLE=y` y sin filtros de exclusion del volcado. El registro contiene `GCOV_COVERAGE_DUMP_END` y la captura finaliza sin errores.

| Metrica | Cobertura |
| --- | --- |
| Lineas | 81.4 % (35/43) |
| Funciones | 100 % (7/7) |
| Ramas | 64.3 % (9/14) |

El informe se encuentra en `twister-out/coverage/index.html`, desde la raiz del repositorio. Los resultados coinciden con la referencia de `HOMEWORK.md`.

## Caminos no cubiertos

Las ocho lineas ejecutables no cubiertas corresponden a cuatro bloques de error de `ring_buf.c`. La condicion de capacidad invalida contiene dos casos distintos, por lo que se identifican cinco pruebas adicionales posibles:

| Caso propuesto | Resultado esperado | Motivo por el que no esta cubierto |
| --- | --- | --- |
| `rb_init(0)` | `-EINVAL` | Los tests utilizan una capacidad valida de 4. |
| `rb_init(RING_BUF_MAX_CAPACITY + 1U)` | `-EINVAL` | No se solicita una capacidad superior al maximo. |
| `rb_pop(&value)` con buffer vacio | `-ENODATA`, sin modificar `value` | Las extracciones con puntero valido se realizan despues de insertar elementos. |
| `rb_peek(NULL)` | `-EINVAL` | Se prueba `pop(NULL)`, pero no el argumento nulo de `peek`. |
| `rb_peek(&value)` con buffer vacio | `-ENODATA`, sin modificar `value` | Las consultas actuales se realizan despues de insertar el valor 7. |

`rb_pop(NULL)` no recorre el caso de buffer vacio: la validacion del puntero devuelve `-EINVAL` antes de comprobar el contador.

El 100 % de funciones significa que todas fueron llamadas, no que se hayan ejecutado todos sus caminos. Las pruebas adicionales se describen como analisis; no se implementan porque la tarea 2 no lo exige.

## Reproduccion en Windows

Consultar `WINDOWS_SETUP.md` para el entorno, los comandos y el ajuste local de Twister conservado en `twister-windows-queue.patch`.
