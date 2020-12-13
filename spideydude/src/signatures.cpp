#include "signatures.h"

struct device *D = new device();

String verifySignature(byte b[]) {
  String resp="Unrecognized device signature.";
  for (unsigned int i=0; i < length (signatures); i++) {
    if (memcmp (b, signatures[i].sig, sizeof(D->sig)) == 0) {
      resp = signatures[i].desc;
      D->deviceMatchIndex = i;
      D->sig[0] = b[0];D->sig[1] = b[1];D->sig[2] = b[2];
      return resp;
    }
  }
  return resp;
}

void printDetails(){
  int deviceID = D->deviceMatchIndex;
  if(deviceID == -1){
    Serial.println("Verify the Device first.");
    return;
  }
  Serial.println();
  Serial.println("Signature = 0x"+hexTOstring(D->sig[0])+hexTOstring(D->sig[1])+hexTOstring(D->sig[2]));
  Serial.print("Processor = ");Serial.println(signatures[deviceID].desc);
  Serial.print("Flash = ");Serial.print(getFlashSize());Serial.println(" bytes");
  Serial.print("Max. Bootloader Size = ");Serial.print(getBootloaderSize());Serial.println(" bytes");
  Serial.print("Page Size = ");Serial.print(getPageSize());Serial.println(" bytes");
}

unsigned long getFlashSize(){
  int deviceID = D->deviceMatchIndex;
  if(deviceID == -1){
    return -1;
  }
  return signatures[deviceID].flashSize;
}

int getBootloaderSize(){
  int deviceID = D->deviceMatchIndex;
  if(deviceID == -1){
    return -1;
  }
  return signatures[deviceID].maxBootSize;
}

int getPageSize(){
  int deviceID = D->deviceMatchIndex;
  if(deviceID == -1){
    return -1;
  }
  return signatures[deviceID].pageLen;
}

String hexTOstring(byte a){
  int b=a;
  char hex[]="ABCDEF";
  char num[]="0123456789";
  char ch[2];
  int i=1;
  while(i>=0)
  {
     if((b%16)>9){
        ch[i]=hex[(b%16)%10];
     }
     else{
        ch[i]=num[b%16];
     }
     b/=16;
     i--;
  }
  String ans = "";
  ans += ch[0];
  ans += ch[1];
  return ans;
}
