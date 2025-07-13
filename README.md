# CHIP-8 Emulator

Daniel Villaseñor C.

## Installation

Make sure to first install the SDL graphics library.

```bash
sudo apt install libsdl2-dev
```

Then compile the program using `make`.

## Execution

```bash
./chip8 <path/to/some/rom>
```

You can run the tests in the `tests/` folder, or play some games provided on the `roms/` folder. You can also search for compatible `.ch8` and use this emulator on them.

## Input Keys

```bash
Keypad       Keyboard
+-+-+-+-+    +-+-+-+-+
|1|2|3|C|    |1|2|3|4|
+-+-+-+-+    +-+-+-+-+
|4|5|6|D|    |Q|W|E|R|
+-+-+-+-+ => +-+-+-+-+
|7|8|9|E|    |A|S|D|F|
+-+-+-+-+    +-+-+-+-+
|A|0|B|F|    |Z|X|C|V|
+-+-+-+-+    +-+-+-+-+
```

## Sources Used

- [A Guide on writing a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#prerequisites). Overall specs and documentation about the CHIP-8 Computer.

- [A Video on writing a CHIP-8 emulator](https://www.youtube.com/watch?v=YtSgV3gY3fs&t=88s). This was the video that kicked my drive to make this emulator myself.

- [SDL graphics library tutorial](https://lazyfoo.net/tutorials/SDL/index.php#Hello%20SDL). On how to create a basic window.

- [Test files](https://github.com/Timendus/chip8-test-suite). For testing the emulator.
