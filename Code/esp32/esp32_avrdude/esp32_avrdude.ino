#include "stk500.h"
#define RXD2 16
#define TXD2 17

void showHEX(const byte B){
  if(B<16){
    Serial.print(F("0"));
  }
  Serial.print(B,HEX);
}

void ResetExtDevice() {
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  delayMicroseconds(200);
  digitalWrite(21, HIGH);
  pinMode(21, INPUT);
}

void avrdudeVerbose(const char* msg){
  Serial.print("avrdude: ");
  Serial.println(msg);
}
void avrdudeRW(const char* type){
  Serial.println(F(""));Serial.print(F(type));Serial.print(F(" | "));
}
void avrdudeDone(){
  Serial.println(F("\navrdude done. Thank you."));
}

uint8_t prog[] = {
  0x0C, 0x94, 0x34, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00,
  0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00,
  0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00,
  0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00,
  0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00,
  0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00,
  0x0C, 0x94, 0x3E, 0x00, 0x0C, 0x94, 0x3E, 0x00, 0x11, 0x24, 0x1F, 0xBE, 0xCF, 0xEF, 0xD8, 0xE0,
  0xDE, 0xBF, 0xCD, 0xBF, 0x0E, 0x94, 0x40, 0x00, 0x0C, 0x94, 0x58, 0x00, 0x0C, 0x94, 0x00, 0x00,
  0x8F, 0xEF, 0x84, 0xB9, 0x15, 0xB8, 0x85, 0xB9, 0x2F, 0xEF, 0x33, 0xED, 0x90, 0xE3, 0x21, 0x50,
  0x30, 0x40, 0x90, 0x40, 0xE1, 0xF7, 0x00, 0xC0, 0x00, 0x00, 0x15, 0xB8, 0x2F, 0xEF, 0x33, 0xED,
  0x90, 0xE3, 0x21, 0x50, 0x30, 0x40, 0x90, 0x40, 0xE1, 0xF7, 0x00, 0xC0, 0x00, 0x00, 0xEB, 0xCF,
  0xF8, 0x94, 0xFF, 0xCF
};

void exitPgmMode(){
  Serial2.flush();
  while(1){
    Serial2.write((byte)STK_LEAVE_PROGMODE);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if(res1 == STK_INSYNC && res2 == STK_OK){
      
    }
    else{
      avrdudeVerbose("Error: STK_LEAVE_PROGMODE");
    }
    break;
  }
}

void verifyFlash(){
  avrdudeVerbose("Verifying flash memory against input hex file");
  avrdudeVerbose("Load flash data from input file .hex");
  avrdudeVerbose("Input file .hex 180 bytes");
  avrdudeVerbose("Reading on-chip flash data: ");
  avrdudeRW("Reading");
  bool done = false;
  int error = 0;
  uint16_t pageSize = 0x0080;
  uint16_t address = 0x0000;
  Serial.print(F("####"));
  int pageCount = 0;
  Serial2.flush();
  while(1){
    //Load Address
    byte addrH = (address >> 8) && 0xFF;
    byte addrL = address & 0xFF;
    Serial2.write((byte)STK_LOAD_ADDRESS);
    Serial2.write((byte)addrL);
    Serial2.write((byte)addrH);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if(res1 ==  STK_INSYNC && res2 == STK_OK){
      Serial.print(F("######"));
      Serial2.flush();
      while(1){
        // Reading pgm data
        byte pageSizeH = (pageSize >> 8) && 0xFF;
        byte pageSizeL = pageSize & 0xFF;
        Serial2.write((byte)STK_READ_PAGE);
        Serial2.write((byte)pageSizeH);
        Serial2.write((byte)pageSizeL);
        Serial2.write((byte)'F');
        Serial2.write((byte)CRC_EOP);
        while(!Serial2.available());
        byte res1 = Serial2.read();
        if(res1 ==  STK_INSYNC){
          uint16_t curr = 0;
          while(curr < pageSize){
            while(!Serial2.available());
            byte data = Serial2.read();
            if( data != prog[pageCount*pageSize + curr]){
              error++;
            }
            curr++;
          }
          while(!Serial2.available());
          byte res2 = Serial2.read();
          if(res2 ==  STK_OK){
            pageCount++;
            address += pageSize/2;
          }
          else{
            Serial.println(res2,HEX);
            avrdudeVerbose("Error: STK_READ_PAGE.");
          }
        }
        else{
          Serial.println(res1,HEX);
          avrdudeVerbose("Error: STK_READ_PAGE.");
        }
        break;
      }
    }
    else{
      avrdudeVerbose("Error: STK_LOAD_ADDR");
      break;
    }
    if(pageCount>=2){
      Serial.print(F("###### | 100%\n"));
      Serial.println("");
      avrdudeVerbose("Verifying.....");
      Serial.print("avrdude: 180 bytes of flash verified. (");
      float perc = (error/180)*100;
      Serial.print(perc);
      Serial.println("% error) ");
      break;
    }
  }
}

