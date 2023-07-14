#include <Arduino.h>

const uint8_t PotenResistor = 4; // led at GPIO D4
int           potenValue = 0;

void setup() {
  Serial.begin(9600);

  pinMode(PotenResistor, INPUT);
  delay(2000);
}

void loop() {
  potenValue = analogRead(PotenResistor);
  Serial.println(potenValue);
  delay(500);
}
