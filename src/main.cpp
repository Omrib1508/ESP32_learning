#include <Arduino.h>

const uint8_t PotenResistor = 4; // led at GPIO D4
int potenValue = 0, precision = 2;
float voltage, ratio, res1, res2;

void printElcetircValues(int analogVal) {
    char res1Buffer[15], res2Buffer[15];
    char voltageBuffer[10], printBuffer[100];
    
    ratio = float(analogVal)/float(0xFFF);
    voltage = 3.3 * ratio;
    res1 = 10000 * (1 - ratio);
    res2 = 10000 - res1;
    dtostrf(voltage, precision + 3, precision, voltageBuffer);
    dtostrf(res1, precision + 6, precision, res1Buffer);
    dtostrf(res2, precision + 6, precision, res2Buffer);
    //sprintf(printBuffer, "Vin=3.30[V], Vout=%s[V]", voltageBuffer);
    sprintf(printBuffer, "Vin=3.30[V], Vout=%s[V], R1=%s[Ohm], R2=%s[Ohm]", voltageBuffer, res1Buffer, res2Buffer);
    Serial.println(printBuffer);
}

void setup() {
  Serial.begin(115200);

  pinMode(PotenResistor, INPUT);
  delay(2000);
}

void loop() {
  potenValue = analogRead(PotenResistor);
  printElcetircValues(potenValue);
  delay(500);
}