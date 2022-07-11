#include <NewPing.h>
 
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200

#define ALARM_PIN 6

boolean ALARM_ON = false;
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
 
void setup()
{
    Serial.begin(9600);
    pinMode(ALARM_PIN, OUTPUT);
}
 
void loop()
{
    int uS = sonar.ping();
    long length_cm = uS / US_ROUNDTRIP_CM;
    
    if (length_cm > 0) {
        if (length_cm < 10) {
            if (!ALARM_ON) {
                digitalWrite(ALARM_PIN, HIGH);  
                ALARM_ON = true; 
            }
        } else {
            if (ALARM_ON) {
                digitalWrite(ALARM_PIN, LOW);
                ALARM_ON = false; 
            }   
        }
    }
    
    Serial.print("Ping: ");
    Serial.print(length_cm);
    Serial.println("cm");
    
    delay(50);
}
