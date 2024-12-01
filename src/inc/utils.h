#ifndef UTILS_H
# define UTILS_H

#include "types.h"
#include <stdbool.h>

void memset(void *ptr, unsigned char value, unsigned int num);
void memcpy(void *dest, void *src, unsigned int num);
uint8_t memcmp(void *ptr1, void *ptr2, unsigned int num);
void itoa(int value, char *buffer, int base);
void write_hex(uint32_t value);
int atoi(char *str);
bool isNumeric(char *str);

#endif