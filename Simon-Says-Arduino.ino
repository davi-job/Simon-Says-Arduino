#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_G3  196

int gamePos = 0;

int dificuldade = 250;

int hits = 0;
int pontuacao = 0;

int botoes[4] = {4, 7, 8, 12};
int leds[4] = {6, 9, 10, 11};
int notes[4] = {NOTE_C4, NOTE_G3, NOTE_A3, NOTE_B3};

int BUZZ = 3;

int melody[100] = {};
int simon[100] = {};
int index = 0;

int i = 0;

bool hit = true;

int SimonToLed(int value) {
  switch (value) {
    case 6: return 0; break;
    case 9: return 1; break;
    case 10: return 2; break;
    case 11: return 3; break;
  }
}

void ResetLeds() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], LOW);
  }
}

void(* resetFunc) (void) = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {

    pinMode(botoes[i], INPUT_PULLUP);

    pinMode(leds[i], OUTPUT);
  }

  Serial.println(pontuacao);

  //Animação Inicial//

  for (int a = 0; a < 4; a++) {
    digitalWrite(leds[a], HIGH);
    tone(BUZZ, notes[a] + 300, 200);
    delay(200);
    digitalWrite(leds[a], LOW);
  }
  for (int a = 3; a >= 0; a--) {
    digitalWrite(leds[a], HIGH);
    tone(BUZZ, notes[a] + 300, 200);
    delay(200);
    digitalWrite(leds[a], LOW);
  }

  for (int a = 0, b = 3; a < 2; a++, b--) {
    digitalWrite(leds[a], HIGH);
    tone(BUZZ, notes[a] + 300, 200);
    digitalWrite(leds[b], HIGH);
    tone(BUZZ, notes[b] + 300, 200);
    delay(350);
    digitalWrite(leds[a], LOW);
    digitalWrite(leds[b], LOW);
  }

  delay(500);

  for (int a = 0; a < 4; a++) {
    digitalWrite(leds[a], HIGH);
    tone(BUZZ, notes[a] + 300, 750);
  }

  delay(750);

  for (int a = 3; a >= 0; a--) {
    digitalWrite(leds[a], LOW);
  }
}

void loop() {
  if (analogRead(A0) > 70 && analogRead(A0) < 660) {
    dificuldade = 250;
  }
  else if (analogRead(A0) <= 70) {
    dificuldade = 500;
  }
  else if (analogRead(A0) >= 660 && analogRead(A0) < 1020) {
    dificuldade = 125;
  }
  else if (analogRead(A0) >= 1020) {
    dificuldade = 50;
  }

  if (!hit) {

    ResetLeds();

    for (int i = 0; i < 4; i++) {
      if (!digitalRead(botoes[i])) {

        if (i == SimonToLed(simon[gamePos])) {
          tone(BUZZ, notes[i], 500);
          digitalWrite(leds[i], HIGH);
          delay(250);
          digitalWrite(leds[i], LOW);
          hits++;
          gamePos++;
        }
        else {
          for (int k = 0; k < 4; k++) {
            digitalWrite(leds[k], HIGH);
            tone(BUZZ, notes[k] + 500, 750);
          }
          delay(750);
          ResetLeds();
          pontuacao = 0;
          resetFunc();

        }

        if (hits == index) {
          hit = true;
          hits = 0;
          gamePos = 0;

          pontuacao++;
          Serial.println(pontuacao);

          break;
        }
      }
      else {
        tone(BUZZ, 0, 500);

      }
    }
  }
  else {
    delay(1000);

    randomSeed(analogRead(A1));
    int newNote = random(0, 3);

    melody[index] = notes[newNote];
    simon[index] = leds[newNote];

    index++;

    i = 0;

    while (i < index) {
      digitalWrite(simon[i], HIGH);
      tone(BUZZ, melody[i], dificuldade);
      delay(dificuldade);
      digitalWrite(simon[i], LOW);
      delay(dificuldade / 1.5);

      i++;
    }

    hit = false;
  }
}
