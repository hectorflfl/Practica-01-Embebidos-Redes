/*
 * Crc32.c
 *
 *  Created on: Feb 7, 2019
 *      Author: David Galvez
 */
#define _OPEN_SYS_ITOA_EXT
#include <stdio.h>
#include <stdlib.h>
#include "Crc32.h"

#define NUM_BYTES_MSG 16
#define NUM_BYTES_CRC 16

 unsigned int crc32b(unsigned char *message);


unsigned int crc32_data_recived[4];
 char msg_data_recived[16];
 int a[16];
 char crc_recived[10] = {0};
 unsigned int crc_calculated;
/*int main(void) {
 puts("!!!Hello World!!!"); // prints !!!Hello World!!!
 printf("%u",crc32b((unsigned char*)("Hola como estas")));
 return EXIT_SUCCESS;
 }
 */

 char* iTos(unsigned long a);
 long getPos(int a);

int take_value_crc32(unsigned char *message, char *only_data) {

	static unsigned char bytes_counter;


	bytes_counter = 0;
	while (bytes_counter < NUM_BYTES_CRC) {
		msg_data_recived[bytes_counter] = *message;
		message++;
		bytes_counter++;

	}



	crc_calculated=crc32b(msg_data_recived);

	return strcmp(iTos(crc_calculated), only_data);

}

long getPos(int a){
	long value = 1;
	for(int i = 1; i < a;i++){
		value *= 10;
	}
	return value;
}

char* iTos(unsigned long a){
	long valor = 0;
	static char myarray[10];
	char* pointer = &myarray[0];
	for(int i = 10,j = 0;i > 0;i--){
		valor = a/getPos(i);
		myarray[j] = valor;
		a = a -valor*getPos(i);
		j++;
	}

	return pointer;
}



unsigned int crc32b_comparator(unsigned char *crc1,unsigned char *crc2){


	return strcmp(crc1, crc2);

}
unsigned int crc32b(unsigned char *message) {
	int i, j;
	unsigned int byte, crc, mask;

	i = 0;
	crc = 0xFFFFFFFF;
	for(int k = 0;k < 16;k++){
		byte = message[i];            // Get next byte.
		crc = crc ^ byte;
		for (j = 7; j >= 0; j--) {    // Do eight times.
			mask = -(crc & 1);
			crc = (crc >> 1) ^ (0xEDB88320 & mask);
		}
		i = i + 1;
	}
	return ~crc;
}
