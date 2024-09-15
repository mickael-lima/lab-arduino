constexpr byte SONAR_TRIGGER{13};
constexpr byte SONAR_ECHO{12};

// NOTE: em centímetros por microssegundos. Em uma das etapas do cálculo de distância, será necessário dividir por 2, portanto
// é interessante fazê-lo logo na constante para poupar processamento. A velocidade do som em cm/µs é 0.343
constexpr float V_DO_SOM{0.01715};

constexpr byte LED[] = {2, 3, 4, 5, 6, 7, 8 ,9, 10};
constexpr byte LED_ARR_SIZE{sizeof(LED)/sizeof(byte) - 1}; // NOTE: o - 1 permitirá extrair o complemento dentro do for-loop

void setup() {
  pinMode(SONAR_TRIGGER, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

  for(auto PIN : LED)
    pinMode(PIN, OUTPUT);
}

// NOTE: Essa função retornará a distância em centimetros.
float distance() {

  // Prepara o trigger, inicializando-o com o valor LOW, isso garante que ele esteja pronto para funcionar
  digitalWrite(SONAR_TRIGGER, LOW);
  delayMicroseconds(2);

  // Ativa o trigger e faz com que ele emita o sinal ultrasônico por 10 microssegundos
  digitalWrite(SONAR_TRIGGER, HIGH);
  delayMicroseconds(10);

  // Desativa o trigger após 10 microssegundos de execução, liberando espaço pro SONAR_ECHO trabalhar
  digitalWrite(SONAR_TRIGGER, LOW);

  // NOTE: pulseIn() retorna o tempo em microssegundos
  auto tempo = pulseIn(SONAR_ECHO, HIGH);

  // Isso computará o tempo de IDA e VOLTA (x cm na IDA e x centimetros na volta), para achar x apenas é necessário dividir a
  // distância (dada por V_DO_SOM * tempo) por 2. A operação de divisão fora executada dentro da constante no começo do arquivo
  return (V_DO_SOM * tempo);
}

bool in_interval(float value, int a, int b) {
  return (value >= a && value <= b) ? true : false;
}

void write_to_led(int number) {
  for(auto i = 0; i <= LED_ARR_SIZE; i++)
    digitalWrite(LED[i], (number >> (LED_ARR_SIZE - i)) & 0b1); // NOTE: complemento de i para inverter a ordem dos LEDs
}

void loop() {
  auto actual_distance = distance();

  // Se a distância estiver entre 100 e 150 cm
  if(in_interval(actual_distance, 100.0, 150.0))
    write_to_led(0b111000000); // equivale a {1, 1 ,1, 0, 0, 0, 0, 0, 0}

  // Se a distância estiver entre 50 e 100 cm
  else if(in_interval(actual_distance, 50.0, 100.0))
    write_to_led(0b111111000); // equivale a {1, 1, 1, 1, 1, 1, 0, 0, 0}

  // Se a distância estiver entre 50 e 100cm
  else if(in_interval(actual_distance, 0.0, 50.0)) {
    write_to_led(0b111111111); // equivale a {1, 1, 1, 1, 1, 1, 1, 1, 1}
    delay(actual_distance * 20); // para implementar o efeito de luzes piscando proporcional a distância

    write_to_led(0b111111000);
    delay(actual_distance * 20);
  }

  else
    write_to_led(0b000000000);
}
