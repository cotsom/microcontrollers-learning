#define MOTOR_PIN 9

void setup()
{
    pinMode(MOTOR_PIN, OUTPUT); 
}

void loop()
{
    for (int i = 0; i < 256; i++) {
        analogWrite(MOTOR_PIN, i);
        delay(50);
    }
  
    for (int i = 255; i >= 0; i--) {
        analogWrite(MOTOR_PIN, i);
        delay(50);
    }
}
