#include "lem-in.h"

// sends error message with line if provided, frees a pointer if there is one and returns 1
void error(char *msg, char *line, t_data *data)
{
    if (msg)
        write(1, msg, ft_strlen(msg));
    if (line)
        write(1, line, ft_strlen(line));
    safe_exit(data, 1);
}

// get the next prime number after n
uint get_next_prime(uint n)
{
    uint i = n + 1;

    while (1)
    {
        uint j = 2;

        while (j <= i / 2 && i % j != 0)
            j++;
        if (j > i / 2)
            return i;
        i++;
    }
}

void safe_free(void *ptr)
{
    if (ptr)
        free(ptr);
}

void zero_free(char **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = 0;
    }
}

void safe_exit(t_data *data, int code)
{
    while (data->line)
    {
        zero_free(&data->line);
        data->line = get_next_line(INPUT_FD);
    }

    for (uint i = 0; i < data->new_paths.size; i++)
    {
        safe_free(data->new_paths.paths[i]->rooms);
        safe_free(data->new_paths.paths[i]);
    }
    safe_free(data->new_paths.paths);
    for (uint i = 0; i < data->old_paths.size; i++)
    {
        safe_free(data->old_paths.paths[i]->rooms);
        safe_free(data->old_paths.paths[i]);
    }
    safe_free(data->old_paths.paths);
    for (uint i = 0; i < data->num_rooms; i++)
    {
        safe_free(data->temp_rooms[i]->name);
        safe_free(data->temp_rooms[i]->flow);
        safe_free(data->temp_rooms[i]->edges);

        safe_free(data->temp_rooms[i]);
    }
    safe_free(data->temp_rooms);
    free(data->rooms);

    safe_free(data->heap.paths);

    exit(code);
}

void *safe_calloc(size_t count, size_t size, t_data *data)
{
    
    void *ans = ft_calloc(count, size);
    if (!ans)
		error("Memory allocation failed", 0, data);
    return ans;
}

void *safe_malloc(size_t size, t_data *data)
{
    void *ans = malloc(size);
    if (!ans)
		error("Memory allocation failed", 0, data);
    return ans;
}