void writeFlash(){
  avrdudeVerbose("Reading hex file Blink.hex");
  avrdudeVerbose("Writing flash (180 bytes): ");
  avrdudeRW("Writing");
  bool done = false;
  uint16_t pageSize = 0x0080;
  uint16_t address = 0x0000;
  Serial.print(F("####"));
  int pageCount = 0;
  Serial2.flush();
  while(1){
    //Load Address
    byte addrH = (address >> 8) && 0xFF;
    byte addrL = address & 0xFF;
    Serial2.write((byte)STK_LOAD_ADDRESS);
    Serial2.write((byte)addrL);
    Serial2.write((byte)addrH);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if(res1 ==  STK_INSYNC && res2 == STK_OK){
      Serial.print(F("######"));
      Serial2.flush();
      while(1){
        // Sending pgm data
        byte pageSizeH = (pageSize >> 8) && 0xFF;
        byte pageSizeL = pageSize & 0xFF;
        Serial2.write((byte)STK_PROG_PAGE);
        Serial2.write((byte)pageSizeH);
        Serial2.write((byte)pageSizeL);
        Serial2.write((byte)'F');
        for(uint16_t i = 0; i<pageSize;i++){
          Serial2.write((byte)prog[pageCount*pageSize + i]);
        }
        Serial2.write((byte)CRC_EOP);
        while(!Serial2.available());
        byte res1 = Serial2.read();
        while(!Serial2.available());
        byte res2 = Serial2.read();
        if(res1 ==  STK_INSYNC && res2 == STK_OK){
          pageCount++;
          address += pageSize/2;
        }
        else{
          Serial.println(res1,HEX);
          Serial.println(res2,HEX);
          avrdudeVerbose("Error: STK_PROGRAM_PAGE.");
        }
        break;
      }
    }
    else{
      avrdudeVerbose("Error: STK_LOAD_ADDR");
      break;
    }
    if(pageCount>=2){
      Serial.print(F("###### | 100%\n"));
      Serial.println("");
      avrdudeVerbose("180 bytes of flash written.");
      verifyFlash();
      break;
    }
  }
}

