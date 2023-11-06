// Define os pinos para LEDs e botões
const int ledPins[] = {23, 22, 21, 19};
const int buttonPins[] = {3, 18, 5, 17}; 

#define SPEAKER_PIN 4

#define MAX_GAME_LENGTH 100

#define restart 33

bool gameover_var = false;

const int gameTones[] = { 196, 262, 330, 784};

byte gameSequence[MAX_GAME_LENGTH] = {0}; // Sequência do jogo
byte gameIndex = 0; // Índice do jogo (Fase)

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(restart, INPUT_PULLUP);

  // Inicializa o gerador de números aleatórios.
  randomSeed(analogRead(A0));
}

// Acende o LED fornecido e toca um tom

void lightLedAndPlayTone(byte ledIndex) {
  if (digitalRead(restart) == LOW) { // Verifica se o botão de reset foi pressionado
    resetGame();
    delay(500); // Atraso para evitar pressões acidentais
  }

  digitalWrite(ledPins[ledIndex], HIGH);
  tone(SPEAKER_PIN, gameTones[ledIndex]);
  delay(300);
  digitalWrite(ledPins[ledIndex], LOW);
  noTone(SPEAKER_PIN);
}


// Toca a sequência atual de notas que o usuário deve repetir

void playSequence() {

  if (digitalRead(restart) == LOW) { // Verifica se o botão de reset foi pressionado
    resetGame();
    delay(500); // Atraso para evitar pressões acidentais
  }
  
  // Tocar sequência
  for (int i = 0; i < gameIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLedAndPlayTone(currentLed);

    if (digitalRead(restart) == LOW) { // Verifica se o botão de reset foi pressionado
      resetGame();
      delay(500); // Atraso para evitar pressões acidentais
    }

    delay(50);
  }
}

// Aguarda até que o usuário pressione um dos botões e retorna o índice desse botão

byte readButtons() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (digitalRead(buttonPin) == LOW) {
        return i;
      } else if(digitalRead(restart) == LOW) {
          resetGame();
      }
    }
    delay(1);
  }
}

// Toca a sequência de game over e exibe a pontuação do jogo

void gameOver() {
  Serial.print("Você perdeu! Fim de jogo! Sua pontuação: ");

  // Tirar -1 por causa do gameIndex++ do loop
  Serial.println(gameIndex - 1);
  gameIndex = 0;
  delay(200);

  // Som de derrota
  tone(SPEAKER_PIN, 622);
  delay(300);
  tone(SPEAKER_PIN, 587);
  delay(300);
  tone(SPEAKER_PIN, 554);
  delay(300);

  for (byte i = 0; i < 10; i++) {
    for (int pitch = -10; pitch <= 10; pitch++) {
      tone(SPEAKER_PIN, 523 + pitch);
      delay(5);
    }
  }
  noTone(SPEAKER_PIN);
  delay(500);

  gameover_var = true;
}


// Obtém a entrada do usuário e compara com a sequência esperada
bool checkUserSequence() {
  for (int i = 0; i < gameIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();
    lightLedAndPlayTone(actualButton);

    // Se o usuário errar
    if (expectedButton != actualButton) {
      return false;
    }
  }

  return true;
}


// Toca um som de comemoração sempre que o usuário conclui um nível

void playLevelUpSound() {
  tone(SPEAKER_PIN, 330);
  delay(150);
  tone(SPEAKER_PIN, 392);
  delay(150);
  tone(SPEAKER_PIN, 659);
  delay(150);
  tone(SPEAKER_PIN, 523);
  delay(150);
  tone(SPEAKER_PIN, 587);
  delay(150);
  tone(SPEAKER_PIN, 784);
  delay(150);
  noTone(SPEAKER_PIN);
}

// O loop principal do jogo

void loop() {
  if (digitalRead(restart) == LOW) { // Verifica se o botão de reset foi pressionado
    resetGame(); // Chama a função para reiniciar o jogo
    delay(500); // Adiciona um atraso para evitar pressões acidentais
  }

  // Adiciona um novo valor à sequência do jogo
  gameSequence[gameIndex] = random(0, 4);
  gameIndex++;

  // Garante que o índice do jogo não ultrapasse o comprimento máximo permitido
  if (gameIndex >= MAX_GAME_LENGTH) {
    gameIndex = MAX_GAME_LENGTH - 1;
  }

  playSequence(); // Executa a sequência do jogo

  // Verifica se a sequência do usuário está correta
  if (!checkUserSequence()) {
    gameOver(); // Chama a função para encerrar o jogo
  }

  delay(300); // Aguarda um curto intervalo

  if (gameIndex > 0) {
    playLevelUpSound(); // Toca um som de celebração quando o usuário completa um nível
    delay(300);
  }

  if (digitalRead(restart) == LOW) { // Verifica se o botão de reset foi pressionado
    resetGame(); // Chama a função para reiniciar o jogo
    delay(500); // Atraso para evitar pressões acidentais
  }
}

// Função para resetar o jogo
void resetGame() {

  gameover_var = false; // Reseta a variável de controle de fim de jogo
  gameIndex = 0; // Zera o índice do jogo

  // Acende e apaga os LEDs como parte da reinicialização do jogo
  for (byte i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(300);
    digitalWrite(ledPins[i], LOW);
  }

  delay(500); // Pequeno atraso antes de adicionar o próximo valor à sequência

  // Adiciona um novo valor à sequência do jogo após a reinicialização
  gameSequence[gameIndex] = random(0, 4);
  gameIndex++;

  // Garante que o índice do jogo não ultrapasse o comprimento máximo permitido após a reinicialização
  if (gameIndex >= MAX_GAME_LENGTH) {
    gameIndex = MAX_GAME_LENGTH - 1;
  }

  playSequence(); // Executa a sequência do jogo após a reinicialização
}
