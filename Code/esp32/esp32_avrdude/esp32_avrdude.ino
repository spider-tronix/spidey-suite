#include "avrdude.h"

void setup() {
  Avrdude avrdude;
  avrdude.begin();
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {}
