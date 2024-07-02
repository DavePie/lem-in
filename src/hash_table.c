#include "lem-in.h"


// stores rooms in a hash table
// returns 1 if there is an error, else 0
int	store_in_hash_table(t_data *data)
{
	data->hash_table_size = get_next_prime(get_num_rooms(data));

	data->rooms = ft_calloc(data->hash_table_size, sizeof(t_room*));

	if (!data->rooms)
		return 1;
	for (uint i = 0; data->temp_rooms[i]; i++)
		if (htable_add(data, data->temp_rooms[i]))
			return 1;
	free(data->temp_rooms);
	if (htable_resize(data))
		return 1;
	return 0;
}


// applies djb2 hash function to a string
ulong	hash(char *str)
{
    ulong	key = 5381;
    int c = 0;

    while ((c = *str++))
        key = ((key << 5) + key) + c; /* key * 33 + c */

    return key;
}


// get room from hash table by name
// returns room if found, else NULL
t_room	*htable_get(t_data *data, char *name)
{
	ulong	key = hash(name) % data->hash_table_size;
	t_room	*tmp = data->rooms[key];

	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}


// adds room to hash table
// returns 1 if there is an error, else 0
int	htable_add(t_data *data, t_room *room)
{
	ulong	key = hash(room->name) % data->hash_table_size;
	t_room	*tmp = data->rooms[key];

	while (tmp)
	{
		if (!ft_strcmp(tmp->name, room->name))
			return 1;
		tmp = tmp->next;
	}
	room->next = data->rooms[key];
	data->rooms[key] = room;
	return 0;
}


// returns percentage of filled indexes in hash table
float	htable_load(t_data *data)
{
	uint	count = 0;

	for (uint i = 0; i < data->hash_table_size; i++)
		if (data->rooms[i])
			count++;
	return (float)count / data->hash_table_size;
}


// resize hash table to the size of last filled index
// returns 1 if there is an error, else 0
int	htable_resize(t_data *data)
{
	uint	new_size = data->hash_table_size;
	t_room	**new_table = NULL;

	while (!data->rooms[new_size - 1])
		new_size--;
	if (new_size == data->hash_table_size)
		return 0;
	
	new_table = ft_calloc(new_size, sizeof(t_room*));
	if (!new_table)
		return 1;
	
	ft_memcpy(new_table, data->rooms, new_size * sizeof(t_room*));
	free(data->rooms);
	data->rooms = new_table;

	return 0;
}
