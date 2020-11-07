/*  HexParser.cpp
 *  Created on 1 Nov 2020
 *  Author : Miriam
*/
#include <Arduino.h>
#include "HexParser.h"

uint8_t Ascii2Hex(uint8_t c) {
  if (c >= '0' && c <= '9') { 
    return (uint8_t)(c - '0');  
  }
  if (c >= 'A' && c <= 'F') {
    return (uint8_t)(c - 'A' + 10);
  }
  if (c >= 'a' && c <= 'f') {
    return (uint8_t)(c - 'a' + 10);
  }
  return 0;  
}


void Parser(uint8_t Start[], int Size, size_t * Length, uint8_t prog[]) { 
  /* DataB - no of bytes of actual data
   * RecDB - no of  actual data bytes per record
   * U and L - upper and lower nibble of the size field  
   * hexValue - contains a byte of data parsed from 2 bytes of intel hex  
   * index - to index the array storing the parsed data 
   */
  Serial.print(F("Parsing HEX file......"));
  uint8_t *ptr,U,L,hexValue;
  unsigned int Records=0,DataB=0,RecDB=0,index=0;
  int i;
  ptr=Start;
  for( i=0;i<Size;i++) {
      if( *(ptr+i) == ':') {   //shows we have encountered a new record
        Records+=1; 
        i++;
        
        U=Ascii2Hex(*(ptr+i));     
        i++;
        
        L=Ascii2Hex(*(ptr+i));
        i++;
           
        RecDB=(U*16 +L);
        DataB+=RecDB;
  
        i+=6;            //to skip the address and record type fields
        
        //now extracting the record Data
        while(RecDB--) {         
          //Put first nibble in.
          hexValue = Ascii2Hex(*(ptr+i));
          i++;                      
     
          // Shift the nibble to upper half
          hexValue = ((hexValue << 4) & 0xF0);                  
           
          //Put the lower and upper nibbles together.
          hexValue |= (Ascii2Hex(*(ptr+i)));
          i++;
          
          //Store in separate array
          prog[index]=hexValue;                      
          index++;                       
        }
     }
   }
   *Length = DataB; // storing the value of the size at address given by Length
   Serial.println(F("  DONE. OK"));
   return;
}
