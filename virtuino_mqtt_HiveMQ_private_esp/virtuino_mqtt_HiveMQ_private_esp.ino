#include <WiFi.h>
#include <PubSubClient.h>

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
  pinMode(LED_BUILTIN, OUTPUT);
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

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 String _message = "";
 for (int i = 0; i < length; i++) {
  
     Serial.print((char) payload[i]);
     _message.concat((char) payload[i]);
 }
 if(_message == "on"){
  digitalWrite(LED_BUILTIN, HIGH);
 }else{
  digitalWrite(LED_BUILTIN, LOW);
 }
 Serial.println();
 Serial.println("-----------------------");
}

void loop() {
 client.loop();
 client.publish(topic, "on");
 delay(1500);

 client.publish(topic, "off");
 delay(1500);
 
}
