#include "lem-in.h"

// stores rooms in a hash table
// returns 1 if there is an error, else 0
int store_in_hash_table(t_data *data)
{
	data->hash_table_size = get_next_prime(data->num_rooms * 5);
	data->rooms = ft_calloc(data->hash_table_size, sizeof(t_room *));
	if (!data->rooms)
		return 1;
	for (uint i = 0; data->temp_rooms[i]; i++)
		if (htable_add(data, data->temp_rooms[i]))
			return 1;
	free(data->temp_rooms);
	return 0;
}

// applies djb2 hash function to a string
ulong hash(char *str)
{
	ulong key = 5381;
	int c = 0;

	while ((c = *str++))
		key = ((key << 5) + key) + c; /* key * 33 + c */

	return key;
}

// get room from hash table by name
// returns room if found, else NULL
t_room *htable_get(t_data *data, char *name)
{
	ulong key = hash(name) % data->hash_table_size;

	while (data->rooms[key])
	{
		if (!ft_strcmp(data->rooms[key]->name, name))
			return data->rooms[key];
		key = (key + 1) % data->hash_table_size;
	}
	return NULL;
}

// adds room to hash table
// returns 1 if there is an error, else 0
int htable_add(t_data *data, t_room *room)
{
	ulong key = hash(room->name) % data->hash_table_size;

	while (data->rooms[key])
	{
		if (!ft_strcmp(data->rooms[key]->name, room->name))
			return 1;
		key = (key + 1) % data->hash_table_size;
	}
	data->rooms[key] = room;
	return 0;
}

// returns percentage of filled indexes in hash table
float htable_load(t_data *data)
{
	uint count = 0;

	for (uint i = 0; i < data->hash_table_size; i++)
		if (data->rooms[i])
			count++;
	return (float)count / data->hash_table_size;
}
