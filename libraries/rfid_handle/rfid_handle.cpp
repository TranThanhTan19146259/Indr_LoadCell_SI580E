#include "rfid_handle.h"
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class


void init_rfid()
{
    SPI.begin(); // Init SPI bus
    rfid.PCD_Init(); // Init MFRC522 
}

void handle_rfid()
{
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

//   Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
//   Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    // Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  for (byte i = 0; i < 4; i++) {
    myRam.rfid.nuidPICC[i] = rfid.uid.uidByte[i];
}
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
