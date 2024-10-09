constexpr byte L_PIN[5] = {8, 9, 10, 11, 12};

// De acordo com o enunciado da questão, teremos uma frequência base e, sobre essa
// frequência base, iremos multiplicá-la por uma constante para que seja modificada
constexpr double FREQ_GAIN[] = {0.25, 0.5, 1.0, 1.5, 2.0};

// frequência base, em ciclo por milisegundo, usada para referência de piscamento
constexpr double FREQ_BASE{1.0/1000.0};

// Período que o LED pisca (acede e apaga) dada a frequência
// O LED ficará aceso por L_PER/2 ms e apagado por L_PER/2 ms
//
// NOTE: Essa array será enchida com elementos do tipo double
// mas o Arduino irá arredondar os valores decimais (ocorrerá
// na função setup()) implicitamente. Ocorrerá perda de info
// porém para esse projeto essa perda será quase desprezível
unsigned long L_PER[5];

// Timer individual para cada LED, equivalente ao "tempo inicial"
// se esse programa tivesse só 1 LED.
unsigned long L_TIMER[5];

// Guarda o último estado do LED
bool L_L_STATUS[5];

// Retorna o período em função da frequência e de seu valor
// de ganho (veja o enunciado da questão). Equivale a 1/(fb * f)
// onde f = 0.25 ou 0.5 ou ... etc
double periodo(byte index) {
    return 1 / (FREQ_BASE * FREQ_GAIN[index]);
}

// Verifica se o tempo atual está contido no intervalo fechado
// t_atual é elemento de [0, t_max]? (retornará verdadeiro ou falso).
bool dentro_do_ciclo(unsigned long t_atual, unsigned long t_max) {
   return (0 <= t_atual && t_atual <= t_max) ? true : false;
}

void blink(byte led_index, unsigned long dt) {

    // Tome como exemplo um período que tenha exatamente 1000ms e considere que
    // o LED deva piscar (ou seja, ligar e desligar) 1x. Esse if verifica se o
    // tempo dt (millis() - timer_do_led) está na primeira metade do período
    // (ou seja, entre 0 e 500 ms), o que forçará com que o LED acenda caso
    // esteja desligado, o else é análogo: verificará se dt > 500ms e desligará
    // o LED. Cada LED tem informações específicas dependendo do index, então
    // essa função funciona para uma quantidade n finita de LEDs.
    if(L_L_STATUS[led_index] == LOW && L_PER[led_index] / 2 <= dt) {
        digitalWrite(L_PIN[led_index], HIGH);
        L_L_STATUS[led_index] = HIGH;
    } else {
        digitalWrite(L_PIN[led_index], LOW);
        L_L_STATUS[led_index] = LOW;
    }
}

void setup() {
    // Inicializamos todas as arrays de forma sincronizada com um simples for-loop
    // nesse contexto, talvez iniciar as arrays com 0 não seja necessário, mas é
    // bom para evitar algum bug obscuro de lógica.
    for(byte i = 0; i < 5; i++) {
        pinMode(L_PIN[i], OUTPUT);
        L_PER[i] = periodo(i);
        L_TIMER[i] = 0;
        L_L_STATUS[i] = false;
    }
}

void loop() {

    // Percorreremos todos os LEDs, verificaremos se o intervalo de atuação é de
    // interesse e rodamos a função blink para cada um dos LEDs.
    for(byte i = 0; i < 5; i++) {
        if(dentro_do_ciclo(millis() - L_TIMER[i], L_PER[i])) {
            blink(i, millis() - L_TIMER[i]);
        } else {
          // Ainda no exemplo anterior, se dt > 1000ms, não nos interessa mais
          // ter valores de dt acima disso, já que nosso LED está restringido ao
          // intervalo de ação [0, 1000]: ligado quando dt está em [0,500] e
          // desligado quando dt está em [500, 1000], portanto podemos "resetar"
          // o timer do LED para que dt volte para 0 novamente.
          L_TIMER[i] = millis();
        }
    }
}
