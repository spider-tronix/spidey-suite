# PROTOCOL FOR UART COMMUNICATION BETWEEN MCU AND PROGRAMMER

The protocol is inspired by **stk500 v1** protocol. There are 4 major steps in the protocol and those are - ```Handshaking``` > ```Reading device signature``` > ```Writing to Flash``` > ```Reading from Flash``` > ```End Communication```  

> Reading from Flash helps in confirming whether any error occurred during transmission or while writing the flash. Even if an error is found, the **bad data** is already written to the flash. The flash should be re-programmed again. 

### Handshaking:

To establish the connection, the programmer sends:
- ```SPIDEY_START_TX``` character which corresponds to ASCII character **SOH**.
- ```SPIDEY_NODE_ACK``` character which corresponds to the ASCII character **BS**. 

The MCU responds with :
- ```SPIDEY_ACKNOWLEDGE``` character which corresponds to ASCII character **ACK**

### Reading device signature:

For reading the signature bytes of the MCU, which helps in its identification, the programmer sends :
- ```SPIDEY_GETSIGBYTES``` character which corresponds to ASCII Character **BEL**.
- ```SPIDEY_NODE_ACK``` character which corresponds to the ASCII character **BS**.

The MCU responds by :
- Sending ```first signature``` byte 
- Sending ```second signature``` byte
- Sending ```third signature``` byte. And so on ... 
- Finally, it sends the ```SPIDEY_ACKNOWLEDGE``` character, which corresponds to ASCII character **ACK**.

### Loading address of the page:

Either for writing or reading from a page, its address is to be loaded first. To do so, the programmer sends:
- ```SPIDEY_LOAD_ADDRESS``` character which corresponds to ASCII Character **ENQ**
- ```SPIDEY_NODE_ACK``` character which corresponds to the ASCII character **BS** 
- ```Low byte``` of the Address
- ```High byte``` of the Address

The MCU responds with  :
- ```SPIDEY_ACKNOWLEDGE``` character which corresponds to ASCII character **ACK**

### Writing to Flash 

For writing to flash, the programmer sends:
- ```SPIDEY_START_PROGMODE``` character which corresponds to ASCII Character **STX**
- ```SPIDEY_NODE_ACK``` character which corresponds to the ASCII character **BS** 
- ```Low byte``` of page length.
- ```High byte``` of page length
- **bytes** to be written to the page (REPEAT THIS TILL NUMBER OF BYTES SENT IS SAME AS THE NUMBER OF BYTES PER PAGE IN THE FLASH OF THE MCU)

The MCU responds with :
- ```SPIDEY_DATA_RECIEVED``` character which corresponding to ASCII character **ETX**
- ```SPIDEY_ACKNOWLEDGE``` character which corresponds to ASCII character **ACK**

### Reading from Flash:
For reading from  flash, the programmer sends:
- ```SPIDEY_READ_FLASH``` character which corresponds to ASCII Character **TAB**
- ```SPIDEY_NODE_ACK``` character which corresponds to the ASCII character **BS** 
- ```Low byte``` of page length
- ```High byte``` of page length

The MCU responds by :
- Sending **bytes** from page ( Repeats this till number of bytes sent is number of bytes per page)
- Finally, ```SPIDEY_ACKNOWLEDGE``` character which corresponds to ASCII character **ACK**

### End Communication:
 The programmer sends:
- ```SPIDEY_END_TX``` character which corresponds to ASCII Character **EOT**
- ```SPIDEY_NODE_ACK``` character which corresponds to the ASCII character **BS** 

The MCU responds with :
- ```SPIDEY_ACKNOWLEDGE``` character which corresponds to ASCII character **ACK**
