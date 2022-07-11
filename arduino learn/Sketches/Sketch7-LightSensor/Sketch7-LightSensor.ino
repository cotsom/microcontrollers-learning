#define LIGHTSENSOR_PIN 0 // пин, к которому подключен датчик освещенности
#define LED_PIN 13        // пин, к которому подключен светодиод

void setup() {
    pinMode(LED_PIN, OUTPUT); // настраиваем пин, к которому подключен светодиод
}

void loop() {
   int lightLevel = analogRead(LIGHTSENSOR_PIN); // считываем значение
   
   lightLevel = map(lightLevel, 0, 900, 0, 255); 
   
   lightLevel = constrain(lightLevel, 0, 255);
   
   // если уровень освещенности ниже определенного порога, то зажигаем светодиод
   if (lightLevel < 125) {
       digitalWrite(LED_PIN, HIGH);
   } else {
       digitalWrite(LED_PIN, LOW);
   }
}
