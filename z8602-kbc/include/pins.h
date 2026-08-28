#pragma once

#include <Arduino.h>

// PS/2 link (host connector: DIN5 pin1=CLK, pin2=DATA, pin4=GND, pin5=+5V)
#define PS2_CLOCK_PIN 18
#define PS2_DATA_PIN  19

// Matrix: 16 columns (driven low one at a time), 8 rows (read back)
// Physical wiring order here is arbitrary logical COL0..15 / ROW0..7 -
// map to actual keyboard PCB traces via keytables.h, not by pin number.
constexpr uint8_t COL_PINS[16] = {
    22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37
};

constexpr uint8_t ROW_PINS[8] = {
    38, 39, 40, 41, 42, 43, 44, 45
};

#define LED_SCROLL_PIN 46
#define LED_NUM_PIN    47
#define LED_CAPS_PIN   48
