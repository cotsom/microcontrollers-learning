void setup() {
Serial.begin(9600);
pinMode(7, INPUT_PULLUP);
}
void loop() {
  Serial.println(digitalRead(7));
  
}
