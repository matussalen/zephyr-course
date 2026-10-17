# Pruebas y cobertura en Windows

Entorno validado: Zephyr v4.4.0, Zephyr SDK 1.0.1 y gcovr 8.6.

## Configuracion persistente del equipo

- Variable de usuario `QEMU_BIN_PATH`: `C:\Users\Usuario\zephyr-sdk-1.0.1\hosttools\qemu`.
- Carpeta agregada al `Path` del usuario: `C:\Users\Usuario\zephyr-sdk-1.0.1\gnu\x86_64-zephyr-elf\bin`.
- Reiniciar completamente VS Code y las terminales para heredar las variables actualizadas. Si Windows sigue proporcionando un entorno anterior, cerrar sesion y volver a entrar.
- El escenario de `testcase.yaml` tiene `timeout: 300`. No es necesario `--timeout-multiplier 5`.

Estas variables son configuracion local; no se instalan al clonar este repositorio.

## Ajuste local de Twister

En `D:\Zephyr\deps\zephyr\scripts\pylib\twister\twisterlib\handlers.py`, dentro de `QEMUWinHandler._monitor_output`, se sustituyo `queue.get_nowait()` por `queue.get(timeout=0.1)`.

En este equipo, el ajuste permitio completar el volcado de cobertura, que antes terminaba sin `GCOV_COVERAGE_DUMP_END`. Es una adaptacion local verificada, no una correccion oficial de Zephyr.

El cambio pertenece al repositorio de la dependencia Zephyr. Se conserva entre sesiones, pero una reinstalacion o actualizacion puede eliminarlo o entrar en conflicto con el. Antes de actualizar, revisar ese cambio; no reaplicarlo automaticamente a otras versiones.

Se guarda una copia en `twister-windows-queue.patch`. Si el ajuste desaparece al restaurar la misma version, desde `D:\Zephyr\zephyr-course` comprobar y aplicar:

```bat
git -C ..\deps\zephyr apply --check ..\..\zephyr-course\tests\ring_buf\twister-windows-queue.patch
git -C ..\deps\zephyr apply ..\..\zephyr-course\tests\ring_buf\twister-windows-queue.patch
```

No volver a aplicar si ya esta incorporado. La comprobacion fallara en ese caso o si el contexto ha cambiado.

## Comandos habituales

Desde `D:\Zephyr\zephyr-course`, con el entorno virtual activado:

```bat
west twister -T tests/ring_buf -p qemu_x86 -v
west twister -T tests/ring_buf -p qemu_x86 -v --coverage --coverage-tool gcovr --coverage-basedir app/modules/ring_buf --gcov-tool x86_64-zephyr-elf-gcov.exe
```

No pasar una ruta de Windows normalizada con barras inversas a `--gcov-tool`: en el entorno observado, gcovr no pudo ejecutarla. Se utiliza el nombre del ejecutable accesible mediante `PATH`.

Mantener `CONFIG_ZTEST_SHUFFLE=y` en `prj.conf`; `--extra-args=CONFIG_ZTEST_SHUFFLE=n` solo se utilizo para diagnosticar.

## Validacion del informe

- Las ocho pruebas deben aprobar.
- No debe aparecer `Gcov data capture incomplete`.
- `handler.log` debe contener `GCOV_COVERAGE_DUMP_END`.
- Abrir `twister-out/coverage/index.html`. La referencia de la tarea es 81.4 % de lineas, 100 % de funciones y 64.3 % de ramas.
- La tarea 2 solicita versionar solo `twister-out/coverage/`, no todo el directorio de compilacion.
