#include "lem-in.h"

uint path_contains(t_path *path, t_room *room);


t_path *init_path(t_room **rooms, uint num, uint size)
{

    t_path *p = malloc(sizeof(t_path));
    t_room **r = malloc(sizeof(t_room *) * size);
    uint n = num;

    r[num] = 0;
    while (num-- > 0)
        r[num] = rooms[num];
    *p = (t_path){.max_size = size, .rooms = r, .size = n};
    return p;
}

void initialize_paths(t_data *data)
{
    for (uint i = 0; i < data->start->num_edges; i++)
    {
        t_room *cur = data->start->edges[i];
        if (cur == data->end)
        {
            data->full_paths[data->n_full_paths++] = init_path(&data->end, 1, 2);
            continue;
        }
        data->partial_paths[data->n_partial_paths++] = init_path(&cur, 1, 500);
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
    (void)paths, (void)depth;
    // uint n_turns = 0;
    for (uint i = n_new_paths; i < data->n_full_paths; i++)
    {
    }
    // return better_combination(data, depth) > n_turns;
    return 0;
}

t_path *path_dup_add(t_path *initial, t_room *room, uint original_size)
{
    t_path *test = init_path(initial->rooms, original_size, initial->max_size + 1);
    test->rooms[test->size++] = room;
    return test;
}

// make sure to not operate on new paths
void extend_path(t_data *data, uint *index, uint *n)
{
    uint i = *index;
    t_path *cur = data->partial_paths[i];
    t_room *parent = cur->rooms[cur->size - 1];
    uint n_children = parent->num_edges;
    uint original_size = cur->size;
    int replaced_original = 0;
    int add_end = 0;

    sum_room(parent);
    for (int j = (int)n_children - 1; j >= 0; j--)
    {
        t_room *child = parent->edges[j];
        if (child != data->start && !path_contains(cur, child))
        {
            if (child == data->end)
                add_end = 1;
            else
            {
                if (!replaced_original)
                {
                    replaced_original = 1;
                    cur->rooms[cur->size++] = child;
                }
                else
                    data->partial_paths[data->n_partial_paths++] = path_dup_add(cur, child, original_size);
            }
        }
    }
    if (add_end)
    {
        if (!replaced_original)
        {
            cur->rooms[cur->size++] = data->end;
            data->full_paths[data->n_full_paths++] = cur;
            if (i != data->n_partial_paths - 1)
                data->partial_paths[i] = data->partial_paths[data->n_partial_paths - 1];
            else
                data->partial_paths[i] = 0;
            data->n_partial_paths--;
            // replaced path non extended case
            if (data->n_partial_paths == *n)
            {
                (*n)--;
                (*index)--;
            }
            replaced_original = 1;
        }
        else
            data->full_paths[data->n_full_paths++] = path_dup_add(cur, data->end, original_size);

    }
    else if (!replaced_original)    // dead path (loops on self always)
    {
        // need to free TODO
        if (data->n_partial_paths - 1 == i)
            data->partial_paths[i] = 0;
        else
            data->partial_paths[i] = data->partial_paths[data->n_partial_paths - 1];
        data->n_partial_paths--;
        if (data->n_partial_paths == *n)
        {
            (*n)--;
            (*index)--;
        }
    }
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
    printf("\nstarting\n");
    uint depth = 1;
    uint layer_size = 1;
    uint n_new_paths = 0;
    // uint cur_best = 0;
    t_path **best_paths_arr = 0;

    initialize_paths(data);
    print_paths(data);
    printf("===========================\n");
    while (layer_size && !best_paths(data, best_paths_arr, n_new_paths, depth) && data->n_partial_paths)
    {
        uint n = data->n_partial_paths;
        for (uint path_i = 0; path_i < n; path_i++)
            extend_path(data, &path_i, &n);
        depth++;
        printf("-----------------------------\n");
        print_paths(data);

    }
    return best_paths_arr;
}

uint path_contains(t_path *path, t_room *room)
{
    for (uint i = 0; i < path->size; i++)
        if (path->rooms[i] == room)
            return 1;
    return 0;
}
