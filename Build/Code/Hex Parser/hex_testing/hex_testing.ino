#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>



uint8_t Start[] =
{
':','1','0','0','0','0','0','0','0','0','C','9','4','3','4','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','8','2',' ','\n',
':','1','0','0','0','1','0','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','6','8',' ','\n',
':','1','0','0','0','2','0','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','5','8',' ','\n', 
':','1','0','0','0','3','0','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','4','8',' ','\n',
':','1','0','0','0','4','0','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','3','8',' ','\n',
':','1','0','0','0','5','0','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','2','8',' ','\n',
':','1','0','0','0','6','0','0','0','0','C','9','4','3','E','0','0','0','C','9','4','3','E','0','0','1','1','2','4','1','F','B','E','C','F','E','F','D','8','E','0','4','C',' ','\n',
':','1','0','0','0','7','0','0','0','D','E','B','F','C','D','B','F','0','E','9','4','4','0','0','0','0','C','9','4','5','8','0','0','0','C','9','4','0','0','0','0','D','D',' ','\n',
':','1','0','0','0','8','0','0','0','8','F','E','F','8','4','B','9','1','5','B','8','8','5','B','9','2','F','E','F','3','3','E','D','9','0','E','3','2','1','5','0','8','8',' ','\n',
':','1','0','0','0','9','0','0','0','3','0','4','0','9','0','4','0','E','1','F','7','0','0','C','0','0','0','0','0','1','5','B','8','2','F','E','F','3','3','E','D','7','D',' ','\n',
':','1','0','0','0','A','0','0','0','9','0','E','3','2','1','5','0','3','0','4','0','9','0','4','0','E','1','F','7','0','0','C','0','0','0','0','0','E','B','C','F','D','A',' ','\n',
':','0','4','0','0','B','0','0','0','F','8','9','4','F','F','C','F','F','2',' ','\n',
':','0','0','0','0','0','0','0','1','F','F',' ','\n', 
};


uint8_t Data[400],index=0;

uint8_t Ascii2Hex(uint8_t c)
{
  if (c >= '0' && c <= '9')
  { return (uint8_t)(c - '0');  }

  if (c >= 'A' && c <= 'F')
  {return (uint8_t)(c - 'A' + 10);}

  if (c >= 'a' && c <= 'f')
  {return (uint8_t)(c - 'a' + 10);}

  return 0;  
}



uint8_t * HexParser(uint8_t Start[], int Size, uint8_t * Length)
{ Serial.print("Inside ");
  uint8_t *ptr,Records=0,DataB=0,RecDB=0,U,L,hexValue;
  int i;
  ptr=Start;
  Serial.print(*(ptr+10));
  

  for( i=0;i<Size;i++)
    {
      if( *(ptr+i) == ':')
          { Records+=1;i++;
            U=Ascii2Hex(*(ptr+i));
            Serial.print(U); 
            i++;
            L=Ascii2Hex(*(ptr+i));
            i++;
            Serial.println(L);            
            RecDB=(U*16 +L);
            DataB+=RecDB;

            i+=6;  //to skip the address and record type fields

            //now extracting the record Data
                  while(RecDB--)
                  {         
                      //Put first nibble in.
                      hexValue = Ascii2Hex(*(ptr+i));
                      i++;
                      Serial.print(hexValue,HEX);Serial.print(" "); 
                      // Shift the upper nibble
                      hexValue = ((hexValue << 4) & 0xF0);
                      Serial.print(hexValue,HEX);
                     
                       
                      //Put the nibbles together.
                      hexValue |= (Ascii2Hex(*(ptr+i)));
                      i++;
                      Serial.print("   Merged :");
                      Serial.print(hexValue,HEX); Serial.print(" ");
                      //Store in separate array
                      Data[index]=hexValue; 
                      Serial.print(Data[index],HEX);
                     index++;
                      Serial.println("next byte");                 
                  }
            }
   }
   Serial.print("Inside out ");
   Serial.println(DataB);
   Serial.println(Size);
   Serial.println(Records);
   *Length= DataB; // storing the value of the size Length

   return(Data);
}


int main()
{ uint8_t Length=0,*data;
  int j=1,i,Size=529;
  Serial.begin(9600);
  Serial.println("Testing the hex parser.");
 
  data=HexParser(Start,Size,&Length);
  
  Serial.print("Length of parsed = ");Serial.println(Length);
  
  Serial.println(Data[179],HEX);  Serial.println(Data[175],HEX);  Serial.println(Data[10],HEX);
   
   for (int i=0;i<Length;i++)        //properly parsed if one printed one after other 
   { Serial.println(data[i],HEX);}
   
   
  for (int i=0,j=0;i<200;i++,j++)         //only displays till 172 if till 180,  but if 200 prints fully
    { if(j>=16)
       {j=0;Serial.print("\n");}
     Serial.print(data[i],HEX);
     Serial.print(", ");     
    }

}
