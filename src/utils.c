#include "inc/utils.h"
#include "inc/terminal.h"

void memset(void *ptr, uint8_t value, uint32_t num)
{
	uint8_t *p;
	uint32_t i;

	p = (uint8_t *)ptr;
	i = 0;
	while (i < num)
	{
		p[i] = value;
		i++;
	}
}

void memcpy(void *dest, void *src, uint32_t num)
{
	uint8_t *d;
	uint8_t *s;
	uint32_t i;

	d = (uint8_t *)dest;
	s = (uint8_t *)src;
	i = 0;
	while (i < num)
	{
		d[i] = s[i];
		i++;
	}
}

uint8_t memcmp(void *ptr1, void *ptr2, uint32_t num)
{
	uint8_t *p1;
	uint8_t *p2;
	uint32_t i;

	p1 = (uint8_t *)ptr1;
	p2 = (uint8_t *)ptr2;
	i = 0;
	while (i < num)
	{
		if (p1[i] != p2[i])
			return (1);
		i++;
	}
	return (0);
}

void write_hex(uint32_t value) {
	char hex_chars[] = "0123456789ABCDEF";
	char buffer[11]; // "0x" + 8 hex digit + null terminator
	buffer[0] = '0';
	buffer[1] = 'x';
	for (int i = 0; i < 8; i++) {
		buffer[9 - i] = hex_chars[value & 0xF]; // Son digit'i al
		value >>= 4; // Bir sonraki digit'e geç
	}
	buffer[10] = '\0';
	write(1, buffer, 10); // 10 karakter: "0x" + 8 hex karakter
}

void itoa(int value, char* buffer, int base) {
	char digits[] = "0123456789ABCDEF";
	int i = 0;
	int is_negative = 0;

	if (value < 0 && base == 10) { // Negatif sayılar için destek (sadece base 10)
		is_negative = 1;
		value = -value;
	}

	do {
		buffer[i++] = digits[value % base];
		value /= base;
	} while (value > 0);

	if (is_negative)
		buffer[i++] = '-';

	// String'i ters çevir
	buffer[i] = '\0';
	for (int j = 0; j < i / 2; j++) {
		char tmp = buffer[j];
		buffer[j] = buffer[i - j - 1];
		buffer[i - j - 1] = tmp;
	}
}

int atoi(char *str)
{
	int result = 0;
	int sign = 1;
	int i = 0;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || 
		   str[i] == '\v' || str[i] == '\f' || str[i] == '\r') {
		i++;
	}

	if (str[i] == '-') {
		sign = -1;
		i++;
	} else if (str[i] == '+') {
		i++;
	}

	while (str[i] >= '0' && str[i] <= '9') {
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return result * sign;
}

bool isNumeric(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		if ((str[i] < '0' || str[i] > '9') && str[i] != ' ')
			return false;
	}
	return true;
}
