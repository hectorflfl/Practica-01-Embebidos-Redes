/*
 * Crc32.h
 *
 *  Created on: Feb 7, 2019
 *      Author: David Galvez
 */


#ifndef CRC32_H_
#define CRC32_H_

#include <string.h>
char* iTos(unsigned long a);
unsigned int crc32b(unsigned char *message);
int take_value_crc32(unsigned char *message, char *only_data);
unsigned int crc32b_comparator(unsigned char *crc1,unsigned char *crc2);

#endif /* CRC32_H_ */
