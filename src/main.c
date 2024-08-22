#include "lem-in.h"
#include "get_next_line.h"
#include <time.h>

int main()
{
    clock_t start = clock();
    clock_t total = clock();
    t_data data;
    ft_bzero(&data, sizeof(t_data));

    if (get_data(&data))
        safe_exit(&data, 1);
    if (!data.start || !data.end)
    {
        printf("Invalid map (missing start/end)\n");
        safe_exit(&data, 1);
    }

    htable_load(&data);
    printf("#took %f seconds to load data\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);

    start = clock();
    prune_dead_ends(&data);

    printf("#took %f seconds to prune dead ends\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);
    printf("#Orignal nodes: %d\n", data.num_rooms);
    // printf("loop nodes removed %d\n", prune_path_simple(&data, data.start->edges[0]));

    if (!assign_levels(&data))
    {
        printf("#Invalid map (no path to end)\n");
        safe_exit(&data, 1);
    }

    start = clock();

    find_paths(&data);

    printf("#took %f seconds to calculate best path\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);
    print_map(&data);
    simulate(&data);
    printf("#total time %f seconds\n", ((double)(clock() - total)) / CLOCKS_PER_SEC);
    safe_exit(&data, 0);
    return 0;
}

