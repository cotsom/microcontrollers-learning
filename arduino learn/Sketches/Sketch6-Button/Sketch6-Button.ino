#define LED_PIN 13
#define BUTTON_PIN 2

void setup()
{
    pinMode(LED_PIN, OUTPUT);      // Настроим пин, к которому подключен светодиод на выход
    pinMode(BUTTON_PIN, INPUT);    // А пин, к которой подключена кнопка, на вход
}

void loop()
{
    int val = digitalRead(BUTTON_PIN);   // считываем значение на пине, к которой подключена кнопка
    if (val == HIGH) { // если кнопка не нажата
        digitalWrite(LED_PIN, LOW);  // гасим светодиод
    } else {           // если нажата
        digitalWrite(LED_PIN, HIGH); // зажигаем светодиод
    }
}