#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PubSubClient.h>

char NAME[] = "home-io-controller";
char VERSION[] = "0.0.1";

int buttonState = 0;
bool puby = false;
int buttonCheck = 0;

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

    /*pinMode(1, INPUT_PULLUP);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);*/
    pinMode(5, INPUT_PULLUP);
    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    /*pinMode(9, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);*/

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

    //endpoint();

    /*if( puby ) {
        char buffer2[40];
        char buffer[40];
        sprintf(buffer,"{\"action\":\"input\",\"pin\":%d}",buttonCheck);
        sprintf(buffer2,"[%s]",buffer);

        clientbla.publish("outTopic", buffer2);
        puby = false;
    }*/

    if( currentState ^ 0 ) {
        //for(int i=1; i < 1024 ; i=i*2)  {
            //Serial.println(i);
            //bla += "nog";
            /*if( currentState & i ) {

                char buffer[40];
                sprintf(buffer,"{\"action\":\"input\",\"pin\":%d},",i);
                bla += buffer;

                Serial.print("MATCH FOUND: ");
                Serial.println(i);
            }*/
        //}

        //Serial.println(bla);

        char buf[3];
        sprintf(buf, "%d", (previousState ^ currentState));
        Serial.println(previousState);
        Serial.println(currentState);
        Serial.println("************");
        clientbla.publish("outTopic", buf);

        // reset
        //previousState = currentState; // don't use this because its a super debounce that lets you only click once until you press another button
        currentState = 0;
    }

    // NOTE when I enable the two digital read lines it doesnt connect anymore to the mqtt server
    // i think its a memory problem so maybe remove the rest endpoint from the code

    /*if( digitalRead(1) == LOW ) currentState = currentState | 1;
    if( digitalRead(2) == LOW ) currentState = currentState | 2;
    if( digitalRead(3) == LOW ) currentState = currentState | 4;
    if( digitalRead(4) == LOW ) currentState = currentState | 8;*/
    if( digitalRead(5) == LOW ) currentState = currentState | 16;
    if( digitalRead(6) == LOW ) currentState = currentState | 32;
    if( digitalRead(7) == LOW ) currentState = currentState | 64;
    if( digitalRead(8) == LOW ) currentState = currentState | 128;
    /*if( digitalRead(9) == LOW ) currentState = currentState | 256;
    if( digitalRead(10) == LOW ) currentState = currentState | 512;*/


}


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.println("RECEIVED mqtt data");
}

/**
 * listen for an endpoint request and respond
 * with information about the current service
 */
/*void endpoint()
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
}*/
