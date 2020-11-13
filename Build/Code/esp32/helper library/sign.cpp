#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>
#include "sign.h"

struct device *D;

int vSign(byte b[]) {
  Serial.begin(115200);

  for (int i=0; i < length (signatures); i++) {
    if (memcmp (b, signatures [i].sig, sizeof(D->sig)) == 0) {
      Serial.print("processor = ");
      Serial.println(signatures [i].desc);
      D->deviceMatchIndex = i;
    return 0;
    }
  }
  D->deviceMatchIndex = -1;
  Serial.println ("Unrecogized device signature.");
  return 0;
}

String btoh(byte a)
{
  int b=a;
char hex[]="ABCDEF";
char num[]="0123456789";
char ch[2];
int i=2;
while(i>0)
{
   if((b%16)>9)
   {
    ch[2-i]=hex[(b%16)%10];
    }
    else
    {
      //itoa(b%16,ch[2-i],10);
      ch[2-i]=num[b%16];
      }
      b/=16;
      i--;
}
  return ch;
  }
