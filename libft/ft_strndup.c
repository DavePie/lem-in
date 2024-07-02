#include <stdlib.h>

char	*ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	i;

	new = (char *)malloc(n + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
