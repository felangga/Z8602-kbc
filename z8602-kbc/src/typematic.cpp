#include "typematic.h"

// Standard PS/2 typematic rate/delay byte (set via command F3h):
//   bit 7    = 0 (reserved)
//   bits 6:5 = delay   (0=250ms 1=500ms 2=750ms 3=1000ms)
//   bits 4:0 = rate    period(ms) = (8 + (rate&7)) * 2^((rate>>3)&3) * 4.17
// This is the standard AT/PS2 formula (Table 2/Fig 9 only defines the
// delay steps explicitly; rate steps follow the universal PS/2 spec).

static const uint16_t DELAY_MS[4] = {250, 500, 750, 1000};

static uint16_t delayMs = 500;
static uint16_t periodMs = 91; // ~10.9 cps default

static uint8_t typematicKey = 0;
static unsigned long dueAt = 0;

void typematicSetFromByte(uint8_t rateDelayByte) {
    uint8_t delayBits = (rateDelayByte >> 5) & 0x03;
    uint8_t rateBits = rateDelayByte & 0x1F;

    delayMs = DELAY_MS[delayBits];

    uint8_t a = (rateBits >> 3) & 0x03;
    uint8_t b = rateBits & 0x07;
    periodMs = (uint16_t)((8 + b) * (1 << a) * 4.17f);
}

void typematicKeyDown(uint8_t keyNum) {
    typematicKey = keyNum;
    dueAt = millis() + delayMs;
}

void typematicKeyUp(uint8_t keyNum) {
    if (typematicKey == keyNum) {
        typematicKey = 0;
    }
}

uint8_t typematicPoll() {
    if (typematicKey == 0) return 0;
    if ((long)(millis() - dueAt) < 0) return 0;

    dueAt = millis() + periodMs;
    return typematicKey;
}
