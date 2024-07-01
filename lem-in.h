#ifndef LEMIN_H
#define LEMIN_H
#include "libft.h"
#include <stdio.h>

typedef unsigned int uint;

typedef struct s_room
{
    uint x;
    uint y;
    char *name;
    struct s_room **edges;
} t_room;

typedef struct s_path
{
    uint size;
    uint max_size;
    t_room **rooms;
} t_path;

typedef struct s_data
{
    uint num_ants;
    t_room *start;
    t_room *end;
    uint width;
    uint num_rooms;
    t_room **temp_rooms;
    uint hash_table_size;
    t_room **rooms;
    uint n_partial_paths;
    t_path **partial_paths;
    uint n_full_paths;
    t_path **full_paths;
} t_data;

#endif