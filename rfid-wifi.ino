#include <ESP8266WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

const char* ssid = "";
const char* password = "";

#define RSTPIN A0 // fake pin for wemos d1r2
#define SSPIN 16 // D0 on wemos d1r2 maps to D2 on uno
MFRC522 rc(SSPIN, RSTPIN);

void setup() {
  delay(1000);
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());
  
  SPI.begin();
  rc.PCD_Init();
  rc.PCD_DumpVersionToSerial();

  Serial.println("Scan RFID for details");
}


void loop() {
  bool found = scanRfid();
}

bool scanRfid(){  
  if(!rc.PICC_IsNewCardPresent()){
    return false;
  }
  if(!rc.PICC_ReadCardSerial()){
    return false;
  }
 
  Serial.print("Found RFID: ");
  for(int i = 0; i < 4; i++){
    Serial.print(rc.uid.uidByte[i], HEX);
  }
  Serial.println();
  rc.PICC_HaltA();
  
  return true;
}
