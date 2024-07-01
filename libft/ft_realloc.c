#include <stdlib.h>
#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	ft_bzero(new_ptr, new_size);
	if (ptr)
	{
		size_t min_size = old_size < new_size ? old_size : new_size;
		ft_memcpy(new_ptr, ptr, min_size);
		free(ptr);
	}
	return (new_ptr);
}
