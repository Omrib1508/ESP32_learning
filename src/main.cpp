#include <Arduino.h>

const uint8_t RedLed = 4; // led at GPIO D4
const int     freq = 5000;
const int     ledChannel = 0;
const uint8_t resultion = 8;

void setup() {
  ledcSetup(ledChannel, freq, resultion);
  ledcAttachPin(RedLed, ledChannel);
  delay(2000);
}

void loop() {
  for (int dutyCycle = 0; dutyCycle <= 0xFF; dutyCycle++)
  {
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
  for (int dutyCycle = 0xFF; dutyCycle >= 0; dutyCycle--)
  {
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
}