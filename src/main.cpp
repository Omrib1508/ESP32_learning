#include <Arduino.h>

#define SECONDS 5

const uint8_t RedLed = 25; // led at GPIO D25
const uint8_t SIRmotion = 27; // SIR motion sensor at GPIO D27
const uint8_t Buzzer = 26; // Buzzer at GPIO D26

unsigned long interuptTime = 0;
unsigned long currentTime = millis();

bool          isTimerStart = false;
bool          isMotionDitact = false;

void IRAM_ATTR detectsMovement() {
  digitalWrite(RedLed, HIGH);
  digitalWrite(Buzzer, HIGH);
  isTimerStart = true;
  interuptTime = millis();
}

void setup() {
  Serial.begin(115200);

  pinMode(SIRmotion, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SIRmotion), detectsMovement, RISING);

  pinMode(RedLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);


  digitalWrite(RedLed, LOW);
  digitalWrite(Buzzer, LOW);
}

void loop() {
  currentTime = millis();
  if((digitalRead(RedLed) == HIGH) && (isMotionDitact == false)) {
    Serial.println("MOTION DETECTED!!!");
    isMotionDitact = true;
  }
  // Turn off the LED after the number of seconds defined in the timeSeconds variable
  if(isTimerStart && (currentTime - interuptTime > (SECONDS * 1000))) {
    Serial.println("Motion stopped...");
    digitalWrite(RedLed, LOW);
    digitalWrite(Buzzer, LOW);
    isTimerStart = false;
    isMotionDitact = false;
  }
}
