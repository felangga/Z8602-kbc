#pragma once

#include <Arduino.h>

struct KeyCode {
    uint8_t code;        // scan code set 2 byte (without E0 prefix)
    bool isExtended;     // true if this key's real code is E0-prefixed
};

constexpr uint8_t MATRIX_KEYNUM[8][16] = {
    /* ROW0 */ {  0,  0,110, 45,115, 35, 36,116,   0,117, 41,  0, 99,104, 83, 60},
    /* ROW1 */ {  0, 44, 16, 30,114, 21, 22, 15, 118, 28, 27, 92, 97,102,  0,  0},
    /* ROW2 */ { 58,  0,  1,112,113,  6,  7,120, 119, 13, 12, 76, 75, 85, 80,  0},
    /* ROW3 */ {  0,  0,  2,  3,  4,  5,  8,121,  10,  9, 11,122,123, 86, 81,124},
    /* ROW4 */ {  0,  0, 17, 18, 19, 20, 23,  0,  25, 24, 26, 91, 96,101,106,125},
    /* ROW5 */ {  0,  0, 31, 32, 33, 34, 37, 29,  39, 38, 40, 93, 98,103,108,  0},
    /* ROW6 */ { 64, 57, 46, 47, 48, 49, 52, 43,  54, 53, 42, 90, 95,100,126,  0},
    /* ROW7 */ {  0,  0,  0,  0,  0, 50, 51, 61,   0,  0, 55, 84, 89,105, 79, 62},
};

