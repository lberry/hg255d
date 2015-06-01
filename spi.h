#ifndef SPI_H
#define SPI_H

#include <REGX51.H>
//#include "uipopt.h"
typedef unsigned char u8_t	 ;
//sfr P4 = 0xe8;

/*ENC28J60 -- pin connect,
  use PIN to PIN*/
//sbit VCC1 = P2^0; // VCC1 NO USE
sbit CSN  = P2^6; // CS
sbit SIN  = P2^1; // MOSI
sbit SON  = P2^0; // MISO
sbit SCKN = P2^7; // SCK
sbit RSTN = P2^3; // RST
sbit INTN = P2^5; // INT 

void WriteByte(u8_t temp);
u8_t ReadByte(void);

#endif
