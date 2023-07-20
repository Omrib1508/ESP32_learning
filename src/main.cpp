#include <Arduino.h>
#include <WiFi.h>
#include "WiFiNetwork.h"

const uint8_t RedLed = 26; // led at GPIO D26
const uint8_t GreenLed = 27; // led at GPIO D27
String        readBuffer;
String        RedLedState = "off";   // status of the red led
String        GreenLedState = "off"; // status of the green led
WiFiServer    server(80); // Set web server port number to 80
unsigned long currentTime = millis();
char*         ssid;
char*         password;
int           menuChoose;
void setup() {
  wl_status_t connecntionStatus;
  int          scanIteration = 0;
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);

  digitalWrite(RedLed, LOW);
  digitalWrite(GreenLed, LOW);
  
  Serial.begin(115200);
  Serial.println("Choose from menu:");
  Serial.println("1 - Scan networks");
  Serial.println("2 - Create Station throw network");
  while (!Serial.available()) {}
  menuChoose = Serial.readStringUntil('\n').toInt();

  switch (menuChoose)
  {
    case 1:
      Serial.println("Scanning for network. Please enter the number of scans:");
      while (!Serial.available()) {}
      scanIteration = Serial.readStringUntil('\n').toInt();
      Serial.println(scanIteration);
      scanNetworks(scanIteration);
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
}

void loop() {
  WiFiClient    client;
  String        clientInputBuffer;
  String        header; // variable for storing HTTP request
  char          readByte;
  unsigned long initClientTime = 0;

  client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    Serial.println("New Client");          // print a message out in the serial port
    clientInputBuffer = "";                // make a String to hold incoming data from the client
    initClientTime = millis();
    while (client.connected()) {  // loop while the client's connected
      currentTime = millis();
      if (currentTime - initClientTime <= CLIENTCONNECTIONTIMEOUT)
      {
        Serial.println("Client connection timeout.");
        Serial.println("");
        break;
      }
      
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
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              RedLedState = "on";
              digitalWrite(RedLed, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              RedLedState = "off";
              digitalWrite(RedLed, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              GreenLedState = "on";
              digitalWrite(GreenLed, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              GreenLedState = "off";
              digitalWrite(GreenLed, LOW);
            }
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>Red Led - State " + RedLedState + "</p>");
            // If the output26State is off, it displays the ON button       
            if (RedLedState=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>Green Led - State " + GreenLedState + "</p>");
            // If the output27State is off, it displays the ON button       
            if (GreenLedState=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            clientInputBuffer = "";
          }
        } else if (readByte != '\r') {  // if you got anything else but a carriage return character,
          clientInputBuffer += readByte;      // add it to the end of the currentLine
        }
      }
      initClientTime = currentTime;
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
