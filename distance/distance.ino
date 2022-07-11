#include<NewPing.h>

#define TriggerPin 12
#define EchoPin 11
#define MaxDistance 200
#define AlarmPin 6
boolean AlarmON = false;

NewPing sonar(TriggerPin, EchoPin, MaxDistance);

void setup() {
  Serial.begin(9600);
  pinMode(6, OUTPUT);

}

void loop() {
  delay(50);
  unsigned int distance = sonar.ping_cm();
    
  if(distance < 5){
    digitalWrite(AlarmPin, HIGH);
  }else{
    digitalWrite(AlarmPin, LOW);
  }
}
