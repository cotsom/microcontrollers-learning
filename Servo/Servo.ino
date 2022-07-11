#include<Servo.h>
Servo myServo;
void setup() {
  Serial.begin(9600);
  myServo.attach(9);
  pinMode(7, INPUT_PULLUP);

}

int i = 0;

void loop() {
point:
  Serial.println(digitalRead(7));
  Serial.println(i);
  
  int buttInfo = digitalRead(7);
  
  if(buttInfo == 0){
    if(i == 180){
      myServo.write(0);
      i = 0;
      delay(1000);
      goto point;
    }
    i += 2;
    myServo.write(i);
    //delay(40);
    
  }
}
