// --- Sincronizando 5 LEDs com frequências de piscamento diferentes ---

constexpr byte LED[] = {9, 10, 11, 12, 13};

constexpr float FREQ_BASE{1.0/1000.0}; // em milissegundos
constexpr float FREQ_GAIN[] = {0.25, 0.5, 1.0, 1.75, 2.0};

// poderia ser só um float, mas permite flexibilidade
constexpr float TRANS_DE_ESTADO[] = {0.5, 0.5, 0.5, 0.5, 0.5};

// cada LED vai ter um valor de periodo diferente para piscar, não precisa ser long
unsigned PER_LED[5];

// tempo atual de cada LED
unsigned long t[5];

// cálculo de período em função da frequência individual atribuido a cada LED
// vai ser transformado para unsigned long, pode levar imprecisão, portanto
// antes de castar para unsigned int, soma 0.5 para decidir para onde será
// arredondado
unsigned periodo(byte led_index) {
    return 1 / (FREQ_BASE * FREQ_GAIN[led_index]) + 0.5;
}

// verifica se valor está no intervalo [a, b[
bool no_intervalo(unsigned long valor, unsigned long a, unsigned long b) {
    return a <= valor && valor <= b ? true : false;
}

// verifica se o LED está no período apropriado para ficar aceso
bool periodo_aceso(byte led_index) {
    return (no_intervalo(millis() - t[led_index], 0, PER_LED[led_index] * TRANS_DE_ESTADO[led_index])) ? true : false;
}

void setup() {
    for(byte i = 0; i < 5; i++) {
        pinMode(LED[i], OUTPUT);
        PER_LED[i] = periodo(i);
    }
}

void loop() {
    for(byte i = 0; i < 5; i++) {
        if(periodo_aceso(i))
          digitalWrite(LED[i], HIGH);
        else
          digitalWrite(LED[i], LOW);

        if(millis() - t[i] >= PER_LED[i])
          t[i] = millis();
    }
}
