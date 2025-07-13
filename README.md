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

You can run the tests in the `tests/` folder, or play some games provided on the `roms/` folder. You can also search for compatible `.ch8` files and use this emulator on them.

## Debug mode

To activate debug mode during execution, press the `O` key.  
While in debug mode, press the `P` key to step through each instruction.

If you need the debug mode activated from the start of an execution, you can change the value of the `debug_mode` flag on `src/chip8/chip.c:25`.

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

## Tests Status

`.ch8` files to test the correct behaviour of the emulator.

| Test Name             | Status |              Comments               |
|-----------------------|:------:|-------------------------------------|
| `1-chip-logo.ch8`     | ✅     |                                     |
| `2-ibm-logo.ch8`      | ✅     |                                     |
| `3-corax+.ch8`        | ✅     |                                     |
| `4-flags.ch8`         | ✅     |                                     |
| `5-quirks.ch8`        | ❌     | Some quirks not working as expected |
| `6-keypad.ch8`        | ❌     | `FX0A` not working as expected      |
| `7-beep.ch8`          | 🟡     | Sound system not implemented        |

## About the Code

For a detailed explanation of the flow and how this code works, please refer to [this document](docs/flow_of_how_it_works) (spanish only).

## Sources Used

- [A Guide on writing a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#prerequisites). Overall specs and documentation about the CHIP-8 Computer.

- [A Video on writing a CHIP-8 emulator](https://www.youtube.com/watch?v=YtSgV3gY3fs&t=88s). This was the video that kicked my drive to make this emulator myself.

- [SDL graphics library tutorial](https://lazyfoo.net/tutorials/SDL/index.php#Hello%20SDL). On how to create a basic window.

- [Test files](https://github.com/Timendus/chip8-test-suite). Test files to check the behaviour of the emulator.
