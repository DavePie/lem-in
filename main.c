
#include "get_next_line.h"

int main()
{
    char *temp = get_next_line(0);
    while (temp)
    {
        printf("%s\n", temp);
        free(temp);
        temp = get_next_line(0);
    }
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