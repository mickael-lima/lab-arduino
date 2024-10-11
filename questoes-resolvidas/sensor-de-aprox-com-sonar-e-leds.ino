/* Sensor de distância com indicativo visual por meio de 9 LEDs que se acendem em função da distância
 * Os LEDs são divididos em 3 grupos: o primeiro se acenderá se a distância estiver em um intervalo de
 * $[150, +\inf[$ centímetros, o segundo grupo se acenderá quando $d \in [100, 150]$, o terceiro grupo
 * se acenderá quando $d \in [50, 100]$ e quando $d \in [0, 50]$, o terceiro grupo de LEDs ficará piscando.
 * Será usado como sensor o sonar HC-SR04. NOTE: Testado com êxito em simulação.
*/

// Constante para cálculo da distância em função da velocidade do som. O sonar trabalha detectando a distância
// de ida e de volta, portanto ele retorna a distância como 2d ao invés de d, é necessário já dividir por 2
// Ainda, a velocidade aqui está em centímetros por milissegundos.
constexpr double V_SOM{0.034029/2};

constexpr byte S_ECHO{2};
constexpr byte S_TRIG{3};

unsigned long l_per{0}; // para o último grupo de LEDs
double d{0}; // armazena a distância que o sensor retorna

byte LED[9];

void setup() {
    pinMode(S_ECHO, INPUT);
    pinMode(S_TRIG, OUTPUT);

    for(byte i = 0; i < 9; i++) {
        LED[i] = i + 4; // considerando o pino do Sonar, que já está em uso
        pinMode(LED[i], OUTPUT);
    }
}

// Função para obter a distância pelo sensor
double distancia() {

    // Prepara o pino para ser iniciado
    digitalWrite(S_TRIG, LOW);
    delayMicroseconds(2);

    // Emite sinal HIGH por 10 microssegundos (datasheet)
    digitalWrite(S_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(S_TRIG, LOW);

    // Retorna quanto tempo leva para S_ECHO ir de LOW pra HIGH
    // depois do pulso sonoro emitido anteriormente, esse é o
    // tempo t necessário para calcular a distância. Como, em
    // teoria, a velocidade do som é constante nesse contexto
    // então x(t) = x_o + vt, onde x(t) está sendo mult. por 2
    // x_o = 0, v = V_SOM e t = retorno de pulseIn(). A divisão
    // por 2 já fora feita na constante, portanto não aparece.
    return pulseIn(S_ECHO, HIGH) * V_SOM;
}

// Por questões de simplificação, será usado o número binário puro para piscar o grupo de LED
// Ex: para piscar os 3 primeiros grupos de LED, usa-se 0b111000000
void write_to_led(unsigned int bin_code) {
    for(byte i = 0; i < 9; i++)
        digitalWrite(LED[8 - i], (bin_code >> i) & 0b1);
}

void blink_last_led_group() {
    write_to_led(0b111111000);

    if(millis() - l_per <= 500)
        write_to_led(0b111111111);
    else
        write_to_led(0b111111000);

    if(millis() - l_per > 1000)
        l_per = millis();
}

bool in_interval(double x, double a, double b) {
    return (x >= a && x < b) ? true : false;
}

void loop() {
    double d = distancia();

    if(in_interval(d, 150.00, 300.00))
        write_to_led(0b111000000);

    else if(in_interval(d, 100.00, 150.00))
        write_to_led(0b111111000);

    else if(in_interval(d, 50.00, 100.00))
        write_to_led(0b111111111);

    else if(in_interval(d, 0.00, 50.00))
        blink_last_led_group();

    // Condição visual que serve para indicar que o objeto está muito longe para se ter uma
    // medição precisa.
    else
        write_to_led(0b101010101);
}
