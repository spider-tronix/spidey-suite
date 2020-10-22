#include "avrdude.h"

// ========================  Utility function's definition starts =================================
/*
 * Serial omits HEX numbers starting from 0. Ex: 0A will be 
 * printed as A. Hence, to avoid confusion, this function 
 * prints the HEX in its actual 8 bytes format.
*/
void showHEX(const byte B){
  if(B<16){
    Serial.print(F("0"));
  }
  Serial.print(B,HEX);
}
// ========================  Utility function's definition ends =================================

// ========================  Avrdude class member function definition starts =================================
// ==============================  Helper function's definition starts =================================
/*
 * Send active low RESET signal on PIN 21 of the ESP32.
 * Pulse is held low for 200ms
*/
void Avrdude::ResetExtDevice() {
  pinMode(21, OUTPUT);
  digitalWrite(21, LOW);
  delayMicroseconds(200);
  digitalWrite(21, HIGH);
  pinMode(21, INPUT);
}

/*
 * Prits the verbose output on the monitor/console. 
 * Format is same as the actual avrdude
*/
void Avrdude::Verbose(const char* msg){
  Serial.print("avrdude: ");
  Serial.println(msg);
  response += "avrdude: " + String(msg) + "\n";
}

/*
 * Prints "Reading |" or "Writing |" without newline 
 * Replacemnt function for avrdudeVerbose during Reading and Writing
*/
void Avrdude::RW(const char* type){
  Serial.println(F(""));Serial.print(F(type));Serial.print(F(" | "));
  response += "\n"+ String(type) +" | ";
}

/*
 * Print the final statement in avrdude operation.
*/
void Avrdude::Done(){
  Serial.println(F("\navrdude done. Thank you."));
  response += "\navrdude done. Thank you.";
}
// ==============================  Helper function's definition ends =================================
// ==============================  Essential function's definition ends =================================
/*
 * Function to initialise avrdude process. Initallise Serial comm on PIN RX0, TX0
 * for debugging and verbose output. Serial2 i.e, RX2 and TX2 communicates with the UART 
 * for the connected uC. Appropriate BAUDRATE is selected basd on the uC. 
*/
String Avrdude::begin(String file, uint8_t *pg, size_t len, uint16_t pageLen){
  response = "";
  details.fileName = file;
  details.progData = pg;
  details.len = len;
  details.pageSize = pageLen;
  // Initialising Serial comm. 
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  // Printing basic info first
  String stmnt = "\navrdude: Version "+String(_MAJOR_VERSION_)+"."+String(_MINOR_VERSION_)+"."+String(_SUB_MINOR_VERSION_);
  Serial.println(F(stmnt.c_str()));
  Serial.println(F("         Copyright (c) 2020 Tronix Division"));
  Serial.println(F("         Copyright (c) 2020 Spider R&D Club, NIT Trichy"));
  response += stmnt + "\n         Copyright (c) 2020 Tronix Division\n" + "         Copyright (c) 2020 Spider R&D Club, NIT Trichy\n";
  // Start the process by sending RESET signal first
  ResetExtDevice();
  // Synchronise with AVR Device. If synchronisation succeeds, it will automatically
  // call next function and continue the process. 
  syncAVR();
  // After everything is done print the final msg and terminate process. 
  Done();
  return response;
}

/*
 * Synchrise with the device first. Sends STK_GET_SYNC CRC_EOP 
 * and waits for response. If correct response is received, sends the
 * same things 2 more times two confirm the synchronisation with the device. 
*/
void Avrdude::syncAVR(){
  Serial2.flush();
  unsigned long startTime = millis();
  while(1){
    Serial2.write((byte)STK_GET_SYNC);Serial2.write((byte)CRC_EOP);
    while(!Serial2.available()){                               // Wait untill response is recieved
      unsigned long currTime = millis(); 
      if( (currTime-startTime) >= 600){                        // If the last time data was sent is greater than 600ms, resend the data. 
         Serial2.flush();
         Serial2.write((byte)STK_GET_SYNC);
         Serial2.write((byte)CRC_EOP);
         startTime = millis();
      }
    }   // Program will come here when it has received the data.
    // Read the incoming data 
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if( res1 == STK_INSYNC && res2 == STK_OK){                           // If response is correct 
      //Confirming the synchronisation for the second time, repeasts the same procedure
      Serial2.flush();
      Serial2.write((byte)STK_GET_SYNC);
      Serial2.write((byte)CRC_EOP);
      // Since already we recieved SYNC response, we can assume we need not give some time. The 
      // response this time will come instantly.
      while(!Serial2.available());                                      
      byte res1 = Serial2.read();
      byte res2 = Serial2.read();
      if( res1 == STK_INSYNC && res2 == STK_OK){
        //Confirming one last Time for the synchronisation 
        Serial2.flush();
        Serial2.write((byte)STK_GET_SYNC);
        Serial2.write((byte)CRC_EOP);
        while(!Serial2.available());
        byte res1 = Serial2.read();
        byte res2 = Serial2.read();
        if( res1 == STK_INSYNC && res2 == STK_OK){
          //Confirmed 3 times. avrdude and device in sync
          Verbose("AVR Device initiallised and ready to send instructions.");
          // Go on to read other details from the device 
          readDevice();
        }
        else{
          Verbose("Error: AVR Device not in sync(). No in_sync() for 3rd try");
        }
      }
      else{
        Verbose("Error: AVR Device not in sync().  No in_sync() for 2nd try");
      }
    }
    else{
      Verbose("Error: AVR Device not in sync().  No in_sync() for 1st try");
      response += String(res1) + "  " + String(res2) + "\n";
    }
    break;
  }
}

