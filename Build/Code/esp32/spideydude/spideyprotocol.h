/*
 * spideyprotocol.h
 *
 * Created: 10/20/2020 5:09:09 PM
 * Author: Arunesh
 */ 
#ifndef SPIDEYPROTOCOL_H_
#define SPIDEYPROTOCOL_H_

#define SPIDEY_START_TX       0x01
#define SPIDEY_START_PROGMODE 0x02
#define SPIDEY_DATA_RECIEVED  0x03
#define SPIDEY_END_TX         0x04
#define SPIDEY_LOAD_ADDRESS   0x05
#define SPIDEY_ACKNOWLEDGE    0x06
#define SPIDEY_GETSIGBYTES    0x07
#define SPIDEY_CHECK_FLASH    0x09
#define SPIDEY_NODE_ACK       0x10      /*Byte to be sent by ESP as the safety signal */




#endif /* SPIDEYPROTOCOL_H_ */
