#include <Arduino.h>
#include "RfidKey.h"

#define RFID_CS GPIO_NUM_5
#define RFID_RST GPIO_NUM_33
#define RFID_CLK GPIO_NUM_18
#define RFID_MISO GPIO_NUM_19
#define RFID_MOSI GPIO_NUM_23

rfidKey key(RFID_CS, RFID_RST);

UID_t myUID;

void newKeyCallBack(UID_t &uid)
{
    key.getUID(myUID);
    Serial.println("This is a different key: ");
    printUID(myUID);
}

void keyCallBack(UID_t &uid)
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Readed key: ");
    key.printUID();
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    key.onKey(keyCallBack);
    key.onNewKey(newKeyCallBack);
}

void loop()
{
    key.loop();
}