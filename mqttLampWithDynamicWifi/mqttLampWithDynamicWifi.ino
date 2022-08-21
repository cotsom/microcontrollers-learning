#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <FastLED.h>
#include <PubSubClient.h>
#include "EEPROM.h"


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


// MQTT Broker
const char *mqtt_broker = "srv1.clusterfly.ru";
const char *topic = "user_9f820c56/топик";
const char *mqtt_username = "user_9f820c56";
const char *mqtt_password = "pass_405f3eab";
const int mqtt_port = 9124;

WiFiClient espClient;
PubSubClient client(espClient);

//EEPROM
int addr = 0;
int addrForPass;
#define EEPROM_SIZE 256
String savedSsid;
String savedPassword;

int wifiDelay = 0;
int wifiPointSetupFlag = 0;


int captiveFlag = 1;

const byte DNS_PORT = 53;
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
WebServer webServer(80);
String ssid;
String password;
int counter = -1;
String logpass[100];

String responseHTML = ""
  "<!DOCTYPE html>"
"<html>"
"<head>"
  "<meta charset='utf-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>"
  "<title>Wifi credentionals</title>"
"</head>"
"<body>"
  "<form method='POST' action='/handleForm'>"
    "<input type='text' name='ssid' placeholder='name of your network (ssid)'>"
    "<input type='password' name='pass' placeholder='password of your network'>"
    "<input type='submit' name='submit'>"
  "</form>"
"</body>"
"</html>";


void handleForm() {
   counter++;
   ssid = webServer.arg("ssid"); 
   password = webServer.arg("pass");
  
   Serial.print("Ssid:");
   Serial.println(ssid);
  
   Serial.print("password:");
   Serial.println(password);

   writeToEEPROM(ssid, password);
   
   captiveFlag = 0;
   webServer.stop();
   webServer.close();
   WiFi.softAPdisconnect (true);
   //webServer.send(200, "text/html", responseHTML); //Send web page
   
}

void wifiPointSetup(){
  pinMode(23, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("set wifi creds");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  webServer.on("/handleForm", handleForm);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  
  webServer.begin();

  while(captiveFlag){
    digitalWrite(23, LOW);
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}

void eepromSetup(){
    Serial.println("starting now...");

    if (!EEPROM.begin(EEPROM_SIZE)) {
        Serial.println("failed to init EEPROM");
        delay(1000000);
    }
}

void writeToEEPROM(String ssid, String password){
  //Serial.print("Было записано");
  //Serial.println(ssid);
  //Serial.println(password);
  addr = 0;
  // writing byte-by-byte to EEPROM
    for (int i = 0; i < ssid.length(); i++) {
        EEPROM.write(i, ssid[i]);
        addr += 1;
    }
    EEPROM.write(addr, '\0');
    addr++;
    addrForPass = addr;

    //Serial.print("write from");
    //Serial.println(addr);
    for (int i = 0; i < password.length(); i++) {
        EEPROM.write(addr, password[i]);
        addr += 1;
    }
    EEPROM.write(addr, '\0');
    EEPROM.commit();
}

void readFromEEPROM(){
  addr = 0;
  for (int i = 0; i < EEPROM_SIZE; i++) {
        byte ssidEEPROM = EEPROM.read(i);

        if (ssidEEPROM == '\0') {
            addr = i+1;
            addrForPass = addr;
            break;
        }
        savedSsid += char(ssidEEPROM);
        addr = i+1;
        addrForPass = addr;
    }
    //Serial.println(addrForPass);
    //Serial.print("ssid from EEPROM: ");
    //Serial.println(savedSsid);

    //Serial.print("read from");
    //Serial.println(addrForPass);
    for (int i = addrForPass; i < EEPROM_SIZE; i++) {
        byte passwordEEPROM = EEPROM.read(i);

        if (passwordEEPROM == '\0') {
            break;
            addr = i+1;
        }
  
        savedPassword += char(passwordEEPROM);
    }
    //Serial.print("password from EEPROM: ");
    //Serial.print(savedPassword);
}
            

void setup() {
  Serial.begin(115200);
  //Led
  pinMode(LED_BUILTIN, OUTPUT);
  FastLED.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  FastLED.setBrightness(84);
  
  //EEPROM
  eepromSetup();
  readFromEEPROM();
  WiFi.begin(savedSsid.c_str(), savedPassword.c_str());
  while (WiFi.status() != WL_CONNECTED) {
      if(wifiDelay == 10){
        wifiPointSetupFlag = 1;
        break;
      }
      delay(1000);
      wifiDelay++;
      Serial.println("Connecting to WiFi..");
  }
  if(wifiPointSetupFlag){
    wifiPointSetup();
  }
  

  WiFi.begin(ssid.c_str(), password.c_str());
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
    delay(1550);
  
    for(int i = (NUM_LEDS)-1; i >= 0; i--) {
      leds[i] = CHSV(hue++, 255, 255);
    }
    FastLED.show();
    fadeall();
    delay(1550);
 }
}
