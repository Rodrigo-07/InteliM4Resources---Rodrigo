void setup() {

  // Testar se o código está sendo rodado
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  // Definir a função de cada conexão
  pinMode(15, OUTPUT);

  pinMode(33, INPUT_PULLUP);
}

// Definir tempo do ponto e do traço
int tempo_curto = 300; // ponto
int tempo_longo = 500; // traço
int tempo_entre_palavras = 700;

void loop() {

  // S
  Ponto();
  Ponto();
  Ponto();

  // O
  Traco();
  Traco();
  Traco();

  // S
  Ponto();
  Ponto();
  Ponto();

  delay(tempo_entre_palavras); // Espaço entre palavras

}


void Ponto() {
  digitalWrite(15, HIGH);
  delay(tempo_curto);
  digitalWrite(15, LOW);
  delay(tempo_curto);
}

void Traco() {
  digitalWrite(15, HIGH);
  delay(tempo_longo);
  digitalWrite(15, LOW);
  delay(tempo_curto);
}