# PROTOCOL FOR UART COMMUNICATON BETWEEN MCU AND NODE 

## HANDSHAKE :

To establish contact the Node sends :
- First SPIDEY_START_TX character which has an Hexadecimal value of 0x01 and corresponds to ASCII Character SOH
- Next it sends SPIDEY_NODE_ACK character which has an Hexadecimal value of 0x10 which corresponds to the ASCII character BS 

The MCU responds with :
- SPIDEY_ACKNOWLEDGE character which has an hexadecimal value of 0x06 which corresponds to ASCII character ACK

## Get Signature Bytes from MCU :

For reading the signature bytes of the MCU which helps in its identification , the Node sends :
- First SPIDEY_GETSIGBYTES character which has an Hexadecimal value of 0x07 and corresponds to ASCII Character BEL
- Next it sends SPIDEY_NODE_ACK character which has an Hexadecimal value of 0x10 which corresponds to the ASCII character BS byte

The MCU responds by :
- Sending first signature byte 
- Sending second signature byte
- Sending third signature byte 
- Sending the SPIDEY_ACKNOWLEDGE character which has an hexadecimal value of 0x06 which corresponds to ASCII character ACK

## Recieve Address of Page :

For recieveing the address of the page which is to be modifed or the page which is to be read the Node sends:
- First SPIDEY_LOAD_ADDRESS character which has an Hexadecimal value of 0x05 and corresponds to ASCII Character ENQ
- Next it sends SPIDEY_NODE_ACK character which has an Hexadecimal value of 0x10 which corresponds to the ASCII character BS 
- Send High byte of address
- Send Low byte of Address

The MCU responds with  :
- SPIDEY_ACKNOWLEDGE character which has an hexadecimal value of 0x06 which corresponds to ASCII character ACK

## Recieve Code to written to page in flash 

For receiving the code to be written , the Node sends :
- First SPIDEY_START_PROGMODE character which has an Hexadecimal value of 0x02 and corresponds to ASCII Character STX
- Next it sends SPIDEY_NODE_ACK character which has an Hexadecimal value of 0x10 which corresponds to the ASCII character BS 
- Send High byte of page length ( where page length is number of bytes that can be stored in a page )
- Send Low byte of page length
- Send bytes to be written to the page (REPEAT THIS TILL NUMBER OF BYTES SENT IS SAME AS NUMBER OF BYTES PER PAGE IN FLASH )

The MCU responds with :
- First it send the SPIDEY_DATA_RECIEVED character which has an hexadecimal value of 0x03 corresponding to ASCII character ETX
- Next it sends the SPIDEY_ACKNOWLEDGE character which has an hexadecimal value of 0x06 which corresponds to ASCII character ACK

## To read a page from flash and send it to Node :

To ensure that the modification of the application code was done properly we have to read a page from the flash and send its contents to the node . For this to be done the Node sends :
- First SPIDEY_READ_FLASH character which has an Hexadecimal value of 0x09 and corresponds to ASCII Character TAB
- Next it sends SPIDEY_NODE_ACK character which has an Hexadecimal value of 0x10 which corresponds to the ASCII character BS 
- Send High byte of page length ( where page length is number of bytes that can be stored in a page )
- Send Low byte of page length

The MCU responds by :
- Sending bytes from page to Node ( Repeats this till number of bytes sent is number of bytes per page)
- At last sending the SPIDEY_ACKNOWLEDGE character which has an hexadecimal value of 0x06 which corresponds to ASCII character ACK

## To Terminate communication :
 The Node sends:
- First SPIDEY_END_TX character which has an Hexadecimal value of 0x04 and corresponds to ASCII Character EOT
- Next it sends SPIDEY_NODE_ACK character which has an Hexadecimal value of 0x10 which corresponds to the ASCII character BS 

The MCU responds with :
- SPIDEY_ACKNOWLEDGE character which has an hexadecimal value of 0x06 which corresponds to ASCII character ACK





