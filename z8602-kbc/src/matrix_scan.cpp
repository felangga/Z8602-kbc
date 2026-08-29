#include "matrix_scan.h"
#include "pins.h"
#include "keytables.h"

static bool confirmedState[127] = {false};
static bool prevRawState[127] = {false};
static uint8_t activeCount = 0;

void matrixInit() {
    for (uint8_t c = 0; c < 16; c++) {
        pinMode(COL_PINS[c], INPUT); // idle Hi-Z, emulates open-drain "high"
    }
    for (uint8_t r = 0; r < 8; r++) {
        pinMode(ROW_PINS[r], INPUT_PULLUP);
    }
}

uint8_t matrixScan(KeyEvent *events) {
    bool rawState[127] = {false};

    for (uint8_t c = 0; c < 16; c++) {
        pinMode(COL_PINS[c], OUTPUT);
        digitalWrite(COL_PINS[c], LOW);
        delayMicroseconds(20); // settle time per datasheet Figure 3

        for (uint8_t r = 0; r < 8; r++) {
            uint8_t keyNum = MATRIX_KEYNUM[r][c];
            bool pressed = (digitalRead(ROW_PINS[r]) == LOW);

            // TEMP DEBUG: raw (pre-debounce) read of every mapped matrix
            // cell, printed on change only, regardless of whether the scan
            // codes for that key are filled in yet. Remove once wiring is
            // confirmed working.
            static bool lastRawCell[8][16] = {false};
            if (keyNum != 0 && pressed != lastRawCell[r][c]) {
                lastRawCell[r][c] = pressed;
                Serial.print(F("COL"));
                Serial.print(c);
                Serial.print(F("/ROW"));
                Serial.print(r);
                Serial.print(F(" (key "));
                Serial.print(keyNum);
                Serial.print(F("): "));
                Serial.println(pressed ? F("LOW (pressed)") : F("HIGH (released)"));
            }

            if (keyNum == 0) continue; // phantom / unpopulated matrix cell
            if (pressed) {
                rawState[keyNum] = true;
            }
        }

        pinMode(COL_PINS[c], INPUT); // release back to Hi-Z
    }

    uint8_t eventCount = 0;

    for (uint8_t k = 1; k <= 126; k++) {
        bool stable = (rawState[k] == prevRawState[k]);
        bool changed = (rawState[k] != confirmedState[k]);

        if (stable && changed) {
            if (rawState[k]) {
                // key going down — respect rollover limit (datasheet: max 6
                // concurrent keys, extras ignored until one releases)
                if (activeCount >= MAX_ACTIVE_KEYS) {
                    prevRawState[k] = rawState[k];
                    continue;
                }
                activeCount++;
            } else {
                activeCount--;
            }

            confirmedState[k] = rawState[k];
            events[eventCount].keyNum = k;
            events[eventCount].pressed = rawState[k];
            eventCount++;
        }

        prevRawState[k] = rawState[k];
    }

    return eventCount;
}
