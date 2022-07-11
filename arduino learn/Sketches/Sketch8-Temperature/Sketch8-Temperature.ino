#define TEMPSENSOR_PIN 0 // пин, к которому подключен сенсор

void setup()
{
    Serial.begin(9600);
}
 
void loop()
{
    float tempc = 0;
    for (int i = 0; i <= 7; i++) {
        float temperatureValue = (5.0 * analogRead(TEMPSENSOR_PIN) * 100.0) / 1024.0;
        tempc = tempc + temperatureValue;
        delay(100);
    }
    
    tempc = tempc / 8.0;
    
    Serial.print(tempc, DEC);
    Serial.println(" celsius");
    delay(200);
}
