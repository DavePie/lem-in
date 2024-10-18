#include "lem-in.h"

t_path *init_path(t_room **rooms, uint num, uint cap, int has_backtrack)
{
    uint n = num;
    t_path *p = malloc(sizeof(t_path));

    if (!p)
        return 0;
    t_room **r = malloc(sizeof(t_room *) * cap);
    if (!r)
    {
        free(p);
        return 0;
    }

    r[num] = 0;
    if (num >= 1)
        p->min_left = rooms[num - 1]->level;
    while (num-- > 0)
        r[num] = rooms[num];
    *p = (t_path){.cap = cap, .rooms = r, .size = n, .has_backtrack = has_backtrack};
    return p;
}

uint get_edge_index(t_room *r, t_room *edge)
{
    for (uint i = 0; i < r->num_edges; i++)
        if (r->edges[i] == edge)
            return i;
    return -1;
}

int assign_levels(t_data *data)
{
    int path_to_sink = 0;
    t_room **queue = safe_malloc(sizeof(t_room *) * data->num_rooms, data);
    if (!queue)
        return 0;

    int cur = 0;
    int num_queue = 1;
    queue[0] = data->end;
    data->end->level = 1;

    while (num_queue > 0)
    {
        t_room *r = queue[cur];
        if (r != data->start)
            for (uint i = 0; i < r->num_edges; i++)
                if (r->edges[i]->level == 0)
                {
                    r->edges[i]->level = r->level + 1;
                    queue[cur + num_queue] = r->edges[i];
                    num_queue++;
                }
        cur++;
        num_queue--;
        if (r == data->start)
            path_to_sink = 1;
    }
    free(queue);
    return path_to_sink;
}

void visit_path(t_path *p)
{
    for (uint i = 0; i < p->size; i++)
        p->rooms[i]->visited = -1;
}

void update_flow(t_path *p)
{
    for (uint i = 0; i < p->size - 1; i++)
    {
        t_room *r = p->rooms[i];

        uint j = get_edge_index(r, p->rooms[i + 1]);
        r->flow[j]++;

        uint k = get_edge_index(p->rooms[i + 1], r);
        p->rooms[i + 1]->flow[k]--;
    }
}

void sort_paths(t_paths *paths_group)
{
    uint size = paths_group->size;
    t_path **paths = paths_group->paths;

    // simple sort
    for (uint i = 0; i < size - 1; i++)
    {
        uint min_i = i;
        for (uint j = i + 1; j < size; j++)
            if (paths[min_i]->size > paths[j]->size)
                min_i = j;
        t_path *temp = paths[min_i];
        paths[min_i] = paths[i];
        paths[i] = temp;
    }
}

uint calculate_duration(t_data *data, t_paths *paths_group)
{
    uint size = paths_group->size;
    t_path **paths = paths_group->paths;
    uint cur_ants = 0;
    uint i = 0;
    for (; i < size - 1; i++)
    {
        uint height_change = paths[i + 1]->size - paths[i]->size;
        if (height_change * (i + 1) + cur_ants > data->num_ants)
            break;
        cur_ants += height_change * (i + 1);
    }
    return (data->num_ants - cur_ants) / (i + 1) + !!((data->num_ants - cur_ants) % (i + 1)) + (paths[i]->size - 2);
}

// we can expect at best to find a longer or equal path to the current longest path->
uint continue_search(t_data *data, t_paths *old_paths, t_paths *new_paths)
{
    if (data->num_ants <= 1)
        return 0;
    if (!old_paths)
        return 1;
    sort_paths(new_paths);
    new_paths->turns = calculate_duration(data, new_paths);
    return (new_paths->turns <= old_paths->turns);
}

t_path *find_first_path(t_data *data)
{
    uint size = data->start->level;
    t_path *p = init_path(&data->start, 1, size, 0);

    t_room *cur = data->start;
    while (cur != data->end)
    {
        int i = 0;
        cur->visited = 1;
        while (cur->edges[i]->level >= cur->level)
            i++;
        p->rooms[p->size++] = cur->edges[i];
        cur->flow[i]++;
        uint j = get_edge_index(cur->edges[i], cur);
        cur->edges[i]->flow[j]--;
        cur = cur->edges[i];
    }
    visit_path(p);
    return p;
}

