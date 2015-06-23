#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <String.h>

#define channel 0

int previousState = 0;
int currentState = 0;

int counter = 0;
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
byte remote[] = { 192, 168, 1, 111 };

EthernetClient ethClient;
EthernetServer server(80);

void callback(char* topic, byte* payload, unsigned int length) {

    Serial.print("callback fired: ");
    Serial.print(topic);
    Serial.print(" - ");
    Serial.print((char)payload[0]);
    Serial.print(",");
    Serial.print(payload[1]);
    Serial.print(",");
    Serial.print(payload[2]);
    Serial.print(",");
    Serial.print(payload[3]);
    Serial.print(",");
    Serial.print(payload[4]);
    Serial.println(";");

    if( (char)payload[0] == '{' )
    {
        Serial.println("JSON detected");
    }
}

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
            clientbla.subscribe("command");
        }
    }

}

void loop()
{

    // auto reconnect
    if (!clientbla.connected()) {
        if (clientbla.connect("arduinoClient")) {
            Serial.println("Connected to server");
            clientbla.subscribe("command");
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

    counter++;
    if( counter > 20000)
    {
        sensors();

        int analog0 = analogRead(A0);
        int analog1 = analogRead(A1);
        int analog2 = analogRead(A2);
        int analog3 = analogRead(A3);
        int analog4 = analogRead(A4);
        int analog5 = analogRead(A5);


        // Reading temperature or humidity takes about 250 milliseconds!
          // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
          float h = dht.readHumidity();
          // Read temperature as Celsius
          float t = dht.readTemperature();
          Serial.print("temp: ");
          Serial.println(t);
          Serial.print("hum: ");
          Serial.println(h);

          //Serial.println(getDecimal(t));
          int tester = int(t);
          String value = String(tester);
          Serial.println(tester);
          Serial.println(value);

        char* output = "{\"c\":%d,\"cmd\":\"ar\",\"val0\":%d,\"val1\":%d,\"val2\":%d,\"val3\":%d,\"val4\":%d,\"val5\":%d}";
        int olength = strlen(output) + sizeof(channel) + sizeof(analog0) + sizeof(analog1) + sizeof(analog2) + sizeof(analog3) + sizeof(analog4) + sizeof(analog5);
        char buf2[olength];

        // construct JSON message
        sprintf(buf2, output, channel, analog0, analog1, analog2, analog3, analog4, analog5);

        clientbla.publish("state", buf2);
        counter = 0;
    }

    clientbla.loop();

}

void sensors()
{
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t = dht.readTemperature();
    Serial.print("temp: ");
    Serial.println(t);
    Serial.print("hum: ");
    Serial.println(h);

    //Serial.println(getDecimal(t));
    int temp = int(t);
    int hum = int(h);

    char* output = "{\"c\":%d,\"cmd\":\"sensor\",\"temperature\":%d,\"humidity\":%d}";
    int olength = strlen(output) + sizeof(channel) + sizeof(temp) + sizeof(hum);
    char buf2[olength];

    // construct JSON message
    sprintf(buf2, output, channel, temp, hum);

    clientbla.publish("/state/sensor", buf2);
}


//function to extract decimal part of float
long getDecimal(float val)
{
 int intPart = int(val);
 long decPart = 1000*(val-intPart); //I am multiplying by 1000 assuming that the foat values will have a maximum of 3 decimal places
                                   //Change to match the number of decimal places you need
 if(decPart>0)return(decPart);           //return the decimal part of float number if it is available
 else if(decPart<0)return((-1)*decPart); //if negative, multiply by -1
 else if(decPart=0)return(00);           //return 0 if decimal part of float number is not available
}
