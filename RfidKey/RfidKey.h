/**************************************************************
 * RfidKey
 * (c) 2024 Juan M. Uribe
 *
 * This library allows easy use of the MFRC522 RFID card readers
 ***************************************************************/
#pragma once
#include <SPI.h>
#include <MFRC522.h>
#include <CRC16.h>
#include <CRC.h>

#define REG_COUNT 10 // Maximum size of key

typedef struct
{
    uint8_t keyUID[REG_COUNT];
    uint8_t keySize;
} UID_t;

class rfidKey
{
private:
    UID_t UID;
    UID_t lastUID;
    uint16_t keyCRC;
    uint16_t keyCS, keyReset;
    MFRC522 mfrc522;
    void (*onKeyFuncPtr)(UID_t &UID) = NULL;
    void (*onNewKeyFuncPtr)(UID_t &UID) = NULL;

public:
    // Constructor
    rfidKey(uint16_t cs, uint16_t reset);

    // Process the key. Must to be include in the loop
    void loop(void);

    // Returns the last readed UID
    void getUID(UID_t &uid);

    // Returns a 16 bits CRC of the last readed UID
    uint16_t getCRC(void);

    // Returns the size of the last readed UID
    uint8_t getKeySize(void);

    // Prints the last readed UID to Serial
    void printUID(void);

    // Sets a callback function called once a key is readed
    // and pass it the readed UID
    void onKey(void (*FunPtr)(UID_t &uid));

    // Sets a callback function called once the readed key is different
    // to the last readed key and pass it the readed UID
    void onNewKey(void (*FunPtr)(UID_t &uid));

    // Reset UID to all zero
    void resetUID(void);

    // Reset lastUID to all zero
    void resetLastUID(void);
};

/***********************************************
 * AUXILIARY FUNCTIONS
 **********************************************/
// operator == comparing UIDs
bool operator==(UID_t &uid1, UID_t &uid2);

// operator != comparing UIDs
bool operator!=(UID_t &uid1, UID_t &uid2);

// Prints an UID to Serial
void printUID(UID_t &uid);