constexpr KeyCode SCANCODE2[127] = {
    /*   0 */ {0x00, false}, // unused/phantom

    // Number row + backspace
    /*   1 */ {0x0E, false}, // ` ~
    /*   2 */ {0x16, false}, // 1
    /*   3 */ {0x1E, false}, // 2
    /*   4 */ {0x26, false}, // 3
    /*   5 */ {0x25, false}, // 4
    /*   6 */ {0x2E, false}, // 5
    /*   7 */ {0x36, false}, // 6
    /*   8 */ {0x3D, false}, // 7
    /*   9 */ {0x3E, false}, // 8
    /*  10 */ {0x46, false}, // 9
    /*  11 */ {0x45, false}, // 0
    /*  12 */ {0x4E, false}, // - _
    /*  13 */ {0x55, false}, // = +
    /*  14 */ {0x00, false}, // (unused key number)
    /*  15 */ {0x66, false}, // Backspace

    // Tab row
    /*  16 */ {0x0D, false}, // Tab
    /*  17 */ {0x15, false}, // Q
    /*  18 */ {0x1D, false}, // W
    /*  19 */ {0x24, false}, // E
    /*  20 */ {0x2D, false}, // R
    /*  21 */ {0x2C, false}, // T
    /*  22 */ {0x35, false}, // Y
    /*  23 */ {0x3C, false}, // U
    /*  24 */ {0x43, false}, // I
    /*  25 */ {0x44, false}, // O
    /*  26 */ {0x4D, false}, // P
    /*  27 */ {0x54, false}, // [ {
    /*  28 */ {0x5B, false}, // ] }
    /*  29 */ {0x5D, false}, // \ | (101-key only)
    /*  30 */ {0x58, false}, // Caps Lock

    // Home row
    /*  31 */ {0x1C, false}, // A
    /*  32 */ {0x1B, false}, // S
    /*  33 */ {0x23, false}, // D
    /*  34 */ {0x2B, false}, // F
    /*  35 */ {0x34, false}, // G
    /*  36 */ {0x33, false}, // H
    /*  37 */ {0x3B, false}, // J
    /*  38 */ {0x42, false}, // K
    /*  39 */ {0x4B, false}, // L
    /*  40 */ {0x4C, false}, // ; :
    /*  41 */ {0x52, false}, // ' "
    /*  42 */ {0x00, false}, // 102-key-only key — see note above (Table 2 looks duplicated/suspect)
    /*  43 */ {0x5A, false}, // Enter

    // Bottom letter row
    /*  44 */ {0x12, false}, // Left Shift
    /*  45 */ {0x61, false}, // \ | (102-key ISO-only extra key)
    /*  46 */ {0x1A, false}, // Z
    /*  47 */ {0x22, false}, // X
    /*  48 */ {0x21, false}, // C
    /*  49 */ {0x2A, false}, // V
    /*  50 */ {0x32, false}, // B
    /*  51 */ {0x31, false}, // N
    /*  52 */ {0x3A, false}, // M
    /*  53 */ {0x41, false}, // , <
    /*  54 */ {0x49, false}, // . >
    /*  55 */ {0x4A, false}, // / ?
    /*  56 */ {0x00, false}, // (unused key number)
    /*  57 */ {0x59, false}, // Right Shift

    // Bottom row (ctrl/alt/space)
    /*  58 */ {0x14, false}, // Left Ctrl
    /*  59 */ {0x00, false}, // (unused key number)
    /*  60 */ {0x11, false}, // Left Alt
    /*  61 */ {0x29, false}, // Space
    /*  62 */ {0x11, true},  // Right Alt (E0 11)
    /*  63 */ {0x00, false}, // (unused key number)
    /*  64 */ {0x14, true},  // Right Ctrl (E0 14)

    // 65..89: gaps, plus the nav cluster (Insert/Delete/Home/End/PgUp/
    // PgDn/arrows). These are filled with the plain E0-prefixed base code
    // (no NumLock-dependent "fake shift" injection — see the file-level
    // note; modern hosts don't need that AT-era workaround).
    /*  65 */ {0x00, false},
    /*  66 */ {0x00, false},
    /*  67 */ {0x00, false},
    /*  68 */ {0x00, false},
    /*  69 */ {0x00, false},
    /*  70 */ {0x00, false},
    /*  71 */ {0x00, false},
    /*  72 */ {0x00, false},
    /*  73 */ {0x00, false},
    /*  74 */ {0x00, false},
    /*  75 */ {0x70, true},  // Insert (E0 70)
    /*  76 */ {0x71, true},  // Delete (E0 71)
    /*  77 */ {0x00, false},
    /*  78 */ {0x00, false},
    /*  79 */ {0x6B, true},  // Left Arrow (E0 6B)
    /*  80 */ {0x6C, true},  // Home (E0 6C)
    /*  81 */ {0x69, true},  // End (E0 69)
    /*  82 */ {0x00, false},
    /*  83 */ {0x75, true},  // Up Arrow (E0 75)
    /*  84 */ {0x72, true},  // Down Arrow (E0 72)
    /*  85 */ {0x7D, true},  // Page Up (E0 7D)
    /*  86 */ {0x7A, true},  // Page Down (E0 7A)
    /*  87 */ {0x00, false},
    /*  88 */ {0x00, false},
    /*  89 */ {0x74, true},  // Right Arrow (E0 74)

    // Keypad
    /*  90 */ {0x77, false}, // Num Lock
    /*  91 */ {0x6C, false}, // Keypad 7
    /*  92 */ {0x6B, false}, // Keypad 4
    /*  93 */ {0x69, false}, // Keypad 1
    /*  94 */ {0x00, false}, // (unused key number)
    /*  95 */ {0x4A, true},  // Keypad / (E0 4A) — confirmed via MATRIX_KEYNUM ROW6/COL12
    /*  96 */ {0x75, false}, // Keypad 8
    /*  97 */ {0x73, false}, // Keypad 5
    /*  98 */ {0x72, false}, // Keypad 2
    /*  99 */ {0x70, false}, // Keypad 0
    /* 100 */ {0x7C, false}, // Keypad *
    /* 101 */ {0x7D, false}, // Keypad 9
    /* 102 */ {0x74, false}, // Keypad 6
    /* 103 */ {0x7A, false}, // Keypad 3
    /* 104 */ {0x71, false}, // Keypad .
    /* 105 */ {0x7B, false}, // Keypad -
    /* 106 */ {0x79, false}, // Keypad +
    /* 107 */ {0x00, false}, // (unused key number)
    /* 108 */ {0x5A, true},  // Keypad Enter (E0 5A)
    /* 109 */ {0x00, false}, // (unused key number)

    // Function row + locks
    /* 110 */ {0x76, false}, // Esc
    /* 111 */ {0x00, false}, // (unused key number)
    /* 112 */ {0x05, false}, // F1
    /* 113 */ {0x06, false}, // F2
    /* 114 */ {0x04, false}, // F3
    /* 115 */ {0x0C, false}, // F4
    /* 116 */ {0x03, false}, // F5
    /* 117 */ {0x0B, false}, // F6
    /* 118 */ {0x83, false}, // F7
    /* 119 */ {0x0A, false}, // F8
    /* 120 */ {0x01, false}, // F9
    /* 121 */ {0x09, false}, // F10
    /* 122 */ {0x78, false}, // F11
    /* 123 */ {0x07, false}, // F12
    /* 124 */ {0x00, false}, // special (Print Screen) — see note above
    /* 125 */ {0x7E, false}, // Scroll Lock
    /* 126 */ {0x00, false}, // special (Pause/Break) — see note above
};
