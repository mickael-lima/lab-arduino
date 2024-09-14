// Contamos, de forma limitada,de 000 até 111 e traduzimos isso em forma de uma tabela RGB,
// onde R = MSB e B = LSB. O código abaixo nos permite percorrer essa tabela com o pot 10k

#include "Arduino.h"

constexpr int IN_POT{A0};

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
  int pot_value{analogRead(IN_POT)};
  RGB = map(pot_value, 0, 1023, 0, 7);

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
}
