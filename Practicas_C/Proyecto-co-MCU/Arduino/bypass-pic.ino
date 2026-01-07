#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27, 16, 2);

struct User {
    const char* uid;
    const char* name;
};

const User users[] = {
    {"46011505", "Yael RDF"},
    {"CAFEBABE", "Super Admin"},
    {"AABBCCDD", "Juan Perez"},
    {"11223344", "Maria Lopez"},
    {"22334455", "Carlos Ruiz"},
    {"33445566", "Ana Garcia"}
};

const int userCount = sizeof(users) / sizeof(users[0]);

String lastCardUID = "";
unsigned long lastCardTime = 0;
unsigned long messageTime = 0;
bool showingMessage = false;

void setup() {
    Serial.begin(9600);
    delay(1000);
    
    Wire.begin();
    SPI.begin();
    
    lcd.init();
    lcd.backlight();
    
    mfrc522.PCD_Init();
    delay(100);
    
    mfrc522.PCD_AntennaOn();
    delay(50);
    
    showIdleScreen();
    
    Serial.println("System Ready! Present card to reader...");
}

void loop() {
    if (showingMessage && (millis() - messageTime) > 2000) {
        showIdleScreen();
        showingMessage = false;
    }
    
    if (!mfrc522.PICC_IsNewCardPresent()) {
        delay(50);
        return;
    }
    
    if (!mfrc522.PICC_ReadCardSerial()) {
        delay(50);
        return;
    }
    
    String uidString = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
        uidString += String(mfrc522.uid.uidByte[i], HEX);
    }
    uidString.toUpperCase();
    
    if (uidString == lastCardUID && (millis() - lastCardTime) < 3000) {
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        return;
    }
    
    lastCardUID = uidString;
    lastCardTime = millis();
    
    bool accessGranted = false;
    String userName = "";
    
    for (int i = 0; i < userCount; i++) {
        if (uidString.equalsIgnoreCase(users[i].uid)) {
            accessGranted = true;
            userName = users[i].name;
            break;
        }
    }
    
    if (accessGranted) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Bienvenido");
        lcd.setCursor(0, 1);
        lcd.print(userName);
        
        Serial.print("Access granted for: ");
        Serial.println(userName);
    } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Acceso");
        lcd.setCursor(0, 1);
        lcd.print("Denegado");
        
        Serial.print("Unknown card: ");
        Serial.println(uidString);
    }
    
    showingMessage = true;
    messageTime = millis();
    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
}

void showIdleScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Presente");
    lcd.setCursor(0, 1);
    lcd.print("Tarjeta");
}