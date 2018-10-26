/*
  Web Server
  Need to install Ethernet2 arduino library
  If new arduino esp32 or make error, need go to c:\Program Files (x86)\Arduino\hardware\espressif\arduino-esp32\cores\esp32\Server.h 
  Change virtual void begin(uint16_t port = 0) = 0; to virtual void begin() = 0;
  Other example can see https://github.com/adafruit/Ethernet2
 */
#include <M5Stack.h>
#include <SPI.h>
#include <Ethernet2.h>
#define SCK 18
#define MISO 19
#define MOSI 23
#define CS 26

//  01 05 00 01 02 00 9d 6a
char uart_buffer[8] = {0x01, 0x05, 0x00, 0x01, 0x02, 0x00, 0x9d, 0x6a};
char uart_rx_buffer[8] = {0};

char Num = 0;
char stringnum = 0;
unsigned long W5500DataNum = 0;
unsigned long Send_Num_Ok = 0;
unsigned long Rec_Num = 0;
unsigned long Rec_Num_Ok = 0;


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  M5.begin(true, false, true);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  SPI.begin(SCK, MISO, MOSI, -1);
  Ethernet.init(CS);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  M5.Lcd.println("M5Stack W5500 Test");
  M5.Lcd.println(" ");
  M5.Lcd.print(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");

          client.println("<body>");
          client.println("<h1>M5Stack W5500 Test</h1>");
          client.println("<br />");
          client.println("<p>Please click here</p>");
          client.println("<a href=\"http://www.M5Stack.com\">M5Stack</a>");
          client.println("<br />");
          client.println("<br />");
          client.println("<br />");

          
          client.print("W5500 Counter Num :");
          client.print(W5500DataNum);
          client.println("<br />");
          client.println("<br />");
          W5500DataNum ++;
          
          client.print("Rec_Num_Ok Counter :");
          client.print(Rec_Num_Ok);
          client.println("<br />");
          client.println("<br />");

          client.print("Rec_Num Counter :");
          client.print(Rec_Num);
          client.println("<br />");
          client.println("<br />");

        
        
          client.println("</body>");

          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

