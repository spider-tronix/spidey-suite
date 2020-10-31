uint8_t *Data[15000],index=0;

uint8_t Ascii2Hex(uint8_t c)
{
	if (c >= '0' && c <= '9')
	{	return (uint8_t)(c - '0');	}

	if (c >= 'A' && c <= 'F')
	{return (uint8_t)(c - 'A' + 10);}

	if (c >= 'a' && c <= 'f')
	{return (uint8_t)(c - 'a' + 10);}

	return 0;  
}



uint8_t * HexParser(uint8_t * Start, uint8_t Size)
{ 
  uint8_t i,*ptr,Records=0,DataB=0,RecDB=0,U,L,hexValue;
  ptr=Start;

  for( i=0;i<Size;i++)
    {
      if( *(ptr+i) == ':')
          { Records+=1;i++;
            U=Ascii2Hex(*(ptr+i));
            i++;
            L=Ascii2Hex(*(ptr+i));
            i++;
                        
            RecDB=(U*16 +L);
            DataB+=RecDB;

            //now extracting the record Data
            while(RecDB--)
            {         
                //Put first nibble in.
                hexValue = (Ascii2Hex(*ptr+i));
                i++;

                // Shift the upper nibble
                hexValue = ((hexValue << 4) & 0xF0);

                //Put the nibbles together.
                hexValue |= (Ascii2Hex(*ptr+i));
                i++;

                Data[index]=hexValue;
                index++;
            }

    }

}
}
