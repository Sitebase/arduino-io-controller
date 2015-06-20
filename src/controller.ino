#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PubSubClient.h>

#define buttonPin 5
#define ledPin 13

char NAME[] = "home-io-controller";
char VERSION[] = "0.0.1";

int buttonState = 0;
bool puby = false;
int buttonCheck = 0;

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

    pinMode(buttonPin, INPUT_PULLUP);
    Button1.SetStateAndTime(LOW, 10);

    if (!clientbla.connected()) {
        if (clientbla.connect("arduinoClient")) {
            Serial.println("Connected to server");
        }
    }

}

void loop()
{

    // auto reconnect
    if (!clientbla.connected()) {
        if (clientbla.connect("arduinoClient")) {
            Serial.println("Connected to server");
        }
    }

    endpoint();

    if( puby ) {
        char buffer[40];
        sprintf(buffer,"{\"action\":\"input\",\"pin\":%d}",buttonCheck);
        clientbla.publish("outTopic", buffer);
        puby = false;
    }


    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
        // turn LED on:
        puby = true;
        buttonCheck = buttonPin;
    }

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
