#include "inc/kern.h"

void	putnbr(int32_t nbr)
{
	char str[2];
	str[1] = 0;

	if (nbr > 9)
	{
		putnbr(nbr / 10);
		str[0] = (nbr % 10) + 48;
		write(1, str, 1);
	}
	else if (nbr < 0)
	{
		str[0] = '-';
		write(1, str, 1);
		nbr = nbr * -1;
		putnbr(nbr);
	}
	else
	{
		str[0] = nbr + 48;
		write(1, str, 1);
	}

}