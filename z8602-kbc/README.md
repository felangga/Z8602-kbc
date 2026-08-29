# z8602-kbc

Firmware for an Arduino Mega 2560 that scans a keyboard switch matrix and speaks
the PS/2 keyboard protocol to a host, as a modern replacement for a Zilog
Z8602-family keyboard microcontroller. Reference: `docs/Zilog PC Keyboard_671580545-an0087.pdf`
(AN008701-0301, "Z8602/14/15/C15/E23 Microcontrollers Control a 101/102 PC/Keyboard").

## Hardware

- **Board:** Arduino Mega 2560 (`platformio.ini` target: `megaatmega2560`)
- **Matrix:** 16 columns x 8 rows, no diodes (watch for ghosting above `MAX_ACTIVE_KEYS`)
- **Link:** bit-banged PS/2 (clock + data), open-drain via the Mega's internal pull-ups
  (no external pull-up resistors needed — see `lib/ps2dev/ps2dev.cpp`)

## Pin mapping

All pin numbers are Arduino digital pin numbers (as printed on the Mega 2560's
silkscreen), defined in `include/pins.h`.

### PS/2 host link

| Signal | Pin | DIN5 connector pin |
|---|---|---|
| Clock | 18 | 1 |
| Data | 19 | 2 |
| — | — | 4 = GND, 5 = +5V (not driven by the Mega) |

### Matrix columns (driven low one at a time, otherwise Hi-Z)

| Logical | Pin | | Logical | Pin |
|---|---|---|---|---|
| COL0 | 22 | | COL8 | 30 |
| COL1 | 23 | | COL9 | 31 |
| COL2 | 24 | | COL10 | 32 |
| COL3 | 25 | | COL11 | 33 |
| COL4 | 26 | | COL12 | 34 |
| COL5 | 27 | | COL13 | 35 |
| COL6 | 28 | | COL14 | 36 |
| COL7 | 29 | | COL15 | 37 |

### Matrix rows (read with `INPUT_PULLUP`, active-low when pressed)

| Logical | Pin |
|---|---|
| ROW0 | 38 |
| ROW1 | 39 |
| ROW2 | 40 |
| ROW3 | 41 |
| ROW4 | 42 |
| ROW5 | 43 |
| ROW6 | 44 |
| ROW7 | 45 |

### Status LEDs (active-low)

| LED | Pin |
|---|---|
| Scroll Lock | 46 |
| Num Lock | 47 |
| Caps Lock | 48 |

## Building

This is a [PlatformIO](https://platformio.org/) project.

```sh
pio run          # build
pio run -t upload  # flash
pio device monitor -b 115200  # serial monitor, if debug logging is re-enabled
```

## Architecture

| File | Responsibility |
|---|---|
| `src/main.cpp` | `setup()`/`loop()`, boot LED animation, dispatches matrix events to the PS/2 link, handles the host's LED/typematic commands |
| `src/matrix_scan.cpp` | Scans the 16x8 matrix, debounces (a state must read identically twice before it's reported), emits make/break `KeyEvent`s, enforces 6-key rollover |
| `include/keytables.h` | `MATRIX_KEYNUM[row][col]` (physical position -> key number) and `SCANCODE2[keyNum]` (key number -> PS/2 Scan Code Set 2 byte) |
| `src/typematic.cpp` | Typematic delay/rate timing per the host's `F3h` command |
| `lib/ps2dev/` | Vendored, locally patched PS/2 device library (adds `last_typematic_byte` exposure; fixes a `keyboard_pausebreak()` byte bug — see git history) |

## Matrix calibration status

`MATRIX_KEYNUM` started as a transcription of the datasheet's reference Figure 5,
but this board's actual matrix wiring does **not** match the Zilog reference
design — rows have needed individual, empirically-verified corrections (some
shifted by a column, some cell-by-cell). Treat any cell that hasn't been
confirmed by testing as unreliable. Current confidence per row, from the most
recent pass:

- **Confirmed / high confidence:** ROW3, ROW4, ROW5 (verified via multiple
  independent key presses each)
- **Partially confirmed:** ROW1, ROW2, ROW6, ROW7 (some cells tested and
  corrected, others still original guesses or unknown/`0`)
- **Untested:** ROW0

An unmapped cell (`0` in `MATRIX_KEYNUM`, or `{0x00, false}` in `SCANCODE2`)
is silently skipped — pressing that physical key does nothing rather than
sending the wrong code.

### Special-cased keys (not representable as a plain `{code, isExtended}` pair)

- **Print Screen** (key 124) — two-code sequence, handled in `main.cpp` via
  `ps2dev`'s `keyboard_press_printscreen()` / `keyboard_release_printscreen()`.
- **Pause/Break** (key 126) — one-shot, make-only, not typematic; handled via
  `ps2dev`'s `keyboard_pausebreak()`.
- **Keys 75, 76, 79-81, 83-86, 89** (Insert/Delete/Home/End/PgUp/PgDn/arrows)
  — the datasheet defines these with a NumLock-dependent "fake shift"
  make/break injected around the real code (an AT-keyboard-era workaround).
  This firmware sends just the plain base code, which is sufficient for
  modern hosts.

## Testing a physical key position

1. Find the column/row pin pair for the position you want to test (see the
   pin tables above).
2. Bridge that COL pin to that ROW pin momentarily (or press the actual
   switch, once wired).
3. Watch the serial monitor at 115200 baud (temporary debug prints can be
   added back into `matrixScan()`/`sendKeyEvent()` if not currently present)
   to see which key number the position reports, then cross-check/correct
   `MATRIX_KEYNUM` and `SCANCODE2` in `include/keytables.h` accordingly.
