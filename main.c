
/*
 * main.c
 *
 *  Created on: May 26, 2015
 *      Author: MANAL
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include "aes.h"

int main ()
{


	 unsigned char state[4][4] = {{0x32,0x88,0x31,0xE0},{0x43,0x5A,0x31,0x37},{0xF6,0x30,0x98,0x07},{0xa8,0x8D,0xA2,0x34}};
	 unsigned char key[4][4]={{0x2B,0x28,0xAB,0x09},{0x7E,0xAE,0xF7,0xCF},{0x15,0xD2,0x15,0x4F},{0x16,0xA6,0x88,0x3C}};
	 unsigned char expan_key[4][44];


key_generate(key,expan_key);

print(state);


//================ start of Encryption =====================

encryption(state,expan_key);

print(state);
//================ start of Decryption =================


decryption(state,expan_key);


print(state);
	return 0;


}