/*
 * Reads Bootloader version and then enters the programming mode.
 * Upon entering, reads the signature bytes from the deivce.
 * And then continues writing flash and finally exits the programming mode. 
*/
void Avrdude::readDevice(){
  unsigned long startTime = millis();
  RW("Reading");
  Serial.print(F("###"));
  response += "###";
  Serial2.flush();
  while(1){
    //Reading SOFTWARE_MAJOR_VER
    Serial2.write((byte)STK_GET_PARAMETER);
    Serial2.write((byte)STK_SW_MAJOR);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();                     // We will just read the version
    byte res3 = Serial2.read();
    if(res1 ==  STK_INSYNC && res3 == STK_OK){      // If response is correct 
      Serial.print(F("######"));
      response += "######";
      Serial2.flush();
      while(1){
        //Reading SOFTWARE_MINOR_VER
        Serial2.write((byte)STK_GET_PARAMETER);
        Serial2.write((byte)STK_SW_MINOR);
        Serial2.write((byte)CRC_EOP);
        while(!Serial2.available());
        byte res1 = Serial2.read();
        byte res2 = Serial2.read();                 // We will just read the version
        byte res3 = Serial2.read();
        if(res1 ==  STK_INSYNC && res3 == STK_OK){  // If the response is correct 
          Serial.print(F("#####"));
          response += "#####";
          Serial2.flush();
          while(1){
            //Sending command to enter the PROG_MODE
            Serial2.write((byte)STK_ENTER_PROGMODE);
            Serial2.write((byte)CRC_EOP);
            while(!Serial2.available());
            byte res1 = Serial2.read();
            byte res2 = Serial2.read();
            if(res1 ==  STK_INSYNC && res3 == STK_OK){       // If correcct response 
              Serial.print(F("####"));
              response += "####";
              Serial2.flush();
              while(1){
                //Reading Signature
                Serial2.write((byte)STK_READ_SIGN);
                Serial2.write((byte)CRC_EOP);
                while(!Serial2.available());
                byte res1 = Serial2.read();byte res2 = Serial2.read();
                byte res3 = Serial2.read();byte res4 = Serial2.read();
                byte res5 = Serial2.read();
                if(res1 ==  STK_INSYNC && res5 == STK_OK){      // If correct response 
                  Serial.print(F("##### | 100% ("));
                  response += "##### | 100% (";
                  float elapsedTime = (millis() - startTime)/1000;
                  Serial.print(elapsedTime, 2);
                  response += String(elapsedTime);
                  Serial.println(F("s)\n"));
                  response += "s)\n\n";
                  Serial.print(F("avrdude: Device Signature 0x"));
                  response += "avrdude: Device Signature 0x";
                  showHEX(res2);showHEX(res3);showHEX(res4);// Print the signature 
                  Serial.print(F("\n"));
                  response += String(res2) + String(res3) + String(res4) + "\n";
                  // Now proceed to write the Flash data 
                  writeFlash();
                  // Finally, exit the programming mode 
                  exitPgmMode();
                }
                else{
                  Verbose("Error: STK_READ_SIGN. Can't get the device signature. ");
                }
                break;
              }
            }
            else{
              Verbose("Error: STK_ENTER_PROGMODE. Can't enter PROG_MODE");
            }
            break;
          }
        }
        else{
          Verbose("Error: SOFTWARE_MINOR_VER. Can't get the software minor version.");
        }
        break;
      }
    }
    else{
       Verbose("Error: SOFTWARE_MAJOR_VER. Can't get the software major version");
    }
    break;
  }
}

