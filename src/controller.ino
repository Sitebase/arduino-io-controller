#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

int led = 13;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

EthernetClient ethClient;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac,ip);
}

void loop() {

  Serial.print("This is a network test");
  delay(1000);
}
