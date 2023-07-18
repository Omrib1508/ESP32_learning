#include <Arduino.h>
#include <WiFi.h>

const uint8_t RedLed = 26; // led at GPIO D26
const uint8_t GreenLed = 27; // led at GPIO D27

const char *ssid = "benshahar";  
const char *password = "9263139926";
int         counter = 0;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String RedLedState = "off";   // status of the red led
String GreenLedState = "off"; // status of the green led

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const    long timeout = 3000;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);

  digitalWrite(RedLed, LOW);
  digitalWrite(GreenLed, LOW);

  Serial.print("Conntecting to newtork:");
  Serial.println(ssid);
  initWiFi();

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address is: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client");          // print a message out in the serial port
    
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && (currentTime - previousTime <= timeout)) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        
        char readByte = client.read();             // read a byte, then
        Serial.write(readByte);                    // print it out the serial monitor
        header += readByte;
        if (readByte == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /GPIO26(RedLed)/on") >= 0) {
              Serial.println("RedLed is on");
              RedLedState = "on";
              digitalWrite(RedLed, HIGH);
            } else if (header.indexOf("GET /GPIO26(RedLed)/off") >= 0) {
              Serial.println("RedLed is off");
              RedLedState = "off";
              digitalWrite(RedLed, LOW);
            } else if (header.indexOf("GET /GPIO26(GreenLed)/on") >= 0) {
              Serial.println("GreenLed is on");
              GreenLedState = "on";
              digitalWrite(GreenLed, HIGH);
            } else if (header.indexOf("GET /GPIO26(GreenLed)/off") >= 0) {
              Serial.println("GreenLed is off");
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
            client.println("<p>GPIO 26 - State " + RedLedState + "</p>");
            // If the output26State is off, it displays the ON button       
            if (RedLedState=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State " + GreenLedState + "</p>");
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
            currentLine = "";
          }
        } else if (readByte != '\r') {  // if you got anything else but a carriage return character,
          currentLine += readByte;      // add it to the end of the currentLine
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
