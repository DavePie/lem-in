#include "lem-in.h"
#include "get_next_line.h"
#include <time.h>

int main()
{
	clock_t start = clock();
	t_data data;
	ft_bzero(&data, sizeof(t_data));

	get_data(&data);
	clock_t end = clock();

	double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Print the time taken
    printf("took %f seconds to execute \n", cpu_time_used);

	printf("table load: %f%%\n", htable_load(&data));
	// get test
	t_room *tmp = htable_get(&data, "start");
	printf("get test: %s\n", tmp ? tmp->name : "not found");

	printf("edges for starting room:\n");
	t_room *room = data.start;
	for (uint i = 0; room && i < room->num_edges; i++)
		printf("%s\n", room->edges[i]->name);
	room = data.end;
	printf("edges for ending room:\n");
	for (uint i = 0; room && i < room->num_edges; i++)
		printf("%s\n", room->edges[i]->name);
	// if (get_data(&data))
	// 	return clear_data(&data);
	// if (validate_data(&data))
	// 	return clear_data(&data);

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