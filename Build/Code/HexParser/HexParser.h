/* HexParser.h
* Created on 1 Nov 2020
*  Author : Miriam
*/

#ifndef HEX_PARSER_H
#define HEX_PARSER_H

uint8_t Data[15000];

uint8_t Ascii2Hex(uint8_t c);

uint8_t * HexParser(uint8_t Start[], int Size, uint8_t * Length);

#endif