// Esse exemplo é semelhante ao anterior, porém agora com 1 botão para top-down e outro para bottom-up

#include "Arduino.h"

constexpr byte OUT_LED_R{9};
constexpr byte OUT_LED_G{10};
constexpr byte OUT_LED_B{11};

constexpr byte IN_BUTTON_DN{7};
constexpr byte IN_BUTTON_UP{8};

struct Color {
    byte R;
    byte G;
    byte B;
};

Color actual_color;

struct ButtonStatus {
    bool up_status{false};
    bool down_status{false};
};

ButtonStatus btn_last_status;

// Lista de cores usados para tabela RGB
constexpr long int TABELA_RGB[] = {0x000000, // Desligado
                              0x0000ff, // Azul
                              0x00ff00, // Verde
                              0x00ffff, // Ciano
                              0xff0000, // Vermelho
                              0xff00ff, // Magenta
                              0xffff00, // Amarelo
                              0x555555, // Branco
                              0x7F007F, // Roxo
                              0x007F7F, // Verde Azulado
                              0x7F7F00, // Laranja
                              0xBEBEBE, // Cinza
                              0xD2691E, // Chocolate
                              0xEE82EE, // Violeta
                              0xA52A2A}; // Marrom

constexpr byte TABELA_LEN{sizeof(TABELA_RGB)/sizeof(const long int)};

void setup() {
    pinMode(OUT_LED_R, OUTPUT);
    pinMode(OUT_LED_G, OUTPUT);
    pinMode(OUT_LED_B, OUTPUT);

    pinMode(IN_BUTTON_DN, INPUT);
    pinMode(IN_BUTTON_UP, INPUT);

    Serial.begin(13000);
}

// OBS: o tinkercad precisa da keyword struct explicita para reconhecer que essa estrutura é um tipo válido
// de dados criado pelo usuário, talvez seja bug do tinkercad ou uma regra geral especificamente para arduino
void number_to_RGB(long int number, Color &color_buffer) {
    color_buffer.R = number >> 16;
    color_buffer.G = number >> 8;
    color_buffer.B = number >> 0;

}

void write_color_to_led(Color &color) {
    // NOTE: o valor dentro da struct sempre será do tipo byte, não precisa mapear
    analogWrite(OUT_LED_R, color.R);
    analogWrite(OUT_LED_G, color.G);
    analogWrite(OUT_LED_B, color.B);
}

ButtonStatus get_button_status() {
    bool up_current_status = digitalRead(IN_BUTTON_UP);
    bool down_current_status = digitalRead(IN_BUTTON_DN);

    if(up_current_status != btn_last_status.up_status) {
        delay(5);
        up_current_status = digitalRead(IN_BUTTON_UP);
    }

    if(down_current_status != btn_last_status.down_status) {
        delay(5);
        down_current_status = digitalRead(IN_BUTTON_DN);
    }

    ButtonStatus current_btn_status;
    current_btn_status.up_status = up_current_status;
    current_btn_status.down_status = down_current_status;

    return current_btn_status;
}


// NOTE: número hardcodado no for loop
void loop() {
    for(auto i = 0; i < TABELA_LEN;) {
        ButtonStatus actual_button_status = get_button_status();

        if (btn_last_status.up_status == LOW && actual_button_status.up_status == HIGH) {

            // NOTE: Não precisa limitar o caso em que i > tamanho da array pois essa condição 
            // já está coberta pelo próprio laço for
            ++i;

            number_to_RGB(TABELA_RGB[i], actual_color);
            write_color_to_led(actual_color);

        } else if (btn_last_status.down_status == LOW && actual_button_status.down_status == HIGH) {

            // NOTE: i == 0 é oaso correto a ser analisado, se não i será decrementado e seu valor 
            // será jogado para -1
            i == 0 ? i = TABELA_LEN - 1 : i--;

            number_to_RGB(TABELA_RGB[i], actual_color);
            write_color_to_led(actual_color);
        }

        btn_last_status = actual_button_status;

    }
}
