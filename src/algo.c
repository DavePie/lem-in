#include "lem-in.h"

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
        for (uint j = 0; j < r->num_edges; j++)
        {
            if (r->edges[j] == p->rooms[i + 1])
            {
                r->flow[j]++;
                for (uint k = 0; k < p->rooms[i + 1]->num_edges; k++)
                {
                    if (p->rooms[i + 1]->edges[k] == r)
                    {
                        p->rooms[i + 1]->flow[k]--;
                        break;
                    }
                }
                break;
            }
        }
    }
}

// // we can expect at best to find a longer or equal path to the current longest path.
// void continue_search(t_path **old_paths, t_path **new_paths, int numPaths)
// {

// }


void find_paths(t_data *data)
{
    printf("START\n");

    // first path ez
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
        int j = 0;
        while (cur->edges[i]->edges[j] != cur)
            j++;
        cur->edges[i]->flow[j]--;
        cur = cur->edges[i];
    }
    visit_path(p);
    print_path(p);

    MinHeap h = (MinHeap){.capacity = 10000, .paths = malloc(sizeof(t_path *) * 10000), .size = 0};

    int visit_id = 2;
    int found = 1;
    (void )h;
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
            // printf("CURRRR: ");
            // print_path(p);
            t_room *last = p->rooms[p->size - 1];
            for (uint i = 0; i < last->num_edges; i++)
            {
                // printf("OPTION %s with flow %d and visit %d\n", last->edges[i]->name, last->flow[i], last->edges[i]->visited);

                if (last->edges[i] == data->end && last->flow[i] < 1)
                {
                    p->rooms[p->size++] = last->edges[i];
                    best = p;
                    h.size = 0;
                    found = 1;
                    visit_id++;
                    visit_path(p);
                    update_flow(p);
                    break;
                }
                // printf("%d %d\n", last->edges[i]->visited != visit_id, last->flow[i] < 1);
                if ((last->edges[i]->visited != visit_id && last->flow[i] < 1) && (last->flow[i] == -1 || (p->has_backtrack || last->visited == visit_id)))
                {
                    if (last->edges[i]->visited != -1 && last->edges[i]->visited != visit_id)
                        last->edges[i]->visited = visit_id;
                    p->rooms[p->size] = last->edges[i];
                    insert(&h, init_path(p->rooms, p->size + 1, p->cap + 1, last->visited != visit_id));
                }
            }
        }
        printf("FOR FLOW WE HAVE\n");
        print_path(best);
        printf("FOR PATHS WE HAVE:\n");
        if (!found)
            continue;
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
                print_path(sol);
            }
        }

    }

}