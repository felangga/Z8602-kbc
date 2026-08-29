#pragma once

#include <Arduino.h>

// Maximum key combinations
constexpr uint8_t MAX_ACTIVE_KEYS = 6;

struct KeyEvent {
    uint8_t keyNum;    // 1..126, matches Figure 5 / SCANCODE2 index
    bool pressed;      // true = make, false = break
};

void matrixInit();
uint8_t matrixScan(KeyEvent *events);
