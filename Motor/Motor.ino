void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(7, INPUT_PULLUP);

}

void loop() {
  Serial.println(digitalRead(7));
  if(digitalRead(7) == 0){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
}