/*
 * Write the flash in page by page fashion as per the stk500 v1 protocol.
*/
void Avrdude::writeFlash(){
  Serial.printf("avrdude: Reading hex file %s\n",details.fileName.c_str());
  response += "avrdude: Reading hex file " + details.fileName +"\n";
  Serial.printf("avrdude: Writing flash (%d bytes):\n",details.len);
  response += "avrdude: Writing flash (" + String(details.len) +" bytes):\n";
  unsigned long startTime = millis();
  RW("Writing");
  uint16_t pageSize = details.pageSize;
  uint16_t address = 0x0000;
  Serial.print(F("####"));
  response += "####";
  int pageCount = 0;
  Serial2.flush();
  while(1){
    //Load the Address on the device 
    byte addrH = (address >> 8) && 0xFF;
    byte addrL = address & 0xFF;
    Serial2.write((byte)STK_LOAD_ADDRESS);
    Serial2.write((byte)addrL);
    Serial2.write((byte)addrH);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if(res1 ==  STK_INSYNC && res2 == STK_OK){    // If load address was succesfull 
      Serial.print(F("######"));
      response += "######";
      Serial2.flush();
      while(1){
        // Sending programm data
        byte pageSizeH = (pageSize >> 8) && 0xFF;
        byte pageSizeL = pageSize & 0xFF;
        Serial2.write((byte)STK_PROG_PAGE);
        Serial2.write((byte)pageSizeH);
        Serial2.write((byte)pageSizeL);
        Serial2.write((byte)'F');
        for(uint16_t i = 0; i<pageSize;i++){
          Serial2.write((byte)details.progData[pageCount*pageSize + i]);
        }
        Serial2.write((byte)CRC_EOP);
        while(!Serial2.available());
        byte res1 = Serial2.read();
        while(!Serial2.available());
        byte res2 = Serial2.read();
        if(res1 ==  STK_INSYNC && res2 == STK_OK){         // Page writing was sucessfull 
          pageCount++;
          address += pageSize/2;
        }
        else{
          //Serial.println(res1,HEX);Serial.println(res2,HEX);
          Verbose("Error: STK_PROGRAM_PAGE. Can't write the page properly.");
        }
        break;
      }
    }
    else{
      Verbose("Error: STK_LOAD_ADDR. Failed to load the address.");
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
      Serial.printf("avrdude: %d bytes of flash written.\n",details.len);
      response += "avrdude: "+ String(details.len) +" bytes of flash written.\n";
      // Writing is complete. Now verify. 
      verifyFlash();
      break;
    }
  }
}

/*
 * Read the program page by page and calculate the error. 
 * Note that the the program is already written, even if error is found,
 * its just to notify that user should try flashing again 
*/
void Avrdude::verifyFlash(){
  Verbose("Verifying flash memory against input hex file");
  Serial.printf("avrdude: Loading flash data from input file %s\n",details.fileName.c_str());
  response += "avrdude: Loading flash data from input file " + details.fileName +"\n";
  Serial.printf("avrdude: Input file has %d bytes of Flash\n",details.len);
  response += "avrdude: Input file has " + String(details.len) +" bytes of Flash\n";
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
    Serial2.write((byte)STK_LOAD_ADDRESS);
    Serial2.write((byte)addrL);
    Serial2.write((byte)addrH);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if(res1 ==  STK_INSYNC && res2 == STK_OK){
      Serial.print(F("######"));
      response += "######";
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
            if( data != details.progData[pageCount*pageSize + curr]){
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
            //Serial.println(res2,HEX);
            Verbose("Error: STK_READ_PAGE. Can't read the PAGE");
          }
        }
        else{
          //Serial.println(res1,HEX);
          Verbose("Error: STK_READ_PAGE. Commanf failed");
        }
        break;
      }
    }
    else{
      Verbose("Error: STK_LOAD_ADDR. Can't load the address to read from");
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
      Serial.printf("avrdude: %d bytes of flash verified. (",details.len);
      response += "avrdude: "+ String(details.len) + " bytes of flash verified. (";
      float perc = (error/180)*100;
      Serial.print(perc);
      Serial.println("% error) ");
      response += String(perc) + "% error) \n";
      break;
    }
  }
}

/*
 * After performing intended operation, exit from programming mode. 
*/
void Avrdude::exitPgmMode(){
  Serial2.flush();
  while(1){
    Serial2.write((byte)STK_LEAVE_PROGMODE);
    Serial2.write((byte)CRC_EOP);
    while(!Serial2.available());
    byte res1 = Serial2.read();
    byte res2 = Serial2.read();
    if(res1 == STK_INSYNC && res2 == STK_OK){
         // Command executed succesfully 
         //we do nothing
    }
    else{
      Verbose("Error: STK_LEAVE_PROGMODE. Failed to leave programming mode.");
    }
    break;
  }
}
// ========================  Essential function's definition ends =================================
// ========================  Avrdude class member function definition ends =================================
