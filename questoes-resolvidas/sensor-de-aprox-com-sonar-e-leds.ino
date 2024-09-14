constexpr byte SONAR_TRIGGER{7};
constexpr byte SONAR_ECHO{8};

// NOTE: em centímetros por microssegundos. Em uma das etapas do cálculo de distância, será necessário dividir por 2, portanto
// é interessante fazê-lo logo na constante para poupar processamento. A velocidade do som em cm/µs é 0.343
constexpr float V_DO_SOM{0.01715};

constexpr byte LED[3] = {9, 10, 11};
constexpr byte LED_ARR_SIZE{sizeof(LED)/sizeof(byte)};

// O Sonar age enviando um pulso de som e contabilizando o tempo que ele demora pra voltar até o sensor (semelhante a um morcego)
// portanto precisa de duas portas: uma para emitir o som e outra para recebe-lo de volta
void setup() {
  pinMode(SONAR_TRIGGER, OUTPUT); // O arduino envia informação através do Trigger, que vai fazer o speaker emitir o som
  pinMode(SONAR_ECHO, INPUT); // O arduino recebe informação do Echo (onde a onda de som retorna)

  for(auto PIN : LED)
    pinMode(PIN, OUTPUT);
}

// É possível calcular a distância considerando a velocidade do som constante, o que permite a aplicação das fórmulas básicas do M.U
// como a velocidade está em cm/microssegundos, essa função retornará a distância em centimetros.
float distance() {

  // Prepara o trigger, inicializando-o com o valor LOW, isso garante que ele esteja pronto para funcionar
  digitalWrite(SONAR_TRIGGER, LOW);
  delayMicroseconds(2);

  // Ativa o trigger e faz com que ele emita o sinal ultrasônico por 10 microssegundos
  digitalWrite(SONAR_TRIGGER, HIGH);
  delayMicroseconds(10);

  // Desativa o trigger após 10 microssegundos de execução, liberando espaço pro SONAR_ECHO trabalhar
  digitalWrite(SONAR_TRIGGER, LOW);

  // Quando SONAR_TRIGGER for pra LOW, vai ativar uma espécie de "timer" dentro do sensor que vai computar o tempo de IDA e VOLTA da
  // onda de som, a função pulseIn() computa o tempo que SONAR_ECHO demora pra ir de LOW para HIGH.
  //
  // NOTE: pulseIn() retorna o tempo em microssegundos
  auto tempo = pulseIn(SONAR_ECHO, HIGH);

  // S = So + vt. Para esse exemplo usaremos float apenas para testes, mas para outras aplicações o int pode ser o suficiente
  // Isso computará o tempo de IDA e VOLTA (x cm na IDA e x centimetros na volta), para achar x apenas é necessário dividir a
  // distância (dada por V_DO_SOM * tempo) por 2. A operação de divisão fora executada dentro da constante no começo do arquivo
  return (V_DO_SOM * tempo);
}

// Função para verificar se value está contido no intervalo fechado [a,b], considerando a < b
// NOTE: tirar template depois e colocar tipos fixos
template<typename T, typename U>
bool in_interval(T value, U a, U b) {
  return (value >= a && value <= b) ? true : false;
}

void write_to_led(byte number) {
  for(auto i = 0; i < LED_ARR_SIZE; i++)
    digitalWrite(LED[i], (number >> i) & 0b001);
}

void loop() {
  auto actual_distance = distance();

  // NOTE: caso a distância não precise ser em float, pode-se refatorar usando switch case
  // Se a distância estiver entre 100 e 150 cm
  if(in_interval(actual_distance, 100, 150))
    write_to_led(1); // equivale a {0, 0 ,1}

  // Se a distância estiver entre 50 e 100 cm
  else if(in_interval(actual_distance, 50, 100))
    write_to_led(3); // equivale a {0, 1, 1}

  // Se a distância estiver entre 50 e 100cm
  else if(in_interval(actual_distance, 0, 50)) {
    write_to_led(7); // equivale a {1, 1, 1}
    delay(actual_distance * 20); // para implementar o efeito de luzes piscando proporcional a distância

    write_to_led(0);
    delay(actual_distance * 20);
  }

  else
    write_to_led(0);

}
