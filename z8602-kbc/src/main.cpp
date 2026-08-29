#include <Arduino.h>
#include <ps2dev.h>

#include "pins.h"
#include "matrix_scan.h"
#include "keytables.h"
#include "typematic.h"

PS2dev keyboard(PS2_CLOCK_PIN, PS2_DATA_PIN);

static unsigned long lastScanAt = 0;
constexpr unsigned long SCAN_INTERVAL_MS = 4; // datasheet: 4.17ms

static void applyLeds(uint8_t ledsByte) {
    // keyboard_handle's leds byte: bit0=Scroll bit1=Num bit2=Caps (standard
    // PS/2 ED-command bit order)
    digitalWrite(LED_SCROLL_PIN, (ledsByte & 0x01) ? LOW : HIGH); // active-low per Fig 4a
    digitalWrite(LED_NUM_PIN,    (ledsByte & 0x02) ? LOW : HIGH);
    digitalWrite(LED_CAPS_PIN,   (ledsByte & 0x04) ? LOW : HIGH);
}

static void sendKeyEvent(uint8_t keyNum, bool pressed) {
    if (keyNum == 0 || keyNum > 126) return;


    if (keyNum == 124) {
        if (pressed) {
            keyboard.keyboard_press_printscreen();
            typematicKeyDown(keyNum);
        } else {
            keyboard.keyboard_release_printscreen();
            typematicKeyUp(keyNum);
        }
        return;
    }
    if (keyNum == 126) {
        // Datasheet: make-only, no separate break code, not typematic.
        if (pressed) keyboard.keyboard_pausebreak();
        return;
    }

    KeyCode kc = SCANCODE2[keyNum];
    if (kc.code == 0x00) return; // unmapped in table — fill keytables.h

    if (pressed) {
        if (kc.isExtended) keyboard.keyboard_press_special(kc.code);
        else keyboard.keyboard_press(kc.code);
        typematicKeyDown(keyNum);
    } else {
        if (kc.isExtended) keyboard.keyboard_release_special(kc.code);
        else keyboard.keyboard_release(kc.code);
        typematicKeyUp(keyNum);
    }
}

// LED MOD 
// KITT/Larson-scanner boot animation across Num/Caps/Scroll — bounces a
// single lit LED back and forth a few times before settling to the real
// lock-status LEDs once the host starts talking to us.
static void startupLightShow() {
    const uint8_t ledPins[3] = {LED_NUM_PIN, LED_CAPS_PIN, LED_SCROLL_PIN};
    const uint8_t bounce[4] = {0, 1, 2, 1}; // num -> caps -> scroll -> caps -> (repeat)
    constexpr unsigned long STEP_MS = 120;
    constexpr uint8_t CYCLES = 3;

    for (uint8_t cycle = 0; cycle < CYCLES; cycle++) {
        for (uint8_t step = 0; step < 4; step++) {
            for (uint8_t p = 0; p < 3; p++) {
                digitalWrite(ledPins[p], (p == bounce[step]) ? LOW : HIGH); // active-low
            }
            delay(STEP_MS);
        }
    }
    for (uint8_t p = 0; p < 3; p++) digitalWrite(ledPins[p], HIGH); // all off
}

void setup() {
    pinMode(LED_SCROLL_PIN, OUTPUT);
    pinMode(LED_NUM_PIN, OUTPUT);
    pinMode(LED_CAPS_PIN, OUTPUT);
    applyLeds(0);

    startupLightShow();

    matrixInit();
    keyboard.keyboard_init();

    delay(500);       // let host settle after power-up
    keyboard.write(0xAA); // BAT-pass code, standard PS/2 power-on self-test result
}

static uint8_t lastSeenTypematicByte = 0;

void loop() {
    uint8_t leds = 0;
    if (keyboard.available()) {
        // keyboard_handle() answers ED/EE/F0/F2/F3/F4/F5/F6/FE/FF per Table 4.
        keyboard.keyboard_handle(&leds);
        applyLeds(leds);

        // last_typematic_byte is a z8602-kbc patch to lib/ps2dev — upstream
        // acks F3h (set typematic rate/delay) but drops the payload byte.
        if (keyboard.last_typematic_byte != lastSeenTypematicByte) {
            lastSeenTypematicByte = keyboard.last_typematic_byte;
            typematicSetFromByte(lastSeenTypematicByte);
        }
    }

    unsigned long now = millis();
    if (now - lastScanAt >= SCAN_INTERVAL_MS) {
        lastScanAt = now;

        KeyEvent events[MAX_ACTIVE_KEYS * 2];
        uint8_t n = matrixScan(events);
        for (uint8_t i = 0; i < n; i++) {
            sendKeyEvent(events[i].keyNum, events[i].pressed);
        }
    }

    uint8_t repeatKey = typematicPoll();
    if (repeatKey != 0) {
        KeyCode kc = SCANCODE2[repeatKey];
        if (kc.code != 0x00) {
            if (kc.isExtended) keyboard.keyboard_press_special(kc.code);
            else keyboard.keyboard_press(kc.code);
        }
    }
}
