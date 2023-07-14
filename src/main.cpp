#include <Arduino.h>

const uint8_t RedLed = 5; // led at GPIO D5
const uint8_t PushButton = 4; // led at GPIO D4
int           buttonState = 0;

void setup() {
  Serial.begin(115200);

  pinMode(PushButton, INPUT);
  pinMode(RedLed, OUTPUT);
}

void loop() {
  buttonState = digitalRead(PushButton);
  Serial.println(buttonState);

  if (buttonState == HIGH)
  {
    digitalWrite(RedLed, HIGH);
  }
  else
  {
    digitalWrite(RedLed, LOW);
  }
  
}
