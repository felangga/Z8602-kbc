#pragma once

#include <Arduino.h>

// Max simultaneously tracked keys. Z8602 limits to 6 (6-key rollover);
// we don't need the register economy it needed, so this can go higher if
// your matrix supports it without ghosting (no diodes = watch for that).
constexpr uint8_t MAX_ACTIVE_KEYS = 6;

struct KeyEvent {
    uint8_t keyNum;   // 1..126, matches Figure 5 / SCANCODE2 index
    bool pressed;      // true = make, false = break
};

void matrixInit();

// Scans the full 16x8 matrix once. Returns number of edge events (make/break)
// found this call, filling `events` (must hold at least MAX_ACTIVE_KEYS*2).
// Debounce: a state must be read identically twice in a row before it's
// reported as an edge, matching the "detected twice" rule in the datasheet.
uint8_t matrixScan(KeyEvent *events);
