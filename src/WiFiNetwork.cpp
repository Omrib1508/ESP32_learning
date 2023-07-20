/********************************************************************
* @name     WiFiNetwork.cpp
* @brief    This file is including all the functions that need for 
*           applying a wifi network
* @author   Omri Ben-Shahar Dgany
* @date     19/07/2023
********************************************************************/
#include <Arduino.h>
#include <WiFi.h>
#include "WiFiNetwork.h"

void scanNetworks(int scanIteration)
{
  int networks = 0;
  
  for (int i = 0; i < scanIteration; i++)
  {
    networks = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (networks == 0)
    {
      Serial.println("No networks found");
      continue;
    }
    else 
    {
      Serial.print(networks);
      Serial.println(" networks found");
      Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
      for (int network = 0; network < networks; ++network) 
      {
        // Print SSID and RSSI for each network found
        Serial.printf("%2d",network + 1);
        Serial.print(" | ");
        Serial.printf("%-32.32s", WiFi.SSID(network).c_str());
        Serial.print(" | ");
        Serial.printf("%4d", WiFi.RSSI(network));
        Serial.print(" | ");
        Serial.printf("%2d", WiFi.channel(network));
        Serial.print(" | ");
        switch (WiFi.encryptionType(network))
        {
          case WIFI_AUTH_OPEN:            Serial.print("open");     break;
          case WIFI_AUTH_WEP:             Serial.print("WEP");      break;
          case WIFI_AUTH_WPA_PSK:         Serial.print("WPA");      break;
          case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2");     break;
          case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
          case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
          case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3");     break;
          case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3");break;
          case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI");     break;
          default:                        Serial.print("unknown");
        }
        Serial.println();
      }
    }
  }
}

wl_status_t initNetworkStation(const char* ssid, const char* password) {
  wl_status_t   networkStatus  = WL_IDLE_STATUS;
  unsigned long initSearchTime = 0;
  unsigned long currentTime    = 0;
  bool          isSearchTimout = false;

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  initSearchTime = millis();
  while (networkStatus != WL_CONNECTED) {
    networkStatus = WiFi.status();
    currentTime = millis();
    if (currentTime - initSearchTime > SEARCHNETOWRKSTIMEOUT)
    {
      isSearchTimout = true;
      break;
    }
    Serial.print('.');
    delay(500);
  }
  Serial.println("");
  if (isSearchTimout == true)
  {
    Serial.println("Timeout searching for network");
  }
  else
  {
    Serial.println("WiFi connected.");
  }
  return networkStatus;
}
