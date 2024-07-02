#include "lem-in.h"
#include "get_next_line.h"





int main()
{
	t_data	data;
	ft_bzero(&data, sizeof(t_data));

	get_data(&data);

	printf("table load: %f%%\n", htable_load(&data));
	// get test
	t_room *tmp = htable_get(&data, "start");
	printf ("get test: %s\n", tmp ? tmp->name : "not found");

	printf("edges for room start:\n");
	t_room *room = htable_get(&data, "start");
	for (uint i = 0; i < room->num_edges; i++)
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