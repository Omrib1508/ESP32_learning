#include <Arduino.h>
#include <WiFi.h>
#include "WiFiNetwork.h"

const uint8_t RedLed        = 13; // led at GPIO D13
const uint8_t GreenLed      = 12; // led at GPIO D12
const uint8_t BlueLed       = 14; // led at GPIO D14
String        readBuffer;
String        RedLedState   = "0";   // status of the red led
String        GreenLedState = "0"; // status of the green led
String        BlueLedState  = "0";   // status of the red led
char*         ssid;
char*         password;
unsigned long currentTime = millis();
int           menuChoose;
int           pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
const int     freq          = 5000;
const int     redChannel    = 0;
const int     greenChannel  = 1;
const int     blueChannel   = 2;
const int     resolution    = 8;

void setup() {
  wl_status_t connecntionStatus;
  int          scanIteration = 0;
  
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
  ledcAttachPin(RedLed, redChannel);
  ledcAttachPin(greenChannel, GreenLed);
  ledcAttachPin(blueChannel, BlueLed);

  Serial.begin(115200);
  Serial.println("Choose from menu:");
  Serial.println("1 - Scan networks");
  Serial.println("2 - Create Station throw network");
  while (!Serial.available()) {}
  menuChoose = Serial.readStringUntil('\n').toInt();
}

void loop() {
  
 
}


switch (menuChoose)
  {
    case 1:
      Serial.println("Scanning for network. Please enter the number of scans:");
      while (!Serial.available()) {}
      scanIteration = Serial.readStringUntil('\n').toInt();
      Serial.println(scanIteration);
      scanNetworks(scanIteration);
      ESP.restart();
      break;
    case 2:
      Serial.println("Please enter the ssid:");
      while (!Serial.available()) {}
      readBuffer = Serial.readStringUntil('\n');
      ssid = new char[readBuffer.length() + 1];
      strcpy(ssid, readBuffer.c_str());
      Serial.println(ssid);
    
      Serial.println("Please enter the ssid password:");
      while (!Serial.available()) {}
      readBuffer = Serial.readStringUntil('\n');
      password = new char[readBuffer.length() + 1];
      strcpy(password, readBuffer.c_str());
      Serial.println(password);

      Serial.print("Conntecting to newtork: ");
      Serial.println(ssid);
      connecntionStatus = initNetworkStation(ssid, password);
      if (connecntionStatus != WL_CONNECTED)
      {
        Serial.print(connecntionStatus);
        ESP.restart();
      }
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      server.begin();
      break;

    default:
      if (connecntionStatus != WL_CONNECTED)
      {
        Serial.print("Choose not good");
        ESP.restart();
      }
      break;
  }