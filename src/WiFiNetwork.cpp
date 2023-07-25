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
        delay(200);
      }
      Serial.println("");
      WiFi.scanDelete();
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
  delay(500);
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

void clientConnection(WiFiServer server, const int redChannel, const int greenChannel, const int blueChannel) {
  WiFiClient    client;
  String        clientInputBuffer;
  String        header; // variable for storing HTTP request
  String        RedLedState   = "0";   // status of the red led
  String        GreenLedState = "0"; // status of the green led
  String        BlueLedState  = "0";   // status of the red led
  char          readByte;
  unsigned long currentTime    = 0;
  unsigned long initClientTime = 0;
  int           pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;

  while (true)
  { 
    client = server.available();   // Listen for incoming clients 
    if (client) {                             // If a new client connects,
      Serial.println("New Client");          // print a message out in the serial port
      clientInputBuffer = "";                // make a String to hold incoming data from the client
      currentTime = millis();
      initClientTime = currentTime;
      while (client.connected() && (currentTime - initClientTime <= CLIENTCONNECTIONTIMEOUT)) {  // loop while the client's connected
        currentTime = millis();
        if (client.available()) {             // if there's bytes to read from the client,
          readByte = client.read();             // read a byte, then
          Serial.write(readByte);                    // print it out the serial monitor
          header += readByte;
          if (readByte == '\n') {                    // if the byte is a newline character
            if (clientInputBuffer.length() == 0) {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              // Display the HTML web page
              client.println("<!DOCTYPE html><html>");
              client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              client.println("<link rel=\"icon\" href=\"data:,\">");
              client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
              client.println("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>");
              client.println("</head><body><div class=\"container\"><div class=\"row\"><h1>ESP Color Picker</h1></div>");
              client.println("<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> ");
              client.println("<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div>");
              client.println("<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);");
              client.println("document.getElementById(\"change_color\").href=\"?r\" + Math.round(picker.rgb[0]) + \"g\" +  Math.round(picker.rgb[1]) + \"b\" + Math.round(picker.rgb[2]) + \"&\";}</script></body></html>");
              // The HTTP response ends with another blank line
              client.println();

              // Request sample: /?r201g32b255&
              // Red = 201 | Green = 32 | Blue = 255
              if(header.indexOf("GET /?r") >= 0) 
              {
                pos1 = header.indexOf('r');
                pos2 = header.indexOf('g');
                pos3 = header.indexOf('b');
                pos4 = header.indexOf('&');
                RedLedState = header.substring(pos1+1, pos2);
                GreenLedState = header.substring(pos2+1, pos3);
                BlueLedState = header.substring(pos3+1, pos4);
                /*Serial.println(redString.toInt());
                Serial.println(greenString.toInt());
                Serial.println(blueString.toInt());*/
                ledcWrite(redChannel, RedLedState.toInt());
                ledcWrite(greenChannel, GreenLedState.toInt());
                ledcWrite(blueChannel, BlueLedState.toInt());
              }
              // Break out of the while loop
              break;
            } 
            else { // if you got a newline, then clear currentLine
              clientInputBuffer = "";
            }
          } else if (readByte != '\r') {  // if you got anything else but a carriage return character,
            clientInputBuffer += readByte;      // add it to the end of the currentLine
          }
        }
      }
      // Clear the header variable
      header = "";
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
}