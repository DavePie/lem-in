#include "lem-in.h"

// sends error message with line if provided, frees a pointer if there is one and returns 1
int	error(char *msg, char *line, void *to_free)
{
	if (msg)
		write(1, msg, ft_strlen(msg));
	if (line)
		write(1, line, ft_strlen(line));
	if (to_free)
		free(to_free);
	return 1;
}


// get number of rooms
uint	get_num_rooms(t_data *data)
{
	uint	i = 0;

	while (data->temp_rooms[i])
		i++;
	return i;
}


// get the next prime number after n
uint	get_next_prime(uint n)
{
	uint	i = n + 1;

	while (1)
	{
		uint	j = 2;

		while (j <= i / 2 && i % j != 0)
			j++;
		if (j > i / 2)
			return i;
		i++;
	}
}
