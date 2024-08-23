#include "lem-in.h"

int is_room_shaped(char *line);
int is_link_shaped(t_data *data, char *line);
int add_room(t_data *data, uint modifier);
int link_rooms(t_room *room1, t_room *room2);
char *get_next_noncomment_line(int fd, t_data *data);

// read the input and store the data (num ants, rooms and edges)
// returns 1 if there is an error, else 0
int get_data(t_data *data)
{
	data->line = NULL;
	uint modifier = NONE;

	data->temp_rooms_size = 20;
	data->temp_rooms = ft_calloc(data->temp_rooms_size, sizeof(t_room *));
	if (!data->temp_rooms)
		error("Error: memory allocation failed\n", NULL, data);

	get_next_noncomment_line(INPUT_FD, data);
	if (!data->line || !ft_isnumber(data->line))
		error("Error: first entry must be ants quantity:\n", 0, data);
	data->num_ants = ft_atoi(data->line);
	zero_free(&data->line);
	while (get_next_noncomment_line(INPUT_FD, data))
	{
		if (!ft_strncmp(data->line, "##", 2)) // directive
			modifier = !ft_strcmp(data->line + 2, "start") * START + !ft_strcmp(data->line + 2, "end") * END;
		else if (is_room_shaped(data->line))
		{
			add_room(data, modifier);
			modifier = NONE;
		}
		else
			break;
		zero_free(&data->line);
	}
	if (modifier)
		error("Error: invalid command usuage:\n", data->line, data);
	if (store_in_hash_table(data))
		error(NULL, NULL, data);
	while (data->line)
	{
		if (!is_link_shaped(data, data->line))
			safe_exit(data, 1);
		zero_free(&data->line);
		get_next_noncomment_line(INPUT_FD, data);
	}
	if (data->line)
		error("Error: invalid line:\n", data->line, data);

	return 0;
}

// gets the next nonempty line
char *get_next_noncomment_line(int fd, t_data *data)
{
	data->line = get_next_line(fd);

	while (data->line && !ft_strncmp(data->line, "#", 1) && ft_strncmp(data->line, "##", 2))
	{
		if (!ft_strncmp(data->line, "\n", 1))
			error("Error: empty line\n", data->line, data);
		zero_free(&data->line);
		data->line = get_next_line(fd);
	}
	if (data->line)
	{
		int len = ft_strlen(data->line);
		if (len > 0 && (data->line)[len - 1] == '\n')
			(data->line)[len - 1] = 0;
	}
	return data->line;
}

// room shaped line has shape "[name(string)] [x(uint)] [y(uint)]"
// returns 1 if line is room shaped, else 0
int is_room_shaped(char *line)
{
	if (*line == ' ')
		return 0;
	while (*line != ' ' && *line)
		if (!ft_isprint(*(line++)))
			return 0;
	if (*(line++) != ' ' || *line == ' ')
		return 0;
	while (*line != ' ' && *line)
	{
		if (!ft_isdigit(*line) && *line != '-')
			return 0;
		line++;
	}
	if (*(line++) != ' ' || *line == ' ')
		return 0;
	while (*line != ' ' && *line)
	{
		if (!ft_isdigit(*line) && *line != '-')
			return 0;
		line++;
	}
	return *line == '\0';
}

// link shaped line has shape "[name1(string)]-[name2(string)]"
// returns 1 if line is link shaped, else 0
int is_link_shaped(t_data *data, char *line)
{
	char *l = line;

	if (*line == ' ' || *line == '-')
		return 0;
	while (*line != ' ' && *line != '-' && *line)
		if (!ft_isprint(*(line++)))
			return 0;
	if (*(line++) != '-')
		return 0;
	while (*line != ' ' && *line != '-' && *line)
		if (!ft_isprint(*(line++)))
			return 0;
	if (*line != '\0')
		return 0;
	char **names = ft_split(l, '-');
	if (!names)
		return 1;
	t_room *room1 = htable_get(data, names[0]);
	t_room *room2 = htable_get(data, names[1]);
	ft_free_tab((void **)names);

	if (!room1 || !room2)
		error("Error: invalid link:\n", l, data);
	int a = link_rooms(room1, room2);
	int b = link_rooms(room2, room1);
	return !(a && b);
}

// adds a room to the data in temp_rooms
// returns 1 if there is an error, else 0
int add_room(t_data *data, uint modifier)
{
	t_room *room = safe_calloc(1, sizeof(t_room), data);

	if (!room)
		return 1;

	room->name = ft_strndup(data->line, ft_strchr(data->line, ' ') - data->line);
	if (!room->name)
	{
		free(room);
		error("Memory allocation failed", 0, data);
	}
	room->x = ft_atoi(ft_strchr(data->line, ' '));
	room->y = ft_atoi(ft_strrchr(data->line, ' '));
	room->edges = ft_calloc(10, sizeof(t_room *));
	if (!room->edges)
	{
		free(room->name);
		free(room);
		error("Memory allocation failed", 0, data);
	}
	room->edge_cap = 10;

	if (modifier == START)
		data->start = room;
	else if (modifier == END)
		data->end = room;

	data->temp_rooms[data->num_rooms++] = room;
	if (data->num_rooms == data->temp_rooms_size) // reallocation if the array is full
	{
		data->temp_rooms = ft_realloc(data->temp_rooms, data->num_rooms * sizeof(t_room *), (data->temp_rooms_size * 2) * sizeof(t_room *));
		data->temp_rooms_size *= 2;
		if (!data->temp_rooms)
			error("Memory allocation failed", 0, data);
	}
	return 0;
}

// link two rooms
// returns 1 if there is an error, else 0
int link_rooms(t_room *room1, t_room *room2)
{
	if (room1->num_edges == room1->edge_cap)
	{
		room1->edges = ft_realloc(room1->edges, room1->num_edges * sizeof(t_room *), (room1->edge_cap * 2) * sizeof(t_room *));
		room1->edge_cap *= 2;
		if (!room1->edges)
			return 1;
	}
	room1->edges[room1->num_edges++] = room2;
	return 0;
}