void find_new_paths(t_data *data, t_paths *n_path)
{
    for (uint i = 0; i < data->start->num_edges; i++)
    {
        if (data->start->flow[i] == 1)
        {
            t_room *temp[2] = {data->start, data->start->edges[i]};
            t_path *sol = init_path(temp, 2, data->num_rooms, 0);
            t_room *cur = sol->rooms[sol->size - 1];

            while (cur != data->end)
                for (uint j = 0; j < cur->num_edges; j++)
                    if (cur->flow[j] == 1)
                    {
                        sol->rooms[sol->size++] = cur->edges[j];
                        cur = cur->edges[j];
                        break;
                    }
            n_path->paths[n_path->size++] = sol;
        }
    }
}

int contains(t_path *p, t_room *r)
{
    for (uint k = 0; k < p->size; k++)
        if (r == p->rooms[k])
            return 1;
    return 0;
}

t_path *find_flow_path(t_data *data, MinHeap *h, int *visit_id)
{
    while (h->size > 0)
    {
        t_path *p = extractMin(h);
        t_room *last = p->rooms[p->size - 1];
        for (uint i = 0; i < last->num_edges; i++)
        {
            t_room *edge = last->edges[i];
            if (edge == data->end && last->flow[i] < 1)
            {
                p->rooms[p->size++] = edge;
                visit_path(p);
                update_flow(p);
                return p;
            }
            if ((edge->visited != *visit_id && last->flow[i] < 1) && (last->flow[i] == -1 || (p->has_backtrack || last->visited == *visit_id)) &&  !contains(p, edge))//&& last->edge_visit[i] != *visit_id)
            {
                if (edge->visited != -1)
                    edge->visited = *visit_id;
                last->edge_visit[i] = *visit_id;
                p->rooms[p->size] = edge;
                insert(h, init_path(p->rooms, p->size + 1, p->cap + 1, last->visited != *visit_id), data);
            }
        }
        free(p->rooms);
        free(p);
    }
    return 0;
}

void find_paths(t_data *data)
{

    t_path *first = find_first_path(data);

    data->heap = (MinHeap){.capacity = 400000, .paths = safe_malloc(sizeof(t_path *) * 400000, data), .size = 0};
    MinHeap *h = &data->heap;
    int visit_id = 2;
    int new_path_found = 1;

    // Can't have more paths than number of edges connecting to start
    data->old_paths = (t_paths){.capacity = data->start->num_edges, .paths = safe_malloc(sizeof(t_path *) * data->start->num_edges, data)};
    data->new_paths = (t_paths){.capacity = data->start->num_edges, .paths = safe_malloc(sizeof(t_path *) * data->start->num_edges, data)};

    t_paths *o_path = &data->old_paths;
    t_paths *n_path = &data->new_paths;

    o_path->paths[o_path->size++] = first;
    o_path->turns = calculate_duration(data, o_path);

    while (new_path_found)
    {
        new_path_found = 0;
        t_path *start = init_path(&data->start, 1, 20, 0);
        data->start->visited = visit_id;
        insert(h, start, data);

        new_path_found = !!find_flow_path(data, h, &visit_id);
        while (h->size > 0)
        {
            free(h->paths[h->size - 1]->rooms);
            free(h->paths[h->size-- - 1]);
        }
        visit_id++;
        if (!new_path_found)
            continue;
        find_new_paths(data, n_path);
        if (continue_search(data, o_path, n_path))
        {
            o_path->turns = n_path->turns;
            for (uint k = 0; k < n_path->size; k++)
            {
                if (k < o_path->size)
                {
                    free(o_path->paths[k]->rooms);
                    free(o_path->paths[k]);
                }
                o_path->paths[k] = n_path->paths[k];
            }
            o_path->size = n_path->size;

            n_path->size = 0;
        }
        else
            break;
    }
}