#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

char NAME[] = "home-io-controller";
char VERSION[] = "0.0.1";

int led = 13;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

EthernetClient ethClient;
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  server.begin();
}

void loop() {

  Serial.print("This is a network test");

  endpoint();

  delay(1000);

}

/**
 * listen for an endpoint request and respond
 * with information about the current service
 */
void endpoint()
{
    EthernetClient client = server.available();
    if (client) {
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
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: application/json");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println();
                    client.print("{\"name\":\"");
                    client.print(NAME);
                    client.print("\", \"version\": \"");
                    client.print(VERSION);
                    client.print("\"}");
                    break;
                }
                if (c == '\n') {
                    // you're starting a new line
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    // you've gotten a character on the current line
                    currentLineIsBlank = false;
                }
            }
        }
        // give the web browser time to receive the data
        delay(1);
        client.stop();
    }
}
