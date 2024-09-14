#include "Arduino.h"

constexpr int IN_POT{A0};
constexpr int OUT_LED{11}; // NOTE: PWM

void setup() {
    pinMode(OUT_LED, OUTPUT);
}

void loop() {
    auto pot_value{analogRead(IN_POT)};
    auto led_value = map(pot_value, 0, 1023, 0, 255); // Escala [0-1023] para [0-255], cumprindo os requisitos de PWM do Arduino

    digitalWrite(OUT_LED, led_value);
}
