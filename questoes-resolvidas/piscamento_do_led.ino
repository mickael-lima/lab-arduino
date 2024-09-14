constexpr int OUT_LED{11};
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
