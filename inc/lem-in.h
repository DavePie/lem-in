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

void get_data(t_data *data);
// hash table
int store_in_hash_table(t_data *data);
ulong hash(char *str);
t_room *htable_get(t_data *data, char *name);
int htable_add(t_data *data, t_room *room);
float htable_load(t_data *data);
// utils
void error(char *msg, char *line, t_data *data);
uint get_next_prime(uint n);
void safe_exit(t_data *data, int code);
void *safe_malloc(size_t size, t_data *data);
void *safe_calloc(size_t count, size_t size, t_data *data);
void zero_free(char **ptr);

// prune
int prune_path_simple(t_data *data, t_room *begin);
int prune_dead_ends(t_data *data);
// algo
int assign_levels(t_data *data);
void find_paths(t_data *data);
t_path *init_path(t_room **rooms, uint num, uint cap, int has_backtrack);

// debug
void print_room(t_room *room, int print_edges);
void sum_room(t_room *room);
void print_edges(t_room *room);
void sum_edges(t_room *room);
void print_path(t_path *path);
void sum_path(t_path *path);
void print_data(t_data *data, int print_rooms, int print_edges, int print_paths);
void print_map(t_data *data);

// heap
void insert(MinHeap *heap, t_path *path, t_data *data);
t_path *extractMin(MinHeap *heap);

// simulate
void simulate(t_data *data);

#endif