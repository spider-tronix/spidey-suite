#pragma GCC optimize ("-O0") // avoid GCC memcpy inline
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/pgmspace.h>

#define length(var) ((unsigned int)(sizeof(var)/sizeof(var[0])))    //Macros to get the length of an array

const unsigned long kb = 1024;

typedef struct {
  byte sig [3];
  const char * desc;
  unsigned long flashSize;
  unsigned int baseBootSize;
} signatureProperties;

struct device {
  int deviceMatchIndex = -1;
  byte sig[3];
  byte fuse[4];
};

const signatureProperties signatures[] = {
  //     signature        description  flash size  bootloader size
  // Attiny84 family
  { { 0x1E, 0x91, 0x0B }, "ATtiny24",   2 * kb,         0 },
  { { 0x1E, 0x92, 0x07 }, "ATtiny44",   4 * kb,         0 },
  { { 0x1E, 0x93, 0x0C }, "ATtiny84",   8 * kb,         0 },
  // Attiny85 family
  { { 0x1E, 0x91, 0x08 }, "ATtiny25",   2 * kb,         0 },
  { { 0x1E, 0x92, 0x06 }, "ATtiny45",   4 * kb,         0 },
  { { 0x1E, 0x93, 0x0B }, "ATtiny85",   8 * kb,         0 },
  // Atmega328 family
  { { 0x1E, 0x92, 0x0A }, "ATmega48PA",   4 * kb,         0 },
  { { 0x1E, 0x93, 0x0F }, "ATmega88PA",   8 * kb,       256 },
  { { 0x1E, 0x94, 0x0B }, "ATmega168PA", 16 * kb,       256 },
  { { 0x1E, 0x94, 0x06 }, "ATmega168V",  16 * kb,       256 },
  { { 0x1E, 0x95, 0x0F }, "ATmega328P",  32 * kb,       512 },
  { { 0x1E, 0x95, 0x16 }, "ATmega328PB", 32 * kb,       512 },
  // Atmega644 family
  { { 0x1E, 0x94, 0x0A }, "ATmega164P",   16 * kb,      256 },
  { { 0x1E, 0x95, 0x08 }, "ATmega324P",   32 * kb,      512 },
  { { 0x1E, 0x96, 0x0A }, "ATmega644P",   64 * kb,   1 * kb },
  // Atmega2560 family
  { { 0x1E, 0x96, 0x08 }, "ATmega640",    64 * kb,   1 * kb },
  { { 0x1E, 0x97, 0x03 }, "ATmega1280",  128 * kb,   1 * kb },
  { { 0x1E, 0x97, 0x04 }, "ATmega1281",  128 * kb,   1 * kb },
  { { 0x1E, 0x98, 0x01 }, "ATmega2560",  256 * kb,   1 * kb },
  { { 0x1E, 0x98, 0x02 }, "ATmega2561",  256 * kb,   1 * kb },
  // AT90USB family
  { { 0x1E, 0x93, 0x82 }, "At90USB82",    8 * kb, 512,  },
  { { 0x1E, 0x94, 0x82 }, "At90USB162",  16 * kb, 512,  },
  // Atmega32U2 family
  { { 0x1E, 0x93, 0x89 }, "ATmega8U2",    8 * kb,   512 },
  { { 0x1E, 0x94, 0x89 }, "ATmega16U2",  16 * kb,   512 },
  { { 0x1E, 0x95, 0x8A }, "ATmega32U2",  32 * kb,   512 },
  // Atmega32U4 family
  { { 0x1E, 0x94, 0x88 }, "ATmega16U4",  16 * kb,   512 },
  { { 0x1E, 0x95, 0x87 }, "ATmega32U4",  32 * kb,   512 },
  // ATmega1284P family
  { { 0x1E, 0x97, 0x05 }, "ATmega1284P", 128 * kb,   1 * kb },
  { { 0x1E, 0x97, 0x06 }, "ATmega1284",  128 * kb,   1 * kb },
  // ATtiny4313 family
  { { 0x1E, 0x91, 0x0A }, "ATtiny2313A", 2 * kb,   0 },
  { { 0x1E, 0x92, 0x0D }, "ATtiny4313",  4 * kb,   0 },
  // ATtiny13 family
  { { 0x1E, 0x90, 0x07 }, "ATtiny13A",   1 * kb,   0 },
  // Atmega8A family
  { { 0x1E, 0x93, 0x07 }, "ATmega8A",    8 * kb,  256 },
};

