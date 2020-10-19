#include "spideyboot.h"

void SpideyBoot::ResetExtDevice() {
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  delayMicroseconds(200);
  digitalWrite(21, HIGH);
  pinMode(21, INPUT);
}

void SpideyBoot::Verbose(const char* msg){
  Serial.print("spideyboot: ");
  Serial.println(msg);
}

void SpideyBoot::RW(const char* type){
  Serial.println(F(""));Serial.print(F(type));Serial.print(F(" | "));
}

void SpideyBoot::Done(){
  Serial.println(F("\nspideyboot done. Thank you."));
}

void SpideyBoot::sync(){
  Serial2.flush();
  unsigned long startTime = millis();
  while(1){
    Serial2.write((byte)0x01);
    Serial2.write((byte)0x09);
    while(!Serial2.available()){
      unsigned long currTime = millis();
      if(currTime-startTime >= 500){
         Serial2.flush();
         Serial2.write((byte)0x01);
         Serial2.write((byte)0x09);
         startTime = millis();
      }
    }
    byte res1 = Serial2.read();
    if(res1 == 0x06){
      Verbose("AVR device initialized and ready to send instructions.");
      writeFlash();
    }
    else{
      String error = "Failed to sync() with the device, expected=0x06 but got=0x"+String(res1); 
      Verbose(error.c_str());
    }
    break;
  }
}

void SpideyBoot::writeFlash(){
  Serial.printf("spideyboot: Reading hex file %s\n",details.fileName.c_str());
  Serial.printf("spideyboot: Writing flash (%d bytes):\n",details.len);
  unsigned long startTime = millis();
  RW("Writing");
  uint16_t pageSize = details.pageSize;
  uint16_t address = 0x0000;
  Serial.print(F("####"));
  int pageCount = 0;
  Serial2.flush();
  while(1){
    //Load Address
    byte addrH = (address >> 8) && 0xFF;
    byte addrL = address & 0xFF;
    Serial2.write((byte)0x05);
    Serial2.write((byte)0x09);
    Serial2.write((byte)addrL);
    Serial2.write((byte)addrH);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    if(res1 == 0x06){
      Serial.print(F("######"));
      Serial2.flush();
      while(1){
        // Sending pgm data
        byte pageSizeH = (pageSize >> 8) && 0xFF;
        byte pageSizeL = pageSize & 0xFF;
        Serial2.write((byte)0x02);
        Serial2.write((byte)0x09);
        Serial2.write((byte)pageSizeL);
        Serial2.write((byte)pageSizeH);
        for(uint16_t i = 0; i<pageSize;i++){
          Serial2.write((byte)details.progData[pageCount*pageSize + i]);
        }
        while(!Serial2.available());
        byte res1 = Serial2.read();
        while(!Serial2.available());
        byte res2 = Serial2.read();
        if(res1 ==  0x03 && res2 == 0x06){
          pageCount++;
          address += pageSize;
        }
        else{
          Serial.println(res1,HEX);
          Serial.println(res2,HEX);
          Verbose("Error: Can't program the page.");
        }
        break;
      }
    }
    else{
      Verbose("Error: Can't load address");
      break;
    }
    if(pageCount>=2){
      Serial.print(F("###### | 100% ("));
      float elapsedTime = (millis() - startTime)/1000;
      Serial.print(elapsedTime, 2);
      Serial.print(F("s)"));
      Serial.println(F("\n"));
      Serial.printf("spideyboot: %d bytes of flash written.\n",details.len);
      Serial2.flush();
      Serial2.write((byte)0x04);
      Serial2.write((byte)0x09);
      while(!Serial2.available()){}
      byte res1 = Serial2.read();
      if(res1 == 0x06){
          Verbose("Exited programming mode. Process complete");
      }
      else{
         Verbose(String(String("Error in exiting PROGRAMMING mode expected=0x06 but got=0x")+String(res1)).c_str());
      }
      break;
    }
  }
}

void SpideyBoot::begin(String file, uint8_t *pg, size_t len, uint16_t pageLen){
  details.fileName = file;
  details.progData = pg;
  details.len = len;
  details.pageSize = pageLen;
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  String stmnt = "\nspideyboot: Version "+String(_MAJOR_VERSION_)+"."+String(_MINOR_VERSION_)+"."+String(_SUB_MINOR_VERSION_);
  Serial.println(F(stmnt.c_str()));
  Serial.println(F("            Copyright (c) 2020 Tronix Division"));
  Serial.println(F("            Copyright (c) 2020 Spider R&D Club"));
  ResetExtDevice();
  sync();
  Done();
}
