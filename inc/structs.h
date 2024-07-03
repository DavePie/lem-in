#ifndef STRUCTS_H
#define STRUCTS_H

typedef unsigned int	uint;
typedef unsigned long	ulong;


typedef struct  s_room
{
	//position
	int				x;
	int				y;
	char			*name;
	uint			num_edges;
	struct s_room	**edges; // rooms connected to this room
}	t_room;


typedef struct s_path
{
	uint	size; // rooms in the path
	uint	max_size;
	t_room	**rooms;
}	t_path;


typedef struct	s_data
{
	uint	num_ants;

	t_room	*start;
	t_room	*end;
	uint	width; // maxium amount of parallel paths

	uint	num_rooms;
	t_room	**temp_rooms; // temporary array for parsing

	uint	num_links;
	char	**temp_links; // temporary array for parsing

	uint	hash_table_size;
	t_room	**rooms; // hash table by name of rooms

	uint	n_partial_paths;
	t_path	**partial_paths; // paths start-end that being built

	uint	n_full_paths;
	t_path	**full_paths; // paths start-end that are complete
}	t_data;

#endif