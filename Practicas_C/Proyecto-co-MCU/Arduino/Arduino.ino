/*****************************************************************************
 * ARDUINO UNO - SIMPLE RFID TO I2C (NO SD CARD)
 * 
 * This is a simplified version for troubleshooting
 * Users are stored in memory, no SD card needed
 * 
 * PINOUT:
 * =======
 * RC522 RFID:
 *   SDA  -> D10
 *   SCK  -> D13
 *   MOSI -> D11
 *   MISO -> D12
 *   RST  -> D9
 *   GND  -> GND
 *   3.3V -> 3.3V
 * 
 * I2C to PIC:
 *   SDA  -> A4
 *   SCL  -> A5
 *   GND  -> GND
 * 
 * Status LED:
 *   LED  -> D13 (built-in)
 *****************************************************************************/

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>

// Pin definitions
#define RST_PIN         9
#define SS_PIN          10

// I2C address for PIC18F4550
#define PIC_I2C_ADDRESS 0x20

// MFRC522 instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

// User database structure
struct User {
    const char* uid;
    const char* name;
};

// User database in memory (edit this to add users)
const User users[] = {
    {"DEADBEEF", "Admin Mast"},
    {"CAFEBABE", "Super Admin"},
    {"AABBCCDD", "Juan Perez"},
    {"11223344", "Maria Lopez"},
    {"22334455", "Carlos Ruiz"},
    {"33445566", "Ana Garcia"}
};

const int userCount = sizeof(users) / sizeof(users[0]);

// Access response structure
struct AccessResponse {
    uint8_t granted;
    char name[16];
    char uid[9];
};

AccessResponse lastAccess;
unsigned long lastCardTime = 0;
String lastCardUID = "";

void setup() {
    Serial.begin(9600);
    delay(1000);
    
    Serial.println(F("\n╔════════════════════════════════════════╗"));
    Serial.println(F("║  ARDUINO RFID SIMPLE v1.0              ║"));
    Serial.println(F("╚════════════════════════════════════════╝\n"));
    
    // Initialize I2C
    Wire.begin();
    Serial.println(F("[I2C] Initialized as master"));
    
    // Initialize SPI
    SPI.begin();
    Serial.println(F("[SPI] Initialized"));
    
    // Initialize RFID
    Serial.print(F("[RFID] Initializing RC522... "));
    mfrc522.PCD_Init();
    delay(100);
    
    // Check RC522
    byte version = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
    if (version == 0x00 || version == 0xFF) {
        Serial.println(F("FAILED!"));
        Serial.println(F("[ERROR] RC522 not detected"));
        Serial.println(F("        Check connections and power"));
        while(1) {
            delay(1000);
        }
    }
    
    Serial.print(F("OK (v0x"));
    Serial.print(version, HEX);
    Serial.println(F(")"));
    
    // Turn on antenna
    mfrc522.PCD_AntennaOn();
    delay(50);
    
    // List users
    Serial.println(F("\n--- Registered Users ---"));
    for (int i = 0; i < userCount; i++) {
        Serial.print(i + 1);
        Serial.print(F(". "));
        Serial.print(users[i].uid);
        Serial.print(F(" -> "));
        Serial.println(users[i].name);
    }
    Serial.print(F("Total: "));
    Serial.print(userCount);
    Serial.println(F(" users\n"));
    
    // Send ready signal to PIC
    sendStatusToPIC(0xFF);
    
    Serial.println(F("═══════════════════════════════════════"));
    Serial.println(F("System Ready! Present card to reader..."));
    Serial.println(F("═══════════════════════════════════════\n"));
}

void loop() {
    // Check for new card
    if (!mfrc522.PICC_IsNewCardPresent()) {
        delay(50);
        return;
    }
    
    // Read card serial
    if (!mfrc522.PICC_ReadCardSerial()) {
        delay(50);
        return;
    }
    
    // Get UID as string
    String uidString = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
        uidString += String(mfrc522.uid.uidByte[i], HEX);
    }
    uidString.toUpperCase();
    
    // Prevent reading same card multiple times
    if (uidString == lastCardUID && (millis() - lastCardTime) < 3000) {
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return;
    }
    
    lastCardUID = uidString;
    lastCardTime = millis();
    
    // Card detected
    Serial.println(F("\n┌─────────────────────────────┐"));
    Serial.println(F("│     CARD DETECTED           │"));
    Serial.println(F("└─────────────────────────────┘"));
    Serial.print(F("UID: "));
    Serial.println(uidString);
    
    // Look up user
    bool accessGranted = false;
    String userName = "";
    
    for (int i = 0; i < userCount; i++) {
        if (uidString.equalsIgnoreCase(users[i].uid)) {
            accessGranted = true;
            userName = users[i].name;
            break;
        }
    }
    
    // Prepare response
    memset(&lastAccess, 0, sizeof(lastAccess));
    lastAccess.granted = accessGranted ? 1 : 0;
    uidString.toCharArray(lastAccess.uid, 9);
    userName.toCharArray(lastAccess.name, 16);
    
    // Print result
    if (accessGranted) {
        Serial.println(F("┌─────────────────────────────┐"));
        Serial.println(F("│  ✓ ACCESS GRANTED           │"));
        Serial.println(F("└─────────────────────────────┘"));
        Serial.print(F("User: "));
        Serial.println(userName);
    } else {
        Serial.println(F("┌─────────────────────────────┐"));
        Serial.println(F("│  ✗ ACCESS DENIED            │"));
        Serial.println(F("└─────────────────────────────┘"));
        Serial.println(F("Reason: Unknown user"));
        Serial.println(F("\nTo add this user, edit the code:"));
        Serial.print(F("  {\""));
        Serial.print(uidString);
        Serial.println(F("\", \"Your Name\"},"));
    }
    
    // Send to PIC
    Serial.print(F("[I2C] Sending to PIC... "));
    sendAccessDataToPIC();
    
    // Halt card
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    
    Serial.println();
}

// ========== I2C FUNCTIONS ==========

void sendStatusToPIC(uint8_t status) {
    Wire.beginTransmission(PIC_I2C_ADDRESS);
    Wire.write(0xF0);
    Wire.write(status);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
        Serial.println(F("[I2C] Status sent successfully"));
    } else {
        Serial.print(F("[I2C] Status send failed (error "));
        Serial.print(error);
        Serial.println(F(")"));
    }
}

void sendAccessDataToPIC() {
    Wire.beginTransmission(PIC_I2C_ADDRESS);
    
    // Command
    Wire.write(0xAA);
    
    // Access granted flag
    Wire.write(lastAccess.granted);
    
    // UID (8 bytes)
    for (int i = 0; i < 8; i++) {
        Wire.write(lastAccess.uid[i]);
    }
    
    // Name (16 bytes)
    for (int i = 0; i < 16; i++) {
        Wire.write(lastAccess.name[i]);
    }
    
    byte error = Wire.endTransmission();
    
    if (error == 0) {
        Serial.println(F("OK"));
    } else {
        Serial.print(F("FAIL (error "));
        Serial.print(error);
        Serial.println(F(")"));
    }
}