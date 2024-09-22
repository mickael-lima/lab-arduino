const byte RGB_PIN[] = {9, 10, 11}; // NOTE: PWM

// Tabela para testes
const byte T_RGB[][15] = {{0, 0, 0}, // Desligado
                         {0, 0, 255}, // Azul
                         {0, 255, 0}, // Verde
                         {0, 255, 255}, // Ciano
                         {255, 0, 0}, // Vermelho
                         {255, 0, 255}, // Magenta
                         {255, 255, 0}, // Amarelo
                         {85, 85, 85}, // Branco
                         {127, 0, 127}, // Roxo
                         {0, 127, 127}, // Verde Azulado
                         {127, 127, 0}, // Laranja
                         {190, 190, 190}, // Cinza
                         {210, 105, 30}, // Chocolate
                         {238, 130, 238}, // Violeta
                         {165, 42, 42}, // Marrom
};

// Número de elementos na tabela e número de elementos dos elementos da tabela, respectivamente
// NOTE: Isso considera que todos os elementos internos do vetor T_RGB tem o mesmo número de
// componentes
const byte T_LEN = sizeof(T_RGB)/sizeof(T_RGB[0]);
const byte VEC_LEN = 3;

// NOTE: problema de design em relação ao pino do botão não ser constante, podendo mudar
// durante o runtime
struct Button {
  Button(byte pin) : pin{pin} {}

  byte pin{};
  bool current_state = LOW;
  bool last_state = LOW;
  bool was_pressed = false;
};

// Botão para descer e subir a tabela
Button b_down{3};
Button b_up{4};

void setup() {
  for(auto PIN : RGB_PIN)
    pinMode(PIN, OUTPUT);

  // É possível configurar N botões com um for-loop de uma array de objetos Button
  pinMode(b_down.pin, INPUT);
  pinMode(b_up.pin, INPUT);

  Serial.begin(9600);
}

void statusButton(struct Button &btn) {
  auto current_input = digitalRead(btn.pin);

  if(current_input == HIGH) {
    delay(5);
    current_input = digitalRead(btn.pin);
  }

  btn.current_state = current_input;
  btn.was_pressed = (btn.last_state == LOW && btn.current_state == HIGH) ? true : false;
}

void write_to_led(byte t_index) {
  for(auto j = 0; j < VEC_LEN; j++)
    analogWrite(RGB_PIN[j], T_RGB[t_index][j]); // Para escrever os valores contidos em [0, 255]
}

void loop() {
  Serial.print("T_LEN: ");
  Serial.println(T_LEN);

  for(auto i = 0; (i < T_LEN);) {
    statusButton(b_down);
    statusButton(b_up);
    // Para o botão que percorre a tabela no sentido cima->baixo
    if(b_down.was_pressed) {
      i--;
      i < 0 ? i = T_LEN - 1 : i;

      write_to_led(i);
    }

    // Para o botão que percorre a tabela no sentido cima->baixo
    if(b_up.was_pressed) {
      i++;
      i == T_LEN ? write_to_led(0) : write_to_led(i);
   }

    b_down.last_state = b_down.current_state;
    b_up.last_state = b_up.current_state;
  }
}
