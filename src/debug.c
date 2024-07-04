#include "lem-in.h"


// print a room
void	print_room(t_room *room, int print_edges)
{
	ft_putstr_fd("[============]\n", 1);
	ft_putstr_fd("room: ", 1);
	if (!room)
	{
		ft_putendl_fd("NULL", 1);
		return;
	}
	ft_putendl_fd(room->name, 1);

	ft_putstr_fd("x: ", 1);
	ft_putnbr_fd(room->x, 1);
	ft_putstr_fd(" y: ", 1);
	ft_putnbrendl_fd(room->y, 1);

	ft_putstr_fd("num_edges: ", 1);
	ft_putnbrendl_fd(room->num_edges, 1);

	if (print_edges)
	{
		ft_putendl_fd("edges: ", 1);
		for (uint i = 0; i < room->num_edges; i++)
		{
			ft_putstr_fd(room->edges[i]->name, 1);
			if (i < room->num_edges - 1)
				ft_putstr_fd(" | ", 1);
		}
		ft_putchar_fd('\n', 1);
	}
	ft_putstr_fd("[============]\n", 1);
}


//  short print of a room
void	sum_room(t_room *room)
{
	if (!room)
	{
		ft_putendl_fd("room: NULL", 1);
		return;
	}
	ft_putstr_fd("room name: ", 1);
	ft_putstr_fd(room->name, 1);
	ft_putstr_fd(", links: ", 1);
	ft_putnbrendl_fd(room->num_edges, 1);
}


//  print edges of a room
void	print_edges(t_room *room)
{
	if (!room)
	{
		ft_putendl_fd("can't print edges of non-existing room", 1);
		return;
	}
	ft_putendl_fd("edges of ", 1);
	ft_putstr_fd(room->name, 1);
	ft_putstr_fd(": ", 1);
	for (uint i = 0; i < room->num_edges; i++)
	{
		ft_putstr_fd(room->edges[i]->name, 1);
		if (i < room->num_edges - 1)
			ft_putstr_fd(" | ", 1);
	}
	ft_putchar_fd('\n', 1);
}


// short print of edges of a room
void	sum_edges(t_room *room)
{
	if (!room)
	{
		ft_putendl_fd("can't print edges of non-existing room", 1);
		return;
	}
	ft_putstr_fd("edges: ", 1);
	for (uint i = 0; i < room->num_edges; i++)
	{
		ft_putstr_fd(room->edges[i]->name, 1);
		if (i < room->num_edges - 1)
			ft_putstr_fd(" | ", 1);
	}
	ft_putchar_fd('\n', 1);
}


// print a path
void	print_path(t_path *path)
{
	ft_putstr_fd("[============]\n", 1);
	ft_putstr_fd("path: ", 1);
	if (!path)
	{
		ft_putendl_fd("NULL", 1);
		return;
	}
	ft_putnbrendl_fd(path->size, 1);
	for (uint i = 0; i < path->size; i++)
	{
		ft_putstr_fd(path->rooms[i]->name, 1);
		if (i < path->size - 1)
			ft_putstr_fd(" -> ", 1);
	}
	ft_putstr_fd("[============]\n", 1);
}


// short print of a path
void	sum_path(t_path *path)
{
	if (!path)
	{
		ft_putendl_fd("NULL", 1);
		return;
	}
	for (uint i = 0; i < path->size; i++)
	{
		ft_putstr_fd(path->rooms[i]->name, 1);
		if (i < path->size - 1)
			ft_putstr_fd("->", 1);
	}
	ft_putchar_fd('\n', 1);
}


// print paths
void	print_paths(t_data *data)
{

	ft_putstr_fd("Partial paths: ", 1);
	ft_putnbrendl_fd(data->n_partial_paths, 1);
	for (uint i = 0; i < data->n_partial_paths; i++)
	{
		ft_putstr_fd(data->start->name, 1);
		ft_putstr_fd("->", 1);
		sum_path(data->partial_paths[i]);
	}
	ft_putstr_fd("Full paths: ", 1);
	ft_putnbrendl_fd(data->n_full_paths, 1);
	for (uint i = 0; i < data->n_full_paths; i++)
	{
		ft_putstr_fd(data->start->name, 1);
		ft_putstr_fd("->", 1);
		sum_path(data->full_paths[i]);
	}
}


// print data
void	print_data(t_data *data, int print_rooms, int print_edges, int print_paths)
{
	ft_putstr_fd("[============]\n", 1);
	ft_putendl_fd("data:", 1);
	ft_putstr_fd("num_ants: ", 1);
	ft_putnbrendl_fd(data->num_ants, 1);

	ft_putendl_fd("start:", 1);
	print_room(data->start, 0);
	ft_putendl_fd("end:", 1);
	print_room(data->end, 0);

	ft_putstr_fd("width: ", 1);
	ft_putnbrendl_fd(data->width, 1);

	ft_putstr_fd("num_rooms: ", 1);
	ft_putnbrendl_fd(data->num_rooms, 1);

	if (print_rooms)
	{
		ft_putendl_fd("rooms:", 1);
		for (uint i = 0; i < data->num_rooms; i++)
			print_room(data->rooms[i], print_edges);
	}

	if (print_paths)
	{
		ft_putendl_fd("partial_paths:", 1);
		for (uint i = 0; i < data->n_partial_paths; i++)
			print_path(data->partial_paths[i]);
		ft_putendl_fd("full_paths:", 1);
		for (uint i = 0; i < data->n_full_paths; i++)
			print_path(data->full_paths[i]);
	}
	ft_putstr_fd("[============]\n", 1);
}