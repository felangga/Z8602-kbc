#pragma once

#include <Arduino.h>

void typematicSetFromByte(uint8_t rateDelayByte);

void typematicKeyDown(uint8_t keyNum);
void typematicKeyUp(uint8_t keyNum);

// Call every loop iteration; returns the key number to re-fire a Make code
// for, or 0 if nothing is due yet.
uint8_t typematicPoll();
