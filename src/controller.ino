#include <SPI.h>
#include <String.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 192, 168, 1, 117 };
byte ip[]     = { 192, 168, 1, 188 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

void setup() {

    Ethernet.begin(mac, ip);
    if (client.connect("arduinoClient")) {
        client.publish("outTopic","hello world");
        client.subscribe("inTopic");
    }

    Serial.begin(9600);
    //pinMode(11, OUTPUT);
    pinMode(12, INPUT_PULLUP);
    pinMode(13, INPUT_PULLUP);
    Serial.println("waiting for button press ...");
}

void loop()
{

    client.loop();

    int buttonState1 = digitalRead(12);
    int buttonState2 = digitalRead(13);

    if( buttonState1 == HIGH )
        Serial.println("Button 1 pressed");

    if( buttonState2 == HIGH )
        Serial.println("Button 2 pressed");

    delay(50);
}
