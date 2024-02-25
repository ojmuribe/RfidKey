#include "RfidKey.h"

rfidKey::rfidKey(uint16_t cs, uint16_t reset)
{
    SPI.begin();
    mfrc522.PCD_Init(cs, reset);
    uint8_t v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    if ((v == 0x00) || (v == 0xFF))
        mfrc522.PCD_Init(cs, reset);
    keyCS = cs;
    keyReset = reset;
    resetUID();
    resetLastUID();
}

void rfidKey::loop(void)
{
    byte bufferATQA[2];
    byte bufferSize = sizeof(bufferATQA);
    MFRC522::StatusCode status = mfrc522.PICC_RequestA(bufferATQA, &bufferSize);
    if (status != 0)
        mfrc522.PCD_Init(keyCS, keyReset);
    bool newCard = mfrc522.PICC_IsNewCardPresent();
    if (newCard)
    {
        if (mfrc522.PICC_ReadCardSerial())
        {
            for (int i = 0; i < mfrc522.uid.size; i++)
            {
                UID.keyUID[i] = mfrc522.uid.uidByte[i];
            }
            UID.keySize = mfrc522.uid.size;
            uint8_t buffer[UID.keySize];
            memcpy(&buffer, &UID.keyUID, sizeof(UID.keyUID));
            keyCRC = calcCRC16((uint8_t *)buffer, sizeof(buffer));
        }
        mfrc522.PICC_HaltA();
        if (UID != lastUID)
        {
            if (onNewKeyFuncPtr != NULL)
                onNewKeyFuncPtr(UID);
        }
        lastUID = UID;

        if (onKeyFuncPtr != NULL)
            onKeyFuncPtr(UID);
    }
}

void rfidKey::getUID(UID_t &uid)
{
    uid = UID;
}

uint16_t rfidKey::getCRC(void)
{
    return keyCRC;
}

uint8_t rfidKey::getKeySize(void)
{
    return UID.keySize;
}

void rfidKey::printUID()
{
    for (int i = 0; i < UID.keySize; i++)
    {
        Serial.printf("%02X ", UID.keyUID[i]);
    }
    Serial.println();
}

void rfidKey::onKey(void (*FunPtr)(UID_t &uid))
{
    onKeyFuncPtr = FunPtr;
}

void rfidKey::onNewKey(void (*FunPtr)(UID_t &uid))
{
    onNewKeyFuncPtr = FunPtr;
}

void rfidKey::resetUID(void)
{
    for (int i = 0; i < UID.keySize; i++)
        UID.keyUID[i] = 0X00;
    UID.keySize = 0;
}

void rfidKey::resetLastUID(void)
{
    for (int i = 0; i < lastUID.keySize; i++)
        lastUID.keyUID[i] = 0X00;
    lastUID.keySize = 0;
}

void printUID(UID_t &uid)
{
    for (int i = 0; i < uid.keySize; i++)
    {
        Serial.printf("%02X ", uid.keyUID[i]);
    }
    Serial.println();
}

bool operator!=(UID_t &uid1, UID_t &uid2)
{
    bool result = false;
    if (uid1.keySize != uid2.keySize)
        result = true;
    else
    {
        for (int i = 0; i < uid1.keySize; i++)
        {
            if (uid1.keyUID[i] != uid2.keyUID[i])
                result = true;
        }
    }
    return result;
}

bool operator==(UID_t &uid1, UID_t &uid2)
{
    return !(uid1 != uid2);
}