void showHEX(const byte B,const boolean newline=false){
  if(B<16){
    Serial.print("0");
  }
  Serial.print(B,HEX);
  if (newline)
    Serial.println();
}

void readFuse(struct device *D) {
  Serial.print("Fuses: ");
  byte fuse;
  Serial.print("Low=0x");
  D->fuse[0] = boot_lock_fuse_bits_get (GET_LOW_FUSE_BITS);
  showHEX(D->fuse[0]);
  Serial.print("  High=0x");
  D->fuse[1] = boot_lock_fuse_bits_get (GET_HIGH_FUSE_BITS);
  showHEX(D->fuse[1]);
  Serial.print("  Ext=0x");
  D->fuse[2] = boot_lock_fuse_bits_get (GET_EXTENDED_FUSE_BITS);
  showHEX(D->fuse[2]);
  Serial.print("  Lock=0x");
  D->fuse[3] = boot_lock_fuse_bits_get (GET_LOCK_BITS);
  showHEX(D->fuse[3],true);
}

void get_Signature(struct device *D) {
  Serial.print("Signature = 0x");
  D->sig [0] = boot_signature_byte_get (0);
  D->sig [1] = boot_signature_byte_get (2);
  D->sig [2] = boot_signature_byte_get (4);
  showHEX(D->sig[0]);
  Serial.print(" 0x");
  showHEX(D->sig[1]);
  Serial.print(" 0x");
  showHEX(D->sig[2]);
  Serial.println(" ");
}

void verifySignature(struct device *D) {
  for (int i = 0; i < length (signatures); i++) {
    if (memcmp (D->sig, signatures [i].sig, sizeof(D->sig)) == 0) {
      Serial.print("Processor = ");
      Serial.println(signatures [i].desc);
      Serial.print("Flash memory size = ");
      Serial.println(signatures [i].flashSize, DEC);
      D->deviceMatchIndex = i;
      return;
    }
  }
  D->deviceMatchIndex = -1;
  Serial.println ("Unrecogized device signature.");
}

byte readFlash(unsigned long addr){
  #if FLASHEND > 0xFFFF
      return pgm_read_byte_far(addr);
  #else
      return pgm_read_byte(addr);
  #endif
}

void readBootloader(struct device *D) {
  if (signatures[D->deviceMatchIndex].baseBootSize == 0)
  {
    Serial.println("No bootloader support.");
    return;
  }
  unsigned long addr = signatures[D->deviceMatchIndex].flashSize;
  unsigned int  len = signatures[D->deviceMatchIndex].baseBootSize;
  byte hFuse = D->fuse[1];
  Serial.print ("Bootloader in use: ");
  Serial.println((hFuse & bit (0))==0);
  Serial.print ("EEPROM preserved through erase: ");
  Serial.println((hFuse & bit (3))==0);
  Serial.print ("Watchdog timer always on: ");
  Serial.println((hFuse & bit (4))==0);
  switch(hFuse&6){
    case 0: len *= 8; 
            break;
    case 2: len *= 4; 
            break;
    case 4: len *= 2; 
            break;
    case 5: len *= 1; 
            break;
  }
  addr -=len;
  Serial.print("Bootloader size: ");Serial.print(len);Serial.println(" bytes");
  Serial.print("Bootloader starting address: 0x");Serial.println(addr,HEX);
  Serial.println("Bootloader:");
  for (int i = 0; i < len; i++){
    if (i % 16 == 0){
      Serial.print (addr + i, HEX);
      Serial.print (": ");
    }
   showHEX(readFlash(addr + i));
    Serial.print("  ");
    if (i % 16 == 15)
      Serial.println ();
  }
}

int main(void) {
  Serial.begin (115200);
  while (!Serial) {}
  Serial.println("Reading device details.....");
  struct device *D = (struct device *)malloc(sizeof(struct device));
  get_Signature(D);
  readFuse(D);
  verifySignature(D);
  if (D->deviceMatchIndex) {
    readBootloader(D);
  }
  while (1) {
    Serial.println("");
  }
  return 0;
}
