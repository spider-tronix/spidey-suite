#include "signatures.h"

void setup(){
  Serial.begin(9600);
  Serial.println("Initialised...");
  byte sig[3] = { 0x12, 0x95, 0x0F};
  Serial.println("Given Signature is 0x"+hexTOstring(sig[0])+hexTOstring(sig[1])+hexTOstring(sig[2]));
  Serial.println("Verifying Signature.... "+verifySignature(sig));
  printDetails();
}

void loop(){
  //Serial.println("sdssd");
}
