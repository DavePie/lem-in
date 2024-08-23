#include "lem-in.h"

void simulate(t_data *data)
{
    t_paths *path_temp = &data->old_paths;
    Ant *ants = safe_malloc(sizeof(Ant) * data->num_ants, data);
    uint ants_left = data->num_ants;
    uint num_ants = 0;
    if (data->new_paths.turns < data->old_paths.turns && data->new_paths.size > 0)
        path_temp = &data->new_paths;
    // Paths are already sorted
    t_path **paths = path_temp->paths;
    uint num_paths = path_temp->size;
    uint ants_done = 0;
    uint turns = path_temp->turns;
    while (ants_done != data->num_ants)
    {
        if (ants_left > 0)
        {
            for (uint i = 0; i < num_paths && ants_left > 0; i++)
            {
                if (paths[i]->size - 1 <= turns)
                {
                    ants[num_ants] = (Ant){.name = num_ants + 1, .index = 1, .path = paths[i]};
                    num_ants++;
                    ants_left--;
                }
                else
                    break;
            }
        }
        // ants leave in the order they come
        int left = 0;
        for (uint i = 0; i < num_ants; i++)
        {
            Ant *a = &ants[i];
            if (a->index >= a->path->size)
                continue;
            left = 1;
            printf("L%d-%s ", a->name, a->path->rooms[a->index]->name);
            a->index++;
        }
        turns--;
        printf("\n");
        if (!left)
            break;
    }

    // for (uint i = 0; i < num_paths; i++)
    // {
    //     print_path(paths[i]);
    // }
    free(ants);
    // printf("# turns: %d\n", path_temp->turns);
}
