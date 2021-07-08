const int highpin[54] = {
  5, 8, 6, 2, 8, 2,
  6, 6, 9, 7, 9, 3,
  5, 7, 6, 3, 8, 3,
  7, 6, 4, 7, 4, 4,    
  5, 2, 6, 4, 8, 2,
  8, 5, 3, 7, 3, 9,
  5, 3, 6, 9, 8, 3,
  9, 5, 2, 7, 2, 9,
  5, 4, 7, 8, 9, 4   
};

const int lowpin[54] = {
  6, 6, 9, 7, 9, 3,
  5, 8, 6, 2, 8, 2,
  7, 6, 4, 7, 4, 4,
  5, 7, 6, 3, 8, 3,    
  8, 5, 3, 7, 3, 9,
  5, 2, 6, 4, 8, 2,
  9, 5, 2, 7, 2, 9,
  5, 3, 6, 9, 8, 3,
  4, 5, 8, 7, 4, 9
};

void setup() {
  matrixOff();
}

void loop() {
  for(int i = 0; i < 54; i++) {
    lightLED(i);
    delay(50);
    matrixOff();
  }
}

void matrixOff() { //sets each IO pin used by the board LOW
  for(int i = 2; i < 10; i++) {
    pinMode(i, INPUT);
    digitalWrite(i, LOW);
  }
}

void lightLED(int ledNum) { //sets the specified LED on (0-53)
  digitalWrite(highpin[ledNum],HIGH);
  digitalWrite(lowpin[ledNum],LOW);
  pinMode(highpin[ledNum],OUTPUT);
  pinMode(lowpin[ledNum],OUTPUT);
}
