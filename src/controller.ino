#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PubSubClient.h>
#include <String.h>

#define channel 0

int previousState = 0;
int currentState = 0;

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

    //pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    //pinMode(4, INPUT_PULLUP);
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    //pinMode(10, INPUT_PULLUP);

    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);

    digitalWrite(12, HIGH);

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

    if( currentState ^ 0 ) {

        // calculate buffer length needed for the JSON message
        int state = (previousState ^ currentState);
        char* format = "{\"c\":%d,\"cmd\":\"dr\",\"val\":%d}";
        int length = strlen(format) + sizeof(channel) + sizeof(state);
        char buf[length];

        // construct JSON message
        sprintf(buf, format, channel, state);

        // publish message
        clientbla.publish("state", buf);

        // reset
        currentState = 0;
    }

    // NOTE when I enable the two digital read lines it doesnt connect anymore to the mqtt server
    // i think its a memory problem so maybe remove the rest endpoint from the code

    //if( digitalRead(1) == LOW ) currentState = currentState | 1; // is always low
    if( digitalRead(2) == LOW ) currentState = currentState | 2;
    if( digitalRead(3) == LOW ) currentState = currentState | 4;
    //if( digitalRead(4) == LOW ) currentState = currentState | 8; // this input is broken on my UNO, it's always low
    if( digitalRead(5) == LOW ) currentState = currentState | 16;
    if( digitalRead(6) == LOW ) currentState = currentState | 32;
    if( digitalRead(7) == LOW ) currentState = currentState | 64;
    if( digitalRead(8) == LOW ) currentState = currentState | 128;
    if( digitalRead(9) == LOW ) currentState = currentState | 256;
    //if( digitalRead(10) == LOW ) currentState = currentState | 512; // doesn't connect to mqtt anymore if you enable this


}

void callback(char* topic, byte* payload, unsigned int length) {
    // handle message arrived
    Serial.print("received payload:");
    Serial.println(topic);
}

