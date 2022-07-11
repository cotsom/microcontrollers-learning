#include <IRremote.h>

IRrecv remote(8); //Пин с логикой пульта
decode_results results; // Объект от пульта

void setup() {
  Serial.begin(9600);
  remote.enableIRIn();
  pinMode(8, INPUT); // Пин с логикой пульта
  pinMode(13, OUTPUT); //Пин для мотора

}

int power = 0;

void loop() {
  if(remote.decode(&results)){
    Serial.println(results.value);

    if((results.value == 16753245) && (power == 0)){
      digitalWrite(13, HIGH);
      power = 1;
    }else if((results.value == 16753245) && (power == 1)){
      digitalWrite(13, LOW);
      power = 0;
    }
    remote.resume();
  }
}
