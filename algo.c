#include "lem-in.h"


t_path **find_best_paths(t_data *data)
{
    data->partial_paths = malloc(sizeof(t_path) * data->num_rooms);
    data->full_paths = malloc(sizeof(t_path) * data->num_rooms);
    t_room **cur = malloc(sizeof(t_room) * data->num_rooms);
    cur[0] = data->start;

    uint depth = 1;
    uint layer_size = 1;
    uint n_new_paths = 0;
    uint cur_best = 0;
    t_path **best_paths_arr;
    
    initialize_paths(data);
    while (layer_size && !best_paths(data, best_paths_arr, n_new_paths, depth))
    {
        uint n = data->n_partial_paths;
        for (int path_i = 0; path_i < n; path_i++)
            extend_path(data, path_i, &n);

        depth++;
    }

    return best_paths_arr;
}

t_path *init_path(t_room **rooms, uint num, uint size)
{
    
    t_path *p = malloc(sizeof(t_path));
    t_room **r = malloc(sizeof(t_room*) * size);
    t_room* cur = *r;
    r[num] = 0;
    while (num-- > 0)
        r[num] = rooms[num];
    *p = (t_path){.max_size = size, .rooms = r, .size = num};
}

void initialize_paths(t_data *data)
{
    t_room *cur = data->start->edges[0];
    int i = 0;
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

// make sure to not operate on new paths
void extend_path(t_data *data, uint index, uint *n)
{
    // cases:
    // remove path, replace with non extended path (n = num path) (n--, num path --)
    // remove path, replace with extended path (num path --)
    // add path, excess goes at end (num path += ?)
}

uint path_contains(t_path *path, t_room *room)
{
    for (int i = 0; i < path->size; i++)
    {
        if (path->rooms[i] == room)
            return 1;
    }
    return 0;
}

// need to only calculate taking new paths into effect
uint best_paths(t_data *data, t_path **paths, uint n_new_paths, uint depth)
{
    uint n_turns = 0;
    for (int i = n_new_paths; i < data->n_full_paths; i++)
    {

    }
    return better_combination(data, depth) > n_turns;
    
}

uint better_combination(t_data *data, uint depth)
{
    return 0;
}