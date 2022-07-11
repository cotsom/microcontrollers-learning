#include <IRremote.h>

#define IR_PIN 8
#define BUZER_PIN 10
#define LED_PIN 11

IRrecv irrecv(IR_PIN);

boolean LED_ON = false;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZER_PIN, OUTPUT);
  
    Serial.begin(9600);
    
    irrecv.enableIRIn();
}

void loop() {
    decode_results results;
    if (irrecv.decode(&results)) {
        switch (results.value) {
            case 0xFFA25D:
                LED_ON = !LED_ON;
                
                if (LED_ON) {
                    digitalWrite(LED_PIN, HIGH);
                } else {
                    digitalWrite(LED_PIN, LOW);
                }
                break;
                
            case 0xFF30CF:
                playTone('c', 300);
                break;
                
            case 0xFF18E7:
                playTone('d', 300);
                break;
                
            case 0xFF7A85:
                playTone('e', 300);
                break;
                
            default:
                Serial.print("No command for 0x");
                Serial.println(results.value, HEX);
                break;
        }
        
        delay(50);
        irrecv.resume();
    }
}

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(BUZER_PIN, HIGH);
        delayMicroseconds(tone);
        digitalWrite(BUZER_PIN, LOW);
        delayMicroseconds(tone);
    }
}

void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  
    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}
