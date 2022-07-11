void setup() {
Serial.begin(9600);
pinMode(7, INPUT_PULLUP);
pinMode(8, OUTPUT);
}
void loop() {
  Serial.println(digitalRead(7));
  if(digitalRead(7) == 0){
    digitalWrite(8, HIGH);
  } else{
    digitalWrite(8, LOW);
  }
  
}
