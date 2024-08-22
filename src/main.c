#include "lem-in.h"
#include "get_next_line.h"
#include <time.h>

int main()
{
    clock_t start = clock();
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

    start = clock();
    prune_dead_ends(&data);

    printf("took %f seconds to prune dead ends\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);
    printf("Orignal nodes: %d\n", data.num_rooms);
    // printf("loop nodes removed %d\n", prune_path_simple(&data, data.start->edges[0]));

    if (!assign_levels(&data))
    {
        printf("Invalid map (no path to end)\n");
        safe_exit(&data, 1);
    }

    start = clock();

    find_paths(&data);

    printf("took %f seconds to calculate best path\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);
    print_map(&data);
    simulate(&data);
    safe_exit(&data, 0);
    return 0;
}

// Path limiters
// number of edges to start and end
// number of ants
// intersections

// Path eliminators

// during path building
// visiting already visited nodes for this path

// after path buliding
// 2 path with a single intersection overall with each other, shorter path wins

// Path algo

// find shortest path(s) of length n

// will number (current width) of paths of length (n+1) be helpful?

// special case: direct line from start to end