#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PubSubClient.h>

#define buttonPin 12
#define ledPin 13

char NAME[] = "home-io-controller";
char VERSION[] = "0.0.1";

int buttonState = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
byte remote[] = { 192, 168, 1, 111 };

EthernetClient ethClient;
EthernetServer server(80);

PubSubClient clientbla(remote, 1883, callback, ethClient);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
  server.begin();

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.println("Try connect ....");
  if (clientbla.connect("arduinoClient")) {
    Serial.println("Connected to server");
    clientbla.publish("outTopic","hello world");
    clientbla.subscribe("inTopic");
  }

}

void loop()
{
  endpoint();

  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  /*if (buttonState == LOW) {
    clientbla.publish("outTopic","buttonx clicked");
  }*/

  if(clientbla.connected()) {
    clientbla.publish("outTopic","hello world");
}

  delay(1000);
}


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.println("RECEIVED mqtt data");
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
