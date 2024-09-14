constexpr byte IN_POT_R{A0};
constexpr byte IN_POT_G{A1};
constexpr byte IN_POT_B{A2};

// NOTE: Os pinos de saída devem suportar PWM
constexpr byte OUT_LED_R{9};
constexpr byte OUT_LED_G{10};
constexpr byte OUT_LED_B{11};

// O output no PWM tá limitando no intervalo 0-255
struct RGB {
    byte R;
    byte G;
    byte B;
};

struct RGB actual_color;

void get_color_from_pots(struct RGB &color) {

    // NOTE: O analogRead retorna valores no intervalo [0-1023]
    // pode ter janelas de otimização de armazenamento
    int analog_R = analogRead(IN_POT_R);
    int analog_G = analogRead(IN_POT_G);
    int analog_B = analogRead(IN_POT_B);

    color.R = map(analog_R, 0, 1023, 0, 255);
    color.G = map(analog_G, 0, 1023, 0, 255);
    color.B = map(analog_B, 0, 1023, 0, 255);
}

void write_color_to_led(struct RGB &color) {
    analogWrite(OUT_LED_R, color.R);
    analogWrite(OUT_LED_G, color.G);
    analogWrite(OUT_LED_B, color.B);
}

void setup() {

    // NOTE: Pinos analógicos não precisam ser configurados pq não tem output nesse sentido
    pinMode(OUT_LED_R, OUTPUT);
    pinMode(OUT_LED_G, OUTPUT);
    pinMode(OUT_LED_B, OUTPUT);
}

void loop() {

    // Primeiro, leia os valores do potenciômetro e preencha a estrutura
    get_color_from_pots(actual_color);

    // Depois jogue as cores pro LED RGB
    write_color_to_led(actual_color);
}
