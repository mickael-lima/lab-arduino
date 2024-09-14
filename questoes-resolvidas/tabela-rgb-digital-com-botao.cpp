#include "Arduino.h"

constexpr byte OUT_LED_R{9};
constexpr byte OUT_LED_G{10};
constexpr byte OUT_LED_B{11};

constexpr byte IN_BUTTON{7};

struct Color {
    byte R;
    byte G;
    byte B;
};

Color actual_color;

bool last_button_status{false};

void setup() {
    pinMode(OUT_LED_R, OUTPUT);
    pinMode(OUT_LED_G, OUTPUT);
    pinMode(OUT_LED_B, OUTPUT);

    pinMode(IN_BUTTON, INPUT);
}

// OBS: o tinkercad precisa da keyword struct explicita para reconhecer que essa estrutura é um tipo válido
// de dados criado pelo usuário, talvez seja bug do tinkercad ou uma regra geral especificamente para arduino
void number_to_RGB(byte number, struct Color &color_buffer) {
    color_buffer.R = (number >> 2) & 0b001;
    color_buffer.G = (number >> 1) & 0b001;
    color_buffer.B = (number >> 0) & 0b001;
}

void write_color_to_led(struct Color &color) {
    digitalWrite(OUT_LED_R, color.R);
    digitalWrite(OUT_LED_G, color.G);
    digitalWrite(OUT_LED_B, color.B);
}

bool toggle_button_status() {
    bool current_button_status = digitalRead(IN_BUTTON);

    if(current_button_status != last_button_status) {
        delay(5);
        current_button_status = digitalRead(IN_BUTTON);
    }

    return current_button_status;
}


// NOTE: número hardcodado no for loop
void loop() {
    for(auto i = 0; i <= 7;) {
        bool actual_button_status = toggle_button_status();

        if (last_button_status == LOW && actual_button_status == HIGH) {
            number_to_RGB(i, actual_color);
            write_color_to_led(actual_color);
            i++;
        }

        last_button_status = actual_button_status;
    }
}
