#include "SevSeg.h" // импорт библиотеки

SevSeg sevseg;      // инициализация объекта SevSeg

void setup() {
    byte numDigits = 1;       // количество символов в нашем индикаторе (в нашем примере = 1)
    byte digitPins[] = {2};   // массив номеров выходов Arduino, отвечающих за переключение цифр на индикаторе (в нашем примере цифра одна, поэтому элемент массива тоже один)
    
    byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; // выходы UNO для всех сегментов A, B, C, D, E, F, G, DP соответственно
    
    sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins); // настройка библиотеки для работы с семисегментниками
}

void loop() {
    static unsigned long timer = millis();
    static int deciSeconds = 0;
  
    if (millis() >= timer) {
        deciSeconds++;
        timer += 1000; 
        if (deciSeconds == 10) {
            deciSeconds = 0;
        }
        
        sevseg.setNumber(deciSeconds, 1);
    }
    
    sevseg.refreshDisplay();
}