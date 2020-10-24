/*
 * spideyprotocol.h
 *
 * Created: 10/20/2020 5:09:09 PM
 *  Author: hp
 */ 


#ifndef SPIDEYPROTOCOL_H_
#define SPIDEYPROTOCOL_H_
#define spidey_start_tx 0x01
#define spidey_end_tx  0x04
#define spidey_load_address  0x05
#define spidey_start_progmode  0x02
#define spidey_data_recieved   0x03
#define spidey_acknowledge   0x06
#define spidey_getsigbyte 0x07
#define spidey_error_in_flash 0x08
#define spidey_data_correct 0x09
#define spidey_node_ack 0x10      /*Byte to be sent by ESP as the safety signal */




#endif /* SPIDEYPROTOCOL_H_ */