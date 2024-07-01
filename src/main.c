#include "lem-in.h"
#include "get_next_line.h"


// sends error message with line if provided and returns 1
int		error(char *msg, char *line)
{
	if (msg)
		write(1, msg, ft_strlen(msg));
	if (line)
		write(1, line, ft_strlen(line));
	return 1;
}


// room shaped line is "[name(string)] [x(uint)] [y(uint)]"
int	is_room_shaped(char *line)
{
	uint	i = 0;
	uint	space_count = 0;
	uint	x_len = 0;
	uint	y_len = 0;
	uint	name_len = 0;

	while (line[i] && line[i] != '\n')
	{
		if (line[i] == ' ')
			space_count++;
		else if (space_count == 0)
			if (ft_isprint(line[i]))
				name_len++;
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
		i++;
	}
	return (space_count == 2 && name_len && x_len && y_len);
}


// edge shaped line is "[name1(string)]-[name2(string)]"
int	is_edge_shaped(char *line)
{
	uint	i = 0;
	uint	dash_count = 0;
	uint	name1_len = 0;
	uint	name2_len = 0;

	while (line[i] && line[i] != '\n')
	{
		if (line[i] == '-')
			dash_count++;
		else if (dash_count == 0)
			if (ft_isprint(line[i]))
				name1_len++;
		else if (dash_count == 1)
			if (ft_isprint(line[i]))
				name2_len++;
		else
			return 0;
		i++;
	}
	return (dash_count == 1 && name1_len && name2_len);
}


// read the input and store the data (num ants, rooms and edges)
int		get_data(t_data *data)
{
	char	*temp = NULL;
	uint	len;
	uint	modifier = NONE;
	uint	is_first = 1;

	temp = get_next_line(INPUT_FD);
	data->rooms = ft_realloc(data->rooms, 0, (REALLOC_SIZE + 1 ) * sizeof(t_room *));
	while (temp)
	{
		len = ft_strlen(temp);
		if (!ft_strncmp(temp, "#", 1))
		{
			if (!ft_strncmp(temp, "##", 2))
			{
				modifier = ft_strcmp(temp, "##start") * START  + ft_strcmp(temp, "##end") * END;
				if (modifier == NONE)
					return error("Error: invalid directive:\n", temp);
			}
			else
				modifier = NONE;
		}
		else if (is_room_shaped(temp))
		{
			if (is_first)
				return error("Error: first entry must be ants quantity:\n", temp);
			if (data->num_rooms == REALLOC_SIZE)
				data->rooms = ft_realloc(data->rooms, data->num_rooms * sizeof(t_room *), (data->num_rooms + REALLOC_SIZE + 1) * sizeof(t_room *));

			data->rooms[data->num_rooms] = (t_room *)malloc(sizeof(t_room));
			ft_bzero(data->rooms[data->num_rooms], sizeof(t_room));

			if (add_room(data->rooms[data->num_rooms], temp))
				return error("Error: invalid room:\n", temp);

			if (modifier == START)
				data->start = data->rooms[data->num_rooms];
			else if (modifier == END)
				data->end = data->rooms[data->num_rooms];
			modifier = NONE;

			data->num_rooms++;
		}
		else if (is_link_shaped(temp))
		{
			if (is_first)
				return error("Error: first entry must be ants quantity:\n", temp);
			if (modifier)
				return error("Error: link after directive:\n", temp);

			if (add_link(data, temp))
				return error("Error: invalid link:\n", temp);
		}
		else if (is_first)
		{
			if (!ft_isnumber(temp))
				return error("Error: first entry must be ants quantity:\n", temp);
			data->num_ants = ft_atoi(temp);
			is_first = 0;
		}
		else
			return error("Error: invalid line:\n", temp);
	}
	return 0;
}


int main()
{
	t_data	data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if (get_data(&data))
	{
		printf("Error while loading data\n");
		return 1;
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