#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>

//----------------------Led--------------------------------------------
// How many leds in your strip?
#define NUM_LEDS 20

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 2

// Define the array of leds
CRGB leds[NUM_LEDS];
//---------------------------------------------------------------------

int rainbowFlag = 0;
int blinkFlag = 0;
int glowFlag = 0;

// WiFi
const char *ssid = "Potee_2G"; // Enter your WiFi name
const char *password = "Huttka12";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "srv1.clusterfly.ru";
const char *topic = "user_9f820c56/топик";
const char *mqtt_username = "user_9f820c56";
const char *mqtt_password = "pass_405f3eab";
const int mqtt_port = 9124;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  //Led
  pinMode(LED_BUILTIN, OUTPUT);
  FastLED.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(84);
  
 // Set software serial baud to 115200;
 Serial.begin(115200);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe
 client.publish(topic, "Hi EMQX I'm ESP32 ^^");
 client.subscribe(topic);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }


void callback(char *topic, byte *payload, unsigned int length) {
 static uint8_t hue = 0; 
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 String _message = "";
 for (int i = 0; i < length; i++) {
  
     Serial.print((char) payload[i]);
     _message.concat((char) payload[i]);
 }

  /*
 switch(_message){
   case "green":
      rainbowFlag = 0;
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(255, 255, 255);
      }
      FastLED.show();
      break;
      
   case "pink":
      rainbowFlag = 0;
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(128, 255, 255);
      }
      FastLED.show();
      break;

   case "blue":
      rainbowFlag = 0;
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(160, 255, 255);
      }
      FastLED.show();
      break;

   case "white":
      rainbowFlag = 0;
      for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(100, 0, 255);
      }
      FastLED.show();
      break;

   case "rainbow":
      rainbowFlag = 1;
      break;

   case "blink":
      blinkFlag = 1;
      break;

   case "stop":
      rainbowFlag = 0;
      break;
      
 }
 */
 if(_message == "green"){
  rainbowFlag = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(255, 255, 255);
  }
    FastLED.show(); 

 }
 
 if(_message == "pink"){
  rainbowFlag = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(128, 255, 255);
  }
    FastLED.show(); 

 }
 
 if(_message == "blue"){
  rainbowFlag = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(180, 255, 255);
  }
    FastLED.show(); 

 }
 
 if(_message == "white"){
  rainbowFlag = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(100, 0, 255);
  }
    FastLED.show(); 

 }
 
 if(_message == "rainbow"){
  glowFlag = 0;
  rainbowFlag = 1;
 }
 
 if(_message == "blink"){
  blinkFlag = 1;
 }

 if(_message == "glow"){
  rainbowFlag = 0;
  glowFlag = 1;
 }
 
 if(_message == "stop"){
  rainbowFlag = 0;
  glowFlag = 0;
 }

 
 Serial.println();
 Serial.println("-----------------------");
}

void loop() {
 client.loop();
 //client.publish(topic, "on");
 //delay(1500);

 //client.publish(topic, "off");
 //delay(1500);
 if (rainbowFlag){
  static uint8_t hue = 0;
    // First slide the led in one direction
    for(int i = 0; i < NUM_LEDS; i++) {
      // Set the i'th led to red 
      leds[i] = CHSV(hue++, 255, 255);
      leds[i];
      // Show the leds
      FastLED.show(); 
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();
      // Wait a little bit before we loop around and do it again
      delay(50);
    }
  
    // Now go in the other direction.  
    for(int i = (NUM_LEDS)-1; i >= 0; i--) {
      // Set the i'th led to red 
      leds[i] = CHSV(hue++, 255, 255);
      // Show the leds
      FastLED.show();
      // now that we've shown the leds, reset the i'th led to black
      // leds[i] = CRGB::Black;
      fadeall();
      // Wait a little bit before we loop around and do it again
      delay(50);
    }
  //flag = 0;
 }

 if (blinkFlag){
  for(int i = 0; i < 2; i++){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(128, 255, 255);
    }
    FastLED.show();
    fadeall();
    delay(110);

    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(180, 255, 255);
    }
    FastLED.show();
    fadeall();
    delay(110);
  }
  blinkFlag = 0;
 }

 if (glowFlag){
    static uint8_t hue = 0;
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(hue++, 255, 255);
    }
    FastLED.show(); 
    fadeall();
    delay(350);
  
    for(int i = (NUM_LEDS)-1; i >= 0; i--) {
      leds[i] = CHSV(hue++, 255, 255);
    }
    FastLED.show();
    fadeall();
    delay(350);
 }
 
}
