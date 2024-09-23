// NOTE: ao longo do código, fora usado a palavra "auto" de maneira extensiva como um recurso de
// prototipagem, mas algumas resoluções do compilador podem usar mais memória do que realmente se
// é necessário (ex: auto -> unsigned long ao invés de auto -> uint8_t), sem contar problemas que
// podem aparecer em questão de compatibilidade, pois auto foi adicionado depois de C++11.
//
// TODO: remover todas as variáveis do tipo `auto`

const byte RGB_PIN[] = {9, 10, 11}; // NOTE: PWM

// NOTE: algumas cores bem específicas foram exibidas de forma bem estranha por simulações, de forma
// notavel a cor branca que ficou completamente instável no Tinkercad e exibiu uma cor acinzentada
// escura no SimulIDE utilizando o mesmo código para ambos. Caso eu consiga montar essa questão em
// um circuito real para testar as cores, esse comentário será modificado. (talvez eu esteja acessando
// memória inválida no for loop nessa região?)
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

// No contexto da questão, não faz sentido essa constante ser diferente de 3 (RGB), mas em uma
// aplicação diferente seria interessante conseguir calcular o tamanho do arranjo interno
//
// NOTE: como calcular o valor da array interna (RGB)?
// sizeof(T_RGB[0])/sizeof(byte) aparentemente não funciona (resulta o mesmo valor de T_LEN)
const byte VEC_LEN = 3;

// NOTE: problema de design em relação ao pino do botão não ser constante, podendo mudar
// durante o runtime. É possível fazer com que "pin" seja um membro constante da struct
// criando-a dentro do construtor? (e caso sim, faz sentido ter um membro constante em
// uma struct?). (é difícil mudar o pino sem querer pois seria necessário acessar btn.pin,
// mas é sempre bom conseguir ter o máximo de controle possível sobre as variáveis)
struct Button {

  // O construtor da struct permite com que apenas o pino seja inicializado, mantendo o restante dos
  // valores no padrão estabelecido
  //
  // NOTE: como fazer o equivalente em C99 (construtores de struct/classes)?
  Button(byte l_pin) : pin{l_pin} {
      pinMode(pin, INPUT);
  }

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
    analogWrite(RGB_PIN[j], T_RGB[t_index][j]);
}

// Essa função não é necessária, mas deixa o loop principal mais intuitivo
void update_status(struct Button &btn) {
  btn.last_state = btn.current_state;
}

void loop() {
  for(auto i = 0; i < T_LEN;) {
    statusButton(b_down);
    statusButton(b_up);

    // Para o botão que percorre a tabela no sentido cima->baixo
    if(b_down.was_pressed) {
      i--;
      i < 0 ? i = T_LEN - 1 : i; // Esse handler é necessário para que não seja acessado o index -1 no array2D

      write_to_led(i);
    }

    // Para o botão que percorre a tabela no sentido baixo->cima
    if(b_up.was_pressed) {
      i++;

      // Nesse ponto de execução, mesmo após o incremento de i, o laço for não irá interromper automaticamente
      // portanto, ainda há a possibilidade de acesso do index T_LEN, que irá acessar um endereço fora da array2D,
      // por isso esse handler é necessário.
      //
      // Esse handler é essencialmente diferente do anterior pois quando i = T_LEN o laço for irá ser interrompido
      // e reiniciado depois (graças ao loop()). Note que quando i = 0, o valor já será incrementado para 1 antes
      // de chamar a função write_to_led(i).
      //
      // NOTE: Existe um jeito melhor de gerenciar o index para que não ocorra acesso inválido da array e
      // que consiga assegurar o efeito de reinicialização do LED sem usar esse operador ternário ou if/else?
      i == T_LEN ? write_to_led(0) : write_to_led(i);
   }

    update_status(b_down);
    update_status(b_up);
  }
}
