//==================================
//==================================
//
// Connect to AirVantage using MQTT :
// 
// A voting machine counting the 
// number of vote for each color.
//
//==================================
//==================================

// for MQTT and ethernet
#include <SPI.h>
#include <string.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <stdlib.h>

// debounce buttons

#include <Bounce.h>
#define BUTTON_MINUS_1  2
#define BUTTON_ZERO   3
#define BUTTON_PLUS_1  4

// Instantiate Bounce objects with a 5 millisecond debounce time
Bounce bouncer_red =  Bounce(BUTTON_MINUS_1,5); 
Bounce bouncer_yellow =       Bounce(BUTTON_ZERO,5); 
Bounce bouncer_green =   Bounce(BUTTON_PLUS_1,5); 

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte server[] = { 10,41,51,48 };
// M2.eclipse.org byte server[] = { 198,41,30,241 };
byte ip[]     = { 10,41,51,38 };

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

char string1[] = "[{\"red\":[{\"timestamp\":null,\"value\":";
char string2[] = "[{\"yellow\":[{\"timestamp\":null,\"value\":";
char string3[] = "[{\"green\":[{\"timestamp\":null,\"value\":";
char string4[] = "}]}]";

char TMP[10];
char BUFFER[150];

void setup() {
  pinMode(BUTTON_MINUS_1,INPUT);
  pinMode(BUTTON_ZERO,INPUT);
  pinMode(BUTTON_PLUS_1,INPUT);
  
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip);
  if (client.connect("crazymachine", "voting", "$$$$$$$$")) {
    Serial.println("connected");
  } else {
    Serial.println("No connection :/");
  }
}

int red = 0;
int yellow = 0;
int green = 0;

void loop() {
  // Update the debouncers
 bouncer_red.update();
 bouncer_yellow.update();
 bouncer_green.update();
 
 red += (bouncer_red.risingEdge() ? 1 : 0);
 yellow += (bouncer_yellow.risingEdge() ? 1 : 0);  
 green += (bouncer_green.risingEdge() ? 1 : 0);  
 
 
 if (bouncer_red.risingEdge()) {
   strcpy(BUFFER,string1);
   itoa(red,TMP,10);
   strcat(BUFFER,TMP);
   strcat(BUFFER,string4);
   Serial.write(BUFFER);
   Serial.println();
   int result = client.publish("voting/messages/json",BUFFER);
   Serial.println(result);
 }

 if (bouncer_yellow.risingEdge()) {
   strcpy(BUFFER,string2);
   itoa(yellow,TMP,10);
   strcat(BUFFER,TMP);
   strcat(BUFFER,string4);
   Serial.write(BUFFER);
   Serial.println();
   int result = client.publish("voting/messages/json",BUFFER);
   Serial.println(result);
 }
 
  if (bouncer_green.risingEdge()) {
   strcpy(BUFFER,string3);
   itoa(green,TMP,10);
   strcat(BUFFER,TMP);
   strcat(BUFFER,string4);
   Serial.write(BUFFER);
   Serial.println();
   int result = client.publish("voting/messages/json",BUFFER);
   Serial.println(result);
 }
 client.loop();
}
