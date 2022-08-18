#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>


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
            

void setup() {
  Serial.begin(115200);
  wifiPointSetup();
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
 Serial.println("Connected to the WiFi network");
}

void loop() {
  //digitalWrite(23, LOW);
  //dnsServer.processNextRequest();
  //webServer.handleClient();
}
