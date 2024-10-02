#include "Arduino.h"

constexpr byte MAX_BUF_S = 13; // No contexto do Arduino R3, o máximo é 13 (111111111111B)
byte BUF_END = 0; // Index necessário para localizar o último char da array

char num_in[MAX_BUF_S];
double dec_out = 0.0; // a função pow() funciona apenas com double, se não leva a truncamento

byte bin_out[MAX_BUF_S - 1];

byte LED[MAX_BUF_S - 1];

// Essa flag garantirá que getNum() seja chamado apenas 1 vez no programa inteiro
bool hasInput = false;

enum NumberalSystem {
  Decimal = 1,
  Binary,
  Octal,
  Hexadecimal,
  Invalid,
};

void setup() {
  Serial.begin(9600);

  for(auto i = 0; i < MAX_BUF_S - 1; i++) {
      LED[i] = i+1;
      pinMode(LED[i], OUTPUT);
  }

  Serial.println("Insira o valor maximo do contador [0-4095d]");
  Serial.println("Utilize a ordem esquerda->direita.");
  Serial.println("Coloque o sis. numerico no final do numero");
  Serial.println("Ex: FFh, 101b, 13d, 65o");
  Serial.println();
}

// ----
// Essa função pega o input do usuário pelo serial, só é executada uma
// vez durante todo o runtime
// ----
void getNum() {

  // NOTE: esse objeto só vai ser construído uma vez durante o programa
  String str_buf;

  if (Serial.available() > 0) {
    str_buf = Serial.readStringUntil('\n');
    str_buf.toCharArray(num_in, MAX_BUF_S);
    BUF_END = str_buf.length() - 1; // considerando a contagem a partir do 0

    hasInput = true;
  }
}

// ----
// Detecta se o input está em hexadecimal, octal, binário ou decimal
// ----
int getNumSystem() {
  char systemChar = num_in[BUF_END];
  NumberalSystem numSystem;

  switch (systemChar) {
  case 'f':
  case 'F': {
    numSystem = Hexadecimal;
    break;
  }

  case 'o':
  case 'O': {
    numSystem = Octal;
    break;
  }

  case 'b':
  case 'B': {
    numSystem = Binary;
    break;
  }

  case 'd':
  case 'D': {
    numSystem = Decimal;
    break;
  }

  default: {
    numSystem = Invalid;
    break;
  }
  }

  return numSystem;
}

// -- Algoritmos de Conversão SISTEMA -> Decimal + utilitários

// Verifica se o caractére está em um intervalo [a, b] na tabela ASCII
// essa função auxilia bastante na legibilidade do código
bool char_is_in_between(char ch, char a, char b) {
  return (ch >= a && ch <= b) ? true : false;
}

byte hex_digit_value(char hex_char) {
  byte num_buf = 0;

  if (char_is_in_between(hex_char, '0', '9'))
    num_buf = hex_char - '0'; // considerando valores da tabela ASCII

  else if (char_is_in_between(hex_char, 'A', 'F'))
    num_buf = 15 - ('F' - hex_char);

  else if (char_is_in_between(hex_char, 'a', 'f'))
    num_buf = 15 - ('f' - hex_char);

  return num_buf;
}

void hex_to_dec() {
  byte num_hex_digit = BUF_END; // excluindo o indicador de sistema numérico pois começa a contar do 0

  for (auto i = 0; i < num_hex_digit; i++) {
    auto exp = (num_hex_digit - i) - 1;
    dec_out += hex_digit_value(num_in[i]) * pow(16, exp);
  }
}

// --- Execução
void dec_to_bin(int dec_value) {
    for(auto i = 0; i < MAX_BUF_S - 1; i++)
        // obs: array termina em MAX_BUF_S - 2 por causa do [0]
        bin_out[(MAX_BUF_S - 2) - i] = (dec_value >> i) & 0b1;
}

void bin_to_led() {
    for(auto i = 0; i < MAX_BUF_S - 1; i++)
        digitalWrite(LED[i], bin_out[i]);
}

void count() {
    int max_number = int(round(dec_out));

    for(auto i = 0; i <= max_number; i++) {
        dec_to_bin(i);
        bin_to_led();
        delay(1000);
    }
}

void execute() {
  Serial.print("[LOG] limite do contador estabelecido para ");
  Serial.println(num_in);

  switch (getNumSystem()) {
    case NumberalSystem::Hexadecimal: {
        hex_to_dec();
        count();
        break;
    }
  }

  dec_out = 0;
}

void loop() {
  if (!hasInput)
      getNum();
  else
      execute();
}
