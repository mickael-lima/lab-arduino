/*
Arquivo em cpp para facilitar minha vida com o Emacs

22) Implemente um projeto microcontrolado baseado em Arduino R3 em que se utiliza um
potenciêmtro linear de 10KΩ para controlar a frequência de piscamento de um LED
monocromático de 5mm

São necessários os seguintes componentes (além do descrito na questão)

- Um resistor de resistência R maior que 5 v / 3 * 10^-3 > 166 ohm para o LED (220)
*/

#include "Arduino.h"

constexpr int OUT_LED{11}; // obs: testando inicialização de classe no arduino, remover depois
constexpr int IN_POT{A0};

int valor_do_pot{0};
int f_de_piscamento{0};

// desliga e liga o LED em um delay de t ms
void blink(int pin, int ms_delay) {
    digitalWrite(pin, LOW);
    delay(ms_delay);

    digitalWrite(pin, HIGH);
    delay(ms_delay);
}

void setup() {
    pinMode(OUT_LED, OUTPUT);
}

void loop() {
    valor_do_pot = analogRead(IN_POT);
    f_de_piscamento = map(valor_do_pot, 0, 1023, 0, 255);

    blink(OUT_LED, f_de_piscamento);
}
