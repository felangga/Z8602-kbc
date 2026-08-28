#pragma once

#include <Arduino.h>

// Delay/rate table indices per TYPEMATIC_RATE register layout (Figure 9).
// Delay: bits 6:5 -> 250/500/750/1000 ms. Rate: bits 4:0 -> 30.0..2.0 cps.
void typematicSetFromByte(uint8_t rateDelayByte);

void typematicKeyDown(uint8_t keyNum);
void typematicKeyUp(uint8_t keyNum);

// Call every loop iteration; returns the key number to re-fire a Make code
// for, or 0 if nothing is due yet.
uint8_t typematicPoll();
