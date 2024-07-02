#include <stdlib.h>

void	ft_free_tab(void **tab)
{
	if (!tab)
		return ;
	for (size_t i = 0; tab[i]; i++)
		free(tab[i]);
	free(tab);
}
