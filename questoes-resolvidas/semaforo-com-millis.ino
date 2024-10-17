// --- Criar um semáforo utilizando millis() seguindo o enunciado anotado ---

constexpr byte LED[] = {12, 11, 10}; // Vermelho, amarelo e verde, respectivamente
constexpr unsigned long L_PER[] = {0, 1000, 2500, 4000}; // em segundos

// Período do programa
unsigned long t{0};

void setup() {
    Serial.begin(9600);

    for(auto pin : LED)
        pinMode(pin, OUTPUT);
}

// Escreve cada bit do número bin_code no array de LED respectivo por meio de
// bitshifting + AND.
void write_to_leds(byte bin_code) {
    for(auto i = 0; i < 3; i++)
        digitalWrite(LED[i], (bin_code >> i) & 0b001);
}

// Retorna se value está contido no intervalo [a, b[
bool in_interval(unsigned long value, unsigned long a, unsigned long b) {
    return (a <= value && value < b) ? true : false;
}

void loop() {
  for(byte i = 0; i < 3; i++) {
      if(in_interval(millis() - t, L_PER[i], L_PER[i + 1]))
          write_to_leds(0b1 << i);
  }

  if(millis() - t >= L_PER[3]) {
      Serial.print("Resetado no período t = ");
      Serial.print(millis() - t);
      Serial.println(" ms");

      t = millis();
  }
}
