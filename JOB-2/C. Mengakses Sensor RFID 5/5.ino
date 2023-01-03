#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 21 // ESP32 pin GIO21
#define RST_PIN 22 // ESP32 pin GIO22
const int ledPin = 2; 
const int ledPin2 = 4;
//int LedPin = 0;
//int LedPin2 = 0;

MFRC522 rfid(SS_PIN, RST_PIN);
byte keyTagUID[4] = {0xE3, 0x1A, 0xD6, 0x03};
void setup() {
 Serial.begin(9600);
 SPI.begin(); // init SPI bus
 rfid.PCD_Init(); // init MFRC522
 Serial.println("Tap RFID/NFC Tag on reader");
 pinMode(ledPin, OUTPUT);
 pinMode(ledPin2, OUTPUT);
}
void loop() {
 if (rfid.PICC_IsNewCardPresent()) { // new tag is available
 if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
 MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
 if (rfid.uid.uidByte[0] == keyTagUID[0] &&
 rfid.uid.uidByte[1] == keyTagUID[1] &&
 rfid.uid.uidByte[2] == keyTagUID[2] &&
 rfid.uid.uidByte[3] == keyTagUID[3] ) {
 Serial.println("Access is granted");
 digitalWrite(ledPin, HIGH);  // unlock the door for 2 seconds
 delay(2000);
 digitalWrite(ledPin, LOW);  // unlock the door for 2 seconds
 }
 else
 {
 Serial.print("Access denied for user with UID:");
 for (int i = 0; i < rfid.uid.size; i++) {
 Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
 Serial.print(rfid.uid.uidByte[i], HEX);
 digitalWrite(ledPin2, HIGH); // lock the door
 delay(100);
 digitalWrite(ledPin2, LOW); // lock the door
 }
 Serial.println();
 }
 rfid.PICC_HaltA(); // halt PICC
 rfid.PCD_StopCrypto1(); // stop encryption on PCD
 }
 }
}
