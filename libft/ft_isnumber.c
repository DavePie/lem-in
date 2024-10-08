#include "libft.h"

int	ft_isnumber(char *str)
{
	if (!str || !*str)
		return 0;
	if (*str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return 0;
		str++;
	}
	return 1;
}
