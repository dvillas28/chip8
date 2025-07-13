# CHIP8 - Proposito de cada archivo

Escribo este documento para ir anotando que es lo hacia cada archivo, ya que no he tocado el emulador como en 7 meses.

Tambien sirve como flujo del funcionamiento del emulador

## `main.c`: Inicialización del contexto, carga de la ROM e inicio del computador

1. Recibe un path con una ROM
2. Inicializa una estructura de contexto (`context.c:init_ctx`)
3. Prueba a cargar la ROM (`context.c:load_rom`)
4. Si falla, finaliza el programa
5. Si la carga funciona correctamente, se inicia el computador (`chip8.c:start_chip8`)

## `context.c`: Contexto y carga de ROM y fonts

- Define una struct 'ChipContext' con info del computador (tamaño de memoria y registros)
  - 4KB de RAM
  - 16 registros de 8 bits (0-F)
  - Stack de 16 direcciones de 16 bits
  - Registro `PC` de 12 bits
  - Registro `I` de 12 bits
  - Registro `SP` de 8 bits
  - Registro de delay de 8 bits
  - Registro de sonido de 8 bits

  - Arreglo `display` de 64x32 pixeles mocromatico
  - Arreglo `keypad` con el estado de las 16 teclas del computador

- `init_ctx`
  - Inicialización de valores en los registros
  - Escritura de fonts a la memoria RAM (`context.c:load_fonts`) desde `0x50`

- `load_rom`
  - Carga de las instrucciones de la rom en la memoria, desde `0x200`

## `chip8.c`: Loop principal y Loop logico

- `start_chip8`
    1. Creación del objeto de ventana `Window win`
    2. Inicialización de este objeto (`graphics.c:init_graphics`). Si la inicialización falla, terminar
    3. Si no, inicializar las variables de estado del loop de ejecución
        - `quit`: Flag que mantiene el loop principal abierto
        - `debug_mode`: Flag que activa las opciones de debug
        - `SDL_Event e`: Variable que contiene el evento actual del loop
        - `Pixel pixel`: Variable que almacena el pixel actual a dibujar
        - variables de tiempo (`current_time`, `dt`, `last_cycle_time`)
    4. Ejecución de un ciclo de ejecución
        - Procesar el input de ese ciclo (`input.c:process_input`)
        - Ejecutar un ciclo logico del computador (`logic.c:chip_cycle`).
        - Despues de un ciclo logico, dibujar los cambios del `display` (`graphics.c:draw_display`)

    5. Finalización del loop principal
        - Liberar los recursos asociados a los graficos (`graphics.c:close_graphics`)

## `graphics.c`: Funciones que tienen que ver con la Libreria gráfica `SDL`

- Define una estructura `Window` que contiene los punteros `SDL_Window` (Puntero a la ventana) y `SDL_Surface` (Puntero a la superficie contenida por la ventana; Es donde se podra dibujar)
- Define una estructura `Pixel` que contiene un objeto `SDL_Rect` que representa un pixel en una superficie.
- `init_graphics`
  - Inicia la liberia grafica `SDL`
  - Si se inicia correctamente, se crea una `SDL_Window` con un ancho y un largo. Esta ventana se asocia a la struct creada antes.
  - Si la ventana se crea correctamente, se obtiene la superficie asociada a esta y se asocia a la struct creada antes.

- `close_graphics`
  - Se destruye la ventana y se cierra el sistema grafico de `SDL`

- `draw_display`
    1. Se recorre el plano 2D del computador
        - El objeto `SDL_Rect Pixel` es asignado a esa posicion i,j
        - Se accede al valor logico del pixel en esa posicion (desde `ctx->display`)
        - Se mapea el pixel del color correspondiente (encendido o apagado) en la superficie
  
    2. Una vez se recorre todo el plano 2D, se reflejan los cambios visuales con `SDL_UpdateWindowSurface`

## `input.c`: Procesado del input

- `process_input`
  - Se revisa el `SDL_Event` y se realiza la accion correspondiente
    - Si el evento fue `SDL_QUIT`, actualizar la variable `quit`
    - Si el evento fue presionar una tecla, seleccionar que hacer en base a la tecla presionada
      - `O`: Activar el modo debug
      - `P`: Ejecutar un ciclo logico (`logic.c:chip_cycle`) si se esta en modo debug
      - Alguna tecla del computador CHIP-8 (`1234qwerasdfzxcv`): Actualizar el arreglo `ctx->keypad`

    - Si el evento fue soltar una tecla, actualizar el arreglo `ctx->keypad`.

## `logic.c`: Funciones que simular lógica del computador CHIP-8

- 33 funciones que simulan la lógica del computador CHIP-8: Set de intrucciones del computador CHIP-8. Se pueden consultar [acá](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#instructions)

- `chip_cycle`:
  - Ejecución de un ciclo logico del computador
  1. _fetch_ del opcode que indica el program counter (esta posicionado en la parte de la memoria donde esta cargada la ROM)
  2. _decode_ del opcode
  3. Si los registros de delay y sonido siguen siendo positivos, decrementarlos

- `decode`
  - Procesar el opcode y realizar la instrucción correspondiente

## `utils.c`: Utilidades para imprimir cosas en consola
