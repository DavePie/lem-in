#include "lem-in.h"

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
    t_room **queue = malloc(sizeof(t_room *) * data->num_rooms);
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

uint calculate_duration(t_data *data, t_paths *paths)
{
    // simple sort
    for (uint i = 0; i < paths->size - 1; i++)
    {
        uint min_i = i;
        for (uint j = i + 1; j < paths->size; j++)
            if (paths->paths[min_i]->size > paths->paths[j]->size)
                min_i = j;
        t_path *temp = paths->paths[min_i];
        paths->paths[min_i] = paths->paths[i];
        paths->paths[i] = temp;
    }

    uint cur_ants = 0;
    uint i = 0;
    for (; i < paths->size - 1; i++)
    {
        uint height_change = paths->paths[i + 1]->size - paths->paths[i]->size;
        if (height_change * (i + 1) + cur_ants > data->num_ants)
            break;
        cur_ants += height_change * (i + 1);
    }
    return (data->num_ants - cur_ants) / (i + 1) + !!((data->num_ants - cur_ants) % (i + 1)) + (paths->paths[i]->size - 2);
}

// // we can expect at best to find a longer or equal path to the current longest path.
uint continue_search(t_data *data, t_paths *old_paths, t_paths *new_paths)
{
    if (data->num_ants <= 1)
        return 0;
    if (!old_paths)
        return 1;
    new_paths->turns = calculate_duration(data, new_paths);
    if (new_paths->turns > old_paths->turns)
        return 0;
    return 1;
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
        printf("On node %s with level %d\n", cur->name, cur->level);
        while (cur->edges[i]->level >= cur->level)
            i++;
        p->rooms[p->size++] = cur->edges[i];
        cur->flow[i]++;
        uint j = get_edge_index(cur->edges[i], cur);
        cur->edges[i]->flow[j]--;
        cur = cur->edges[i];
    }
    visit_path(p);
    print_path(p);
    return p;
}

void find_paths(t_data *data)
{

    t_path *first = find_first_path(data);

    MinHeap h = (MinHeap){.capacity = 1000000, .paths = malloc(sizeof(t_path *) * 1000000), .size = 0};

    int visit_id = 2;
    int found = 1;
    t_paths old_path = (t_paths){.capacity = data->start->num_edges, .paths = malloc(sizeof(t_path *) * data->start->num_edges)};
    t_paths new_path = (t_paths){.capacity = data->start->num_edges, .paths = malloc(sizeof(t_path *) * data->start->num_edges)};

    old_path.paths[old_path.size++] = first;
    old_path.turns = calculate_duration(data, &old_path);
    printf("%d turns currently\n", old_path.turns);

    while (found)
    {
        printf("================================================================================\n");

        found = 0;
        t_path *start = init_path(&data->start, 1, 20, 0);
        data->start->visited = visit_id;
        insert(&h, start);

        t_path *best = 0;
        while (h.size > 0 && best == 0)
        {
            t_path *p = extractMin(&h);
            t_room *last = p->rooms[p->size - 1];
            for (uint i = 0; i < last->num_edges; i++)
            {
                t_room *edge = last->edges[i];
                if (edge == data->end && last->flow[i] < 1)
                {
                    p->rooms[p->size++] = edge;
                    best = p;
                    h.size = 0;
                    found = 1;
                    visit_id++;
                    visit_path(p);
                    update_flow(p);
                    break;
                }
                if ((edge->visited != visit_id && last->flow[i] < 1) && (last->flow[i] == -1 || (p->has_backtrack || last->visited == visit_id)))
                {
                    if (edge->visited != -1 && edge->visited != visit_id)
                        edge->visited = visit_id;
                    p->rooms[p->size] = edge;
                    insert(&h, init_path(p->rooms, p->size + 1, p->cap + 1, last->visited != visit_id));
                }
            }
        }
        if (!found)
            continue;
        printf("FOR FLOW WE HAVE\n");
        print_path(best);
        printf("FOR PATHS WE HAVE:\n");
        printf("num %d\n", data->start->num_edges);
        for (uint i = 0; i < data->start->num_edges; i++)
        {
            // printf("%d\n", i);
            if (data->start->flow[i] == 1)
            {
                // printf("HAS SOMETHING for %s\n", data->start->edges[i]->name);
                t_room *temp[2] = {data->start, data->start->edges[i]};
                t_path *sol = init_path(temp, 2, 200000, 0);
                t_room *cur = sol->rooms[sol->size - 1];

                while (cur != data->end)
                {
                    for (uint j = 0; j < cur->num_edges; j++)
                    {
                        if (cur->flow[j] == 1)
                        {
                            // printf("flow from (%s) to (%s)\n", cur->name, cur->edges[j]->name);
                            sol->rooms[sol->size++] = cur->edges[j];
                            cur = cur->edges[j];
                            break;
                        }
                    }
                }
                new_path.paths[new_path.size++] = sol;
            }
        }
        for (uint j = 0; j < new_path.size; j++)
        {
            print_path(new_path.paths[j]);
        }
        if (continue_search(data, &old_path, &new_path))
        {
            old_path.size = new_path.size;
            old_path.turns = new_path.turns;
            for (uint k = 0; k < new_path.size; k++)
                old_path.paths[k] = new_path.paths[k];
            new_path.size = 0;
        }
        else
        {
            printf("%d turns before, now %d\n", old_path.turns, new_path.turns);

            break;
        }
        printf("%d turns currently\n", old_path.turns);
    }
}