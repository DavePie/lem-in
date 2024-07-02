#include "lem-in.h"

int	is_room_shaped(char *line);
int	is_link_shaped(char *line);
int	add_room(t_data *data, char *line, uint modifier);
int	add_link(t_data *data, char **line);
int	apply_links(t_data *data);
int	link_rooms(t_room *room1, t_room *room2);


// read the input and store the data (num ants, rooms and edges)
// returns 1 if there is an error, else 0
int	get_data(t_data *data)
{
	char	*line = NULL;
	uint	len;
	uint	modifier = NONE;
	uint	is_first = 1;

	data->temp_rooms = ft_realloc(data->temp_rooms, 0, (REALLOC_SIZE + 1 ) * sizeof(t_room *));
	data->temp_links = ft_realloc(data->temp_links, 0, (REALLOC_SIZE + 1 ) * sizeof(char *));
	if (!data->temp_rooms || !data->temp_links)
		return error("Error: memory allocation failed\n", NULL, NULL);

	line = get_next_line(INPUT_FD);
	while (line)
	{
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = 0;

		if (!ft_strncmp(line, "#", 1)) // comment
		{
			if (!ft_strncmp(line, "##", 2)) // directive
			{
				modifier = !ft_strcmp(line, "##start") * START  + !ft_strcmp(line, "##end") * END;
				if (modifier == NONE)
					return error("Error: invalid directive:\n", line, line);
			}
		}
		else if (is_first)
		{
			if (!ft_isnumber(line))
				return error("Error: first entry must be ants quantity:\n", line, line);
	
			data->num_ants = ft_atoi(line);
			is_first = 0;
		}
		else if (is_room_shaped(line))
		{
			if (is_first)
				return error("Error: first entry must be ants quantity:\n", line, line);

			if (add_room(data, line, modifier))
				return error(NULL, NULL, line);
			
			modifier = NONE;
		}
		else if (is_link_shaped(line))
		{
			if (is_first)
				return error("Error: first entry must be ants quantity:\n", line, line);
			else if (modifier)
				return error("Error: link after directive:\n", line, line);

			if (add_link(data, &line))
				return error("Error: invalid link:\n", line, line);	
		}
		else
			return error("Error: invalid line:\n", line, line);
		if (line)
			free(line);
		line = get_next_line(INPUT_FD);
	}
	if (store_in_hash_table(data) || apply_links(data))
		return error(NULL, NULL, NULL);
	return 0;
}


// room shaped line has shape "[name(string)] [x(uint)] [y(uint)]"
// returns 1 if line is room shaped, else 0
int	is_room_shaped(char *line)
{
	uint	space_count = 0;
	uint	x_len = 0;
	uint	y_len = 0;
	uint	name_len = 0;

	for (uint i = 0; line[i] && line[i] != '\n'; i++)
	{
		if (line[i] == ' ')
			space_count++;
		else if (space_count == 0)
		{
			if (ft_isprint(line[i]))
				name_len++;
		}
		else if (space_count == 1)
		{
			if (!ft_isdigit(line[i]) && line[i] != '-')
				return 0;
			x_len++;
		}
		else if (space_count == 2)
		{
			if (!ft_isdigit(line[i]) && line[i] != '-')
				return 0;
			y_len++;
		}
		else
			return 0;
	}
	return (space_count == 2 && name_len && x_len && y_len);
}


// link shaped line has shape "[name1(string)]-[name2(string)]"
// returns 1 if line is link shaped, else 0
int	is_link_shaped(char *line)
{
	uint	dash_count = 0;
	uint	name1_len = 0;
	uint	name2_len = 0;

	for (uint i = 0; line[i] && line[i] != '\n'; i++)
	{
		if (line[i] == '-')
			dash_count++;
		else if (dash_count == 0)
		{
			if (ft_isprint(line[i]))
				name1_len++;
		}
		else if (dash_count == 1)
		{
			if (ft_isprint(line[i]))
				name2_len++;
		}
		else
			return 0;
	}
	return (dash_count == 1 && name1_len && name2_len);
}


// adds a room to the data in temp_rooms
// returns 1 if there is an error, else 0
int	add_room(t_data *data, char *line, uint modifier)
{
	t_room	*room = ft_calloc(1, sizeof(t_room));

	if (!room)
		return 1;

	room->name = ft_strndup(line, ft_strchr(line, ' ') - line);
	room->x = ft_atoi(ft_strchr(line, ' '));
	room->y = ft_atoi(ft_strrchr(line, ' '));

	if (modifier == START)
		data->start = room;
	else if (modifier == END)
		data->end = room;

	data->temp_rooms[data->num_rooms++] = room;
	if (data->num_rooms % REALLOC_SIZE == 0) // reallocation if the array is full 
	{
		data->temp_rooms = ft_realloc(data->temp_rooms, data->num_rooms * sizeof(t_room *), (data->num_rooms + REALLOC_SIZE) * sizeof(t_room *));
		if (!data->temp_rooms)
			return 1;
	}
	return 0;
}


// adds a link to the data in temp_links
// returns 1 if there is an error, else 0
int	add_link(t_data *data, char **line)
{
	data->temp_links[data->num_links++] = *line;
	if (data->num_links % REALLOC_SIZE == 0) // reallocation if the array is full 
	{
		data->temp_links = ft_realloc(data->temp_links, data->num_links * sizeof(char *), (data->num_links + REALLOC_SIZE) * sizeof(char *));
		if (!data->temp_links)
			return 1;
	}
	*line = NULL;
	return 0;
}


// apply links to the rooms
// returns 1 if there is an error, else 0
int	apply_links(t_data * data)
{
	t_room	*room1 = NULL;
	t_room	*room2 = NULL;
	char 	**names = NULL;

	for (uint i = 0; i < data->num_links; i++)
	{
		names = ft_split(data->temp_links[i], '-');
		if (!names)
			return 1;
		room1 = htable_get(data, names[0]);
		room2 = htable_get(data, names[1]);
		ft_free_tab((void **)names);
		if (!room1 || !room2)
			return error("Error: invalid link:\n", data->temp_links[i], names);
		if (link_rooms(room1, room2))
			return 1;
	}
	return 0;
}


// link two rooms
// returns 1 if there is an error, else 0
int	link_rooms(t_room *room1, t_room *room2)
{
	if (room1->num_edges % EDGES_REALLOC == 0)
	{
		room1->edges = ft_realloc(room1->edges, room1->num_edges * sizeof(t_room *), (room1->num_edges + EDGES_REALLOC) * sizeof(t_room *));
		if (!room1->edges)
			return 1;
	}
	room1->edges[room1->num_edges++] = room2;
	if (room2->num_edges % EDGES_REALLOC == 0)
	{
		room2->edges = ft_realloc(room2->edges, room2->num_edges * sizeof(t_room *), (room2->num_edges + EDGES_REALLOC) * sizeof(t_room *));
		if (!room2->edges)
			return 1;
	}
	room2->edges[room2->num_edges++] = room1;
	return 0;
}
