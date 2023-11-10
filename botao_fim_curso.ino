#include <IRremote.h>

// Definição dos pinos do motor, sensor IR e variáveis globais
#define M1A 2
#define M1B 4
#define VELM1 3
#define BTN_END 6
#define PIN_IR 11

unsigned long valor_recebido;  // Variável para armazenar o valor do código IR recebido
int velocidade;                // Variável para a velocidade do motor (não está sendo usada)
int estado_btn_end;            // Variável para o estado do botão END (pressionado ou não)
bool motorLigado = false;      // Variável para rastrear o estado do motor (ligado ou desligado)

// Funções para controlar o motor
void frente(int vel = 255);  // Função para mover para frente (padrão: velocidade máxima)
void parar(int vel = 0);     // Função para parar o motor (padrão: velocidade zero)
void tras(int vel = 255);    // Função para mover para trás (padrão: velocidade máxima)

void setup() {
  // Inicialização do receptor IR e configuração dos pinos do motor e botão
  IrReceiver.begin(PIN_IR);
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(BTN_END, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // Leitura do estado do botão END
  estado_btn_end = digitalRead(BTN_END);

  // Impressão do valor recebido do controle remoto (pode estar desatualizado se o controle não foi pressionado recentemente)
  Serial.print("Valor recebido: ");
  Serial.println(valor_recebido);

  // Processamento de códigos IR enquanto estão disponíveis
  while (IrReceiver.decode()) {
    // Atualização do valor recebido
    valor_recebido = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(valor_recebido);
    IrReceiver.resume();

    // Leitura da velocidade (não está sendo usada atualmente)
    velocidade = analogRead(VELM1);
  }

  // Se o botão END for pressionado, execute as seguintes ações
  if (estado_btn_end == LOW) {
    tras();  // Move para trás
    delay(500);
    parar();  // Para o motor
    Serial.print("Estado BTN_END: ");
    Serial.println(estado_btn_end);
  }

  // Se o código IR correspondente ao botão ON for recebido, execute as seguintes ações
  if (valor_recebido == 4010852096) {
    frente();            // Move para frente
    valor_recebido = 0;  // Zera o valor recebido após tratar o comando
  }
}

// Função para mover para frente
void frente(int vel) {
  analogWrite(VELM1, vel);
  digitalWrite(M1A, 0);
  digitalWrite(M1B, 1);
}

// Função para parar o motor
void parar(int vel) {
  analogWrite(VELM1, vel);
  digitalWrite(M1A, 0);
  digitalWrite(M1B, 0);
}

// Função para mover para trás
void tras(int vel) {
  analogWrite(VELM1, vel);
  digitalWrite(M1A, 1);
  digitalWrite(M1B, 0);
}