void readDevice(){
  avrdudeRW("Reading");
  Serial.print(F("###"));
  Serial2.flush();
  while(1){
    //Reading SOFTWARE_MAJOR_VER
    Serial2.write((byte)STK_GET_PARAMETER);
    Serial2.write((byte)STK_SW_MAJOR);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    byte res3 = Serial2.read();
    if(res1 ==  STK_INSYNC && res3 == STK_OK){
      Serial.print(F("######"));
      Serial2.flush();
      while(1){
        //Reading SOFTWARE_MINOR_VER
        Serial2.write((byte)STK_GET_PARAMETER);
        Serial2.write((byte)STK_SW_MINOR);
        Serial2.write((byte)CRC_EOP);
        while(!Serial2.available());
        byte res1 = Serial2.read();
        byte res2 = Serial2.read();
        byte res3 = Serial2.read();
        if(res1 ==  STK_INSYNC && res3 == STK_OK){
          Serial.print(F("#####"));
          Serial2.flush();
          while(1){
            //Entering PROG_MODE
            Serial2.write((byte)STK_ENTER_PROGMODE);
            Serial2.write((byte)CRC_EOP);
            while(!Serial2.available());
            byte res1 = Serial2.read();
            byte res2 = Serial2.read();
            if(res1 ==  STK_INSYNC && res3 == STK_OK){
              Serial.print(F("####"));
              Serial2.flush();
              while(1){
                //Reading Signature
                Serial2.write((byte)STK_READ_SIGN);
                Serial2.write((byte)CRC_EOP);
                while(!Serial2.available());
                byte res1 = Serial2.read();byte res2 = Serial2.read();
                byte res3 = Serial2.read();byte res4 = Serial2.read();
                byte res5 = Serial2.read();
                if(res1 ==  STK_INSYNC && res5 == STK_OK){
                  Serial.print(F("##### | 100%\n\n"));
                  Serial.print(F("avrdude: Device Signature 0x"));
                  Serial.print(res2,HEX);Serial.print(res3,HEX);Serial.println(res4,HEX);
                  writeFlash();
                  exitPgmMode();
                }
                else{
                  avrdudeVerbose("Error: Can't read signature.");
                }
                break;
              }
            }
            else{
              avrdudeVerbose("Error: Can't enter PROG_MODE");
            }
            break;
          }
        }
        else{
          avrdudeVerbose("Error: Can't get the SOFTWARE_MINOR_VER");
        }
        break;
      }
    }
    else{
        avrdudeVerbose("Error: Can't get the SOFTWARE_MAJOR_VER");
    }
    break;
  }
}

void syncAVR(){
  Serial2.flush();
  unsigned long startTime = millis();
  while(1){
    Serial2.write((byte)STK_GET_SYNC);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available()){
      unsigned long currTime = millis();
      if(currTime-startTime >= 600){
         Serial2.flush();
         Serial2.write((byte)STK_GET_SYNC);
         Serial2.write((byte)CRC_EOP);
         startTime = millis();
      }
    }
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if( res1 == STK_INSYNC && res2 == STK_OK){
      //Confirming second time
      Serial2.flush();
      startTime = millis();
      Serial2.write((byte)STK_GET_SYNC);
      Serial2.write((byte)CRC_EOP);
      while(!Serial2.available());
      byte res1 = Serial2.read();
      byte res2 = Serial2.read();
      if( res1 == STK_INSYNC && res2 == STK_OK){
        //Confirming one last Time
        Serial2.flush();
        startTime = millis();
        Serial2.write((byte)STK_GET_SYNC);
        Serial2.write((byte)CRC_EOP);
        while(!Serial2.available());
        byte res1 = Serial2.read();
        byte res2 = Serial2.read();
        if( res1 == STK_INSYNC && res2 == STK_OK){
          //Confirmed 3x. avrdude and device in sync
          avrdudeVerbose("AVR Device initiallised and ready to send instructions.");
          readDevice();
        }
        else{
          avrdudeVerbose("Error: AVR Device not in sync().");
        }
      }
      else{
        avrdudeVerbose("Error: AVR Device not in sync().");
      }
      break;
    }
  }
}
void avrdude(){
  Serial.println(F("\navrdude: Version 0.1.0"));
  Serial.println(F("         Copyright (c) 2020 Tronix Division"));
  Serial.println(F("         Copyright (c) 2020 Spider R&D Club"));
  ResetExtDevice();
  //Synchronise with AVR Device
  syncAVR();
  avrdudeDone();
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  avrdude();
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  
}
