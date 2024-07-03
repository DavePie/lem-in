#include "lem-in.h"

t_path *init_path(t_room **rooms, uint num, uint size)
{
    
    t_path *p = malloc(sizeof(t_path));
    t_room **r = malloc(sizeof(t_room*) * size);
    r[num] = 0;
    while (num-- > 0)
        r[num] = rooms[num];
    *p = (t_path){.max_size = size, .rooms = r, .size = num};
    return p;
}

void initialize_paths(t_data *data)
{
    t_room *cur = data->start->edges[0];
    while (cur)
    {
        if (cur == data->end)
        {
            data->full_paths[data->n_full_paths++] = init_path(&data->end, 1, 2);
            continue;
        }
        data->partial_paths[data->n_partial_paths++] = init_path(&cur, 1, 50);
        cur++;
    }
}

uint better_combination(t_data *data, uint depth)
{
    (void)data;
    /*
    ----
    ----
    ---*
    -***
    ****
    */
    return depth;
}

// need to only calculate taking new paths into effect
uint best_paths(t_data *data, t_path **paths, uint n_new_paths, uint depth)
{
    (void)paths;
    uint n_turns = 0;
    for (uint i = n_new_paths; i < data->n_full_paths; i++)
    {

    }
    return better_combination(data, depth) > n_turns;
    
}

// make sure to not operate on new paths
void extend_path(t_data *data, uint index, uint *n)
{
    (void)data, (void) index, (void) n;
    // cases:
    // remove path, replace with non extended path (n = num path) (n--, num path --)
    // remove path, replace with extended path (num path --)
    // add path, excess goes at end (num path += ?)
}

t_path **find_best_paths(t_data *data)
{
    data->partial_paths = malloc(sizeof(t_path) * data->num_rooms);
    data->full_paths = malloc(sizeof(t_path) * data->num_rooms);
    t_room **cur = malloc(sizeof(t_room) * data->num_rooms);
    cur[0] = data->start;

    uint depth = 1;
    uint layer_size = 1;
    uint n_new_paths = 0;
    // uint cur_best = 0;
    t_path **best_paths_arr = 0;
    
    initialize_paths(data);
    while (layer_size && !best_paths(data, best_paths_arr, n_new_paths, depth))
    {
        uint n = data->n_partial_paths;
        for (uint path_i = 0; path_i < n; path_i++)
            extend_path(data, path_i, &n);

        depth++;
    }

    return best_paths_arr;
}




uint path_contains(t_path *path, t_room *room)
{
    for (uint i = 0; i < path->size; i++)
    {
        if (path->rooms[i] == room)
            return 1;
    }
    return 0;
}
