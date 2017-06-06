#include "keyboard.h"

Keyboard::Keyboard(byte rowPins[], byte colPins[]) : keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS) {}

char Keyboard::getKeyPressed() {
    char theKey = keypad.getKey();
    if (theKey) {
        key = theKey;
    } else {
        if (keypad.getState() != PRESSED and keypad.getState() != HOLD) {
            key = NO_KEY_PRESSED;
        }
    }
    return key;
}

