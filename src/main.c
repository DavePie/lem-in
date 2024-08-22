#include "lem-in.h"
#include "get_next_line.h"
#include <time.h>

int main()
{
    clock_t start = clock();
    t_data data;
    ft_bzero(&data, sizeof(t_data));

    get_data(&data);
    // Print the time taken
    printf("took %f seconds to read \n", ((double)(clock() - start)) / CLOCKS_PER_SEC);

    printf("table load: %f%%\n", htable_load(&data));

    start = clock();
    prune_dead_ends(&data);

    printf("took %f seconds to prune dead ends\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);
    printf("Orignal nodes: %d\n", data.num_rooms);
    // printf("loop nodes removed %d\n", prune_path_simple(&data, data.start->edges[0]));

    assign_levels(&data);

    start = clock();

    find_paths(&data);

    printf("took %f seconds to calculate best path\n", ((double)(clock() - start)) / CLOCKS_PER_SEC);

    safe_exit(&data);
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