#include "speaker.h"

#include <Arduino.h>

Speaker::Speaker(int pin) : pin(pin) {
    pinMode(pin, OUTPUT);
}

void Speaker::playSound() {
    digitalWrite(pin, HIGH);
}

void Speaker::muteSound() {
    digitalWrite(pin, LOW);
}

