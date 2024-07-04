#ifndef LEMIN_H
#define LEMIN_H

#define NONE 0
#define START 1
#define END 2
#define REALLOC_SIZE 10
#define EDGES_REALLOC 5
#define INPUT_FD 0

#include <stdio.h>

#include "libft.h"
#include "get_next_line.h"
#include "structs.h"

int		get_data(t_data *data);
// hash table
int		store_in_hash_table(t_data *data);
ulong	hash(char *str);
t_room	*htable_get(t_data *data, char *name);
int		htable_add(t_data *data, t_room *room);
float	htable_load(t_data *data);
// utils
int		error(char *msg, char *line, void *to_free);
uint	get_next_prime(uint n);
t_path	**find_best_paths(t_data *data);
// debug
void	print_room(t_room *room, int print_edges);
void	sum_room(t_room *room);
void	print_edges(t_room *room);
void	sum_edges(t_room *room);
void	print_path(t_path *path);
void	sum_path(t_path *path);
void	print_paths(t_data *data);
void	print_data(t_data *data, int print_rooms, int print_edges, int print_paths);

#endif