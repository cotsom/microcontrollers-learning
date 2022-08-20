#include "EEPROM.h"

int addr = 0;
#define EEPROM_SIZE 64

// the sample text which we are storing in EEPROM
String ssid = "Potee_2G";
String savedSsid;
String password = "Huttka12";
String savedPassword;

void setup() {
    Serial.begin(115200);
    Serial.println("starting now...");

    if (!EEPROM.begin(EEPROM_SIZE)) {
        Serial.println("failed to init EEPROM");
        delay(1000000);
    }
    //------------------------------------------------------------------
    // writing byte-by-byte to EEPROM
    for (int i = 0; i < ssid.length(); i++) {
        EEPROM.write(addr, ssid[i]);
        addr += 1;
    }
    EEPROM.write(addr, '\0');
    addr += 1;
    int addrForPass = addr;

    for (int i = 0; i < password.length(); i++) {
        EEPROM.write(addr, password[i]);
        addr += 1;
    }
    EEPROM.write(addr, '\0');
    EEPROM.commit();


    //------------------------------------------------------------------
    // reading byte-by-byte from EEPROM
    //int iter = 0;
    //while(EEPROM.read(iter != '\0')){
    //  byte readValue = EEPROM.read(i);
    //}
    
    for (int i = 0; i < EEPROM_SIZE; i++) {
        byte ssidEEPROM = EEPROM.read(i);

        if (ssidEEPROM == '\0') {
            break;
            addr = i+1;
            Serial.print(addr);
        }

        savedSsid += char(ssidEEPROM);
    }
    Serial.println(savedSsid);
    
    for (int i = addrForPass; i < EEPROM_SIZE; i++) {
        byte passwordEEPROM = EEPROM.read(i);

        if (passwordEEPROM == '\0') {
            break;
            addr = i+1;
        }
  
        savedPassword += char(passwordEEPROM);
        //Serial.print("lololololo");
    }
    Serial.print(savedPassword);
}

void loop() {

}
