/*
 * Crc32.h
 *
 *  Created on: Feb 7, 2019
 *      Author: David Galvez
 */


#ifndef CRC32_H_
#define CRC32_H_

#include <string.h>
char* iTos(unsigned long a, char Server_Client);
unsigned int crc32b(unsigned char *message);
int take_value_crc32(unsigned char *message, char *only_data, char Server_Client);
unsigned int crc32b_comparator(unsigned char *crc1,unsigned char *crc2);
char* get_data_without_crc();

#endif /* CRC32_H_ */
