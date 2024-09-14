// Conta de 000 até 111 e utiliza os bits como saída para um LED RGB

#include "Arduino.h"

constexpr int OUT_LED_R{9};
constexpr int OUT_LED_G{10};
constexpr int OUT_LED_B{11};

int RGB = 0;

void setup() {
    pinMode(OUT_LED_R, OUTPUT);
    pinMode(OUT_LED_G, OUTPUT);
    pinMode(OUT_LED_B, OUTPUT);
}

// NOTE: número hardcodado no for loop
void loop() {
    for(auto i = 0; i <= 7; i++) {
        RGB = i;

        /*
          Usar shift de bit simplifica bastante o código

          1. Joga o enésimo bit para a posição de bit menos
          significativo

          2. Executa um AND com o bitmask 0b001, extraíndo o
          bit procurado
        */
        digitalWrite(OUT_LED_R, (RGB >> 2) & 0b001);
        digitalWrite(OUT_LED_G, (RGB >> 1) & 0b001);
        digitalWrite(OUT_LED_B, (RGB >> 0) & 0b001);

        delay(1000);
    }
}
