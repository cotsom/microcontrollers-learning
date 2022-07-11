#include<Servo.h>
Servo myServo;
void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  myServo.write(0);
}

int i = 0;

void loop() {
  if(i > 180){
    i = 180;
  }
  if(i < 0){
    i = 0;
  }
  
  Serial.println(digitalRead(7));
  Serial.println(i);
  
  int buttInfo = digitalRead(7);
  int buttInfo1 = digitalRead(8);
  
  if(buttInfo == 0){
    i += 2;
    myServo.write(i);
  }
  if(buttInfo1 == 0){
    i -= 2;
    myServo.write(i);
  }
}
