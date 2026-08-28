#pragma once

#include <Arduino.h>

// ============================================================================
// TEMPLATE — NOT VERIFIED KEY DATA.
//
// The two tables below (matrix layout, scan-code lookup) must be filled in
// and cross-checked directly against the Zilog AN008701-0301 PDF:
//   - MATRIX_KEYNUM[row][col]  <- Figure 5, "101/102 Keyboard Matrix"
//   - SCANCODE2_MAKE/BREAK[]   <- Table 2, "Scan Code Set" (use the
//     "Scan Code Set 2" columns; that's the default this firmware emits)
//
// Key numbers run 1..126 per Figure 5. Row/col indices below are LOGICAL
// (ROW0..7 / COL0..15) — match them to your own harness wiring, not to the
// Z8602's P-port numbering.
//
// Codes with an 0xE0 prefix (extended keys: arrows, right ctrl/alt, etc.)
// are marked isExtended = true and sent via keyboard_press_special() /
// keyboard_release_special() in main.cpp instead of the plain variants.
// ============================================================================

struct KeyCode {
    uint8_t code;       // scan code set 2 byte (without E0 prefix)
    bool isExtended;     // true if this key's real code is E0-prefixed
};

// Example rows only — format demonstration, not real layout.
// Replace 0 with the real key numbers laid out per Figure 5 (8 rows x 16 cols).
constexpr uint8_t MATRIX_KEYNUM[8][16] = {
    /* ROW0 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    /* ROW1 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    /* ROW2 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    /* ROW3 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    /* ROW4 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    /* ROW5 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    /* ROW6 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
    /* ROW7 */ {0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
};

// Indexed by key number 1..126 (index 0 unused/phantom).
// Format demo only — fill from Table 2, Scan Code Set 2 "Make Code" column.
constexpr KeyCode SCANCODE2[127] = {
    /*   0 (unused/phantom) */ {0x00, false},
    // Fill 1..126 here, e.g.:
    // /*   1 */ {0x??, false},
    // ...
};
