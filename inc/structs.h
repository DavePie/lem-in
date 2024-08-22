#ifndef STRUCTS_H
#define STRUCTS_H

typedef unsigned int uint;
typedef unsigned long ulong;

typedef struct s_room
{
	// position
	int x;
	int y;
	char *name;
	uint num_edges;
	uint edge_cap;
	struct s_room **edges; // rooms connected to this room
	int *flow;			   // coresponds to edge of same index
	int visited;
	int level;
} t_room;

typedef struct s_path
{
	uint size; // rooms in the path
	uint cap;
	uint min_left;
	int flow;
	t_room **rooms;
	int has_backtrack;
} t_path;

typedef struct
{
	t_path **paths;
	uint capacity;
	uint size;
	uint turns;
} t_paths;


typedef struct
{
	t_path **paths;
	uint capacity;
	uint size;
} MinHeap;

typedef struct s_data
{
	uint num_ants;

	t_room *start;
	t_room *end;
	uint width; // maxium amount of parallel paths

	uint num_rooms;
	t_room **temp_rooms; // temporary array for parsing
	uint temp_rooms_size;

	uint num_links;
	char **temp_links; // temporary array for parsing

	uint hash_table_size;
	t_room **rooms; // hash table by name of rooms

} t_data;

#endif