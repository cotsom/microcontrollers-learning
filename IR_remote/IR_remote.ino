#include <IRremote.h>
#include <Servo.h>
#include <NewPing.h>

#define TriggerPin 12
#define EchoPin 11
#define MaxDistance 200
#define AlarmPin 6

IRrecv remote(8); //Пин с логикой пульта
decode_results results; // Объект от пульта

Servo myServo; // Объект от сервопривода

NewPing sonar(TriggerPin, EchoPin, MaxDistance); // Объект от дальнометра

short i = 0;

void setup() {
  Serial.begin(9600);
  remote.enableIRIn();

  myServo.attach(9);
  myServo.write(0);

  pinMode(8, INPUT);// Пин с логикой пульта

  pinMode(6, OUTPUT);// Alarm Pin

}

void loop() {
  delay(50);
  unsigned int distance = sonar.ping_cm();
  
  if(i > 180){
    i = 180;
  }
  if(i < 0){
    i = 0;
  }
  
  if(remote.decode(&results)){
    Serial.println(results.value);
    
    if(results.value == 16769055){
      i -= 30;
      myServo.write(i);
    }else if(results.value == 16748655){
      i += 30;
      myServo.write(i);
    }

    remote.resume();
  }
  
 if(distance < 5){
    digitalWrite(AlarmPin, HIGH);
  }else{
    digitalWrite(AlarmPin, LOW);
  }


}
