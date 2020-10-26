#include "spideydude.h"

void Spideydude::ResetExtDevice() {
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  delayMicroseconds(200);
  digitalWrite(21, HIGH);
  pinMode(21, INPUT);
}

void Spideydude::Verbose(const char* msg){
  Serial.print("spideydude: ");
  Serial.println(msg);
  response += "spideydude: " + String(msg) + "\n";
}

void Spideydude::RW(const char* type){
  Serial.println(F(""));Serial.print(F(type));Serial.print(F(" | "));
  response += "\n"+ String(type) +" | ";
}

void Spideydude::Done(){
  Serial.println(F("\nspideydude done. Thank you."));
  response += "\nspideydude done. Thank you.";
}

void Spideydude::sync(){
  Serial2.flush();
  unsigned long startTime = millis();
  while(1){
    Serial2.write((byte)spidey_start_tx);
    Serial2.write((byte)spidey_node_ack);
    while(!Serial2.available()){
      unsigned long currTime = millis();
      if(currTime-startTime >= 500){
         Serial2.flush();
         Serial2.write((byte)spidey_start_tx);
         Serial2.write((byte)spidey_node_ack);
         startTime = millis();
      }
    }
    byte res1 = Serial2.read();
    if(res1 == spidey_acknowledge){
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

void Spideydude::writeFlash(){
  Serial.printf("spideydude: Reading hex file %s\n",details.fileName.c_str());
  response += "spideydude: Reading hex file " + details.fileName +"\n";
  Serial.printf("spideydude: Writing flash (%d bytes):\n",details.len);
  response += "spideydude: Writing flash (" + String(details.len) +" bytes):\n";
  unsigned long startTime = millis();
  RW("Writing");
  uint16_t pageSize = details.pageSize;
  uint16_t address = 0x0000;
  Serial.print(F("####"));
  response += "####";
  int pageCount = 0;
  Serial2.flush();
  while(1){
    //Load Address
    byte addrH = (address >> 8) && 0xFF;
    byte addrL = address & 0xFF;
    Serial2.write((byte)spidey_load_address);
    Serial2.write((byte)spidey_node_ack);
    Serial2.write((byte)addrL);
    Serial2.write((byte)addrH);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    if(res1 == spidey_acknowledge){
      Serial.print(F("######"));
      response += "######";
      Serial2.flush();
      while(1){
        // Sending pgm data
        byte pageSizeH = (pageSize >> 8) && 0xFF;
        byte pageSizeL = pageSize & 0xFF;
        Serial2.write((byte)spidey_start_progmode);
        Serial2.write((byte)spidey_node_ack);
        Serial2.write((byte)pageSizeL);
        Serial2.write((byte)pageSizeH);
        for(uint16_t i = 0; i<pageSize;i++){
          Serial2.write((byte)details.progData[pageCount*pageSize + i]);
        }
        while(!Serial2.available());
        byte res1 = Serial2.read();
        while(!Serial2.available());
        byte res2 = Serial2.read();
        if(res1 ==  spidey_data_recieved && res2 == spidey_acknowledge){
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
      response += "###### | 100% (";
      float elapsedTime = (millis() - startTime)/1000;
      Serial.print(elapsedTime, 2);
      response += String(elapsedTime);
      Serial.println(F("s)\n"));
      response += "s)\n\n";
      Serial.printf("spideydude: %d bytes of flash written.\n",details.len);
      response += "spideydude: "+ String(details.len) +" bytes of flash written.\n";
      verifyFlash();
      break;
    }
  }
}

void Spideydude::verifyFlash(){
  Verbose("Verifying flash memory against input hex file");
  Serial.printf("spideydude: Loading flash data from input file %s\n",details.fileName.c_str());
  response += "spideydude: Loading flash data from input file " + details.fileName +"\n";
  Serial.printf("spideydude: Input file has %d bytes of Flash\n",details.len);
  response += "spideydude: Input file has " + String(details.len) +" bytes of Flash\n";
  Verbose("Reading on-chip flash data: ");
  unsigned long startTime = millis();
  RW("Reading");
  int error = 0;
  uint16_t pageSize = details.pageSize;
  uint16_t address = 0x0000;
  Serial.print(F("####"));
  response += "####";
  int pageCount = 0;
  Serial2.flush();
  while(1){
    //Load Address
     byte addrH = (address >> 8) && 0xFF;
    byte addrL = address & 0xFF;
    Serial2.write((byte)spidey_load_address);
    Serial2.write((byte)spidey_node_ack);
    Serial2.write((byte)addrL);
    Serial2.write((byte)addrH);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    if(res1 == spidey_acknowledge){
      Serial.print(F("######"));
      response += "######";
      Serial2.flush();
      while(1){
        // Reading pgm data
        byte pageSizeH = (pageSize >> 8) && 0xFF;
        byte pageSizeL = pageSize & 0xFF;
        Serial2.write((byte)spidey_check_flash);
        Serial2.write((byte)spidey_node_ack);
        Serial2.write((byte)pageSizeL);
        Serial2.write((byte)pageSizeH);
        uint16_t curr = 0;
        while(curr < pageSize){
          while(!Serial2.available());
          byte data = Serial2.read();
          Serial.println(data,HEX);
          if( data != details.progData[pageCount*pageSize + curr]){
            error++;
          }
          curr++;
        }
        while(!Serial2.available());
        byte res1 = Serial2.read();
        if(res1 ==  spidey_acknowledge){
          pageCount++;
          address += pageSize/2;
        }
        else{
          //Serial.println(res2,HEX);
          Verbose("Error: SPIDEY_READ_PAGE. Can't read the PAGE");
        }
        break;
      }
    }
    else{
      Verbose("Error: SPIDEY_LOAD_ADDR. Can't load the address to read from");
      break;
    }
    if(pageCount>=2){
      Serial.print(F("###### | 100% ("));
      response += "###### | 100% (";
      float elapsedTime = (millis() - startTime)/1000;
      Serial.print(elapsedTime, 2);
      response += String(elapsedTime);
      Serial.println(F("s)\n"));
      response += "s)\n\n";
      Verbose("Verifying.....");
      Serial.printf("spideydude: %d bytes of flash verified. (",details.len);
      response += "spideydude: "+ String(details.len) + " bytes of flash verified. (";
      float perc = (error/180)*100;
      Serial.print(perc);
      Serial.println("% error) ");
      response += String(perc) + "% error) \n";
      endProg();
      break;
    }
  }
}

void Spideydude::endProg(){
  Serial2.flush();
  Serial2.write((byte)spidey_end_tx);
  Serial2.write((byte)spidey_node_ack);
  while(!Serial2.available()){}
  byte res1 = Serial2.read();
  if(res1 == spidey_acknowledge){
      Verbose("Exited programming mode. Process complete");
  }
  else{
     Verbose(String(String("Error in exiting PROGRAMMING mode expected=0x06 but got=0x")+String(res1)).c_str());
  }
}

String Spideydude::begin(String file, uint8_t *pg, size_t len, uint16_t pageLen){
  response = "";
  details.fileName = file;
  details.progData = pg;
  details.len = len;
  details.pageSize = pageLen;
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  String stmnt = "\nspideydude: Version "+String(_MAJOR_VERSION_)+"."+String(_MINOR_VERSION_)+"."+String(_SUB_MINOR_VERSION_);
  Serial.println(F(stmnt.c_str()));
  Serial.println(F("            Copyright (c) 2020 Tronix Division"));
  Serial.println(F("            Copyright (c) 2020 Spider R&D Club, NIT Trichy"));
  response += stmnt + "\n            Copyright (c) 2020 Tronix Division\n" + "            Copyright (c) 2020 Spider R&D Club, NIT Trichy\n";
  ResetExtDevice();
  sync();
  Done();
  return response;
}
