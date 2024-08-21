#include "lem-in.h"

typedef struct s_prune_path
{
    uint length;
    t_room *prev;
    t_room *end;
    uint removed;
} t_p_path;

void remove_edge(t_room *from, t_room *to)
{
    uint j = 0;
    while (from->edges[j] != to && j < from->num_edges)
        j++;
    if (j == from->num_edges)
        return;
    from->edges[j] = from->edges[from->num_edges - 1];
    from->edges[from->num_edges - 1] = 0;
    from->num_edges--;
}

int prune_path_simple(t_data *data, t_room *begin)
{

    if (begin->num_edges == 2)
        return 0;
    t_p_path *paths = malloc(sizeof(t_p_path) * begin->num_edges);

    for (uint i = 0; i < begin->num_edges; i++)
    {
        paths[i] = (t_p_path){};
        paths[i].end = begin->edges[i];
        paths[i].prev = begin;
        if (paths[i].end == data->start || paths[i].end == data->end)
            continue;

        while (paths[i].end->num_edges == 2)
        {
            paths[i].end->visited = 1;
            if (paths[i].end->edges[0] != paths[i].prev)
            {
                paths[i].prev = paths[i].end;
                paths[i].end = paths[i].end->edges[0];
            }
            else
            {
                paths[i].prev = paths[i].end;
                paths[i].end = paths[i].end->edges[1];
            }
            paths[i].length++;
            if (paths[i].end == data->start || paths[i].end == data->end)
                break;
        }
    }

    int nodes_removed = 0;

    for (uint i = 0; i < begin->num_edges; i++)
    {
        if (paths[i].end == data->start || paths[i].end == data->end)
        {
            continue;
        }
        if (!paths[i].end->visited)
        {
            paths[i].end->visited = 1;
            nodes_removed += prune_path_simple(data, paths[i].end);
        }
        for (uint j = i + 1; j < begin->num_edges; j++)
        {
            if (paths[i].end == paths[j].end && !paths[i].removed && !paths[j].removed)
            {
                if (paths[i].length > paths[j].length)
                {
                    nodes_removed += paths[i].length;
                    paths[i].removed = 1;
                    remove_edge(paths[i].end, paths[i].prev);
                }
                else
                {
                    nodes_removed += paths[j].length;
                    paths[j].removed = 1;
                    remove_edge(paths[j].end, paths[j].prev);
                }
            }
        }
    }
    for (uint i = begin->num_edges; i > 0; i--)
    {
        if (paths[i - 1].removed)
        {
            begin->edges[i - 1] = begin->edges[begin->num_edges - 1];
            begin->edges[begin->num_edges - 1] = 0;
            begin->num_edges--;
        }
    }
    return nodes_removed;
}

int prune_dead_ends(t_data *data)
{
    int min_prune = 0;
    int extended_prune = 0;

    for (uint i = 0; i < data->num_rooms; i++)
    {
        t_room *cur = data->temp_rooms[i];
        cur->flow = ft_calloc(cur->num_edges, sizeof(int));
        if (cur != data->start && cur != data->end && cur->num_edges == 1)
            min_prune++;
        while (cur != data->start && cur != data->end && cur->num_edges == 1)
        {
            extended_prune++;
            remove_edge(cur->edges[0], cur);
            cur = cur->edges[0];
        }
    }
    printf("min prunes: %d\n", min_prune);
    printf("pruned extended prunes: %d\n", extended_prune);
    return extended_prune;
}