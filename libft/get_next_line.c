#include "get_next_line.h"

char *ft_strchrr(char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return (s);
		s++;
	}
	return (NULL);
}

size_t ft_strlenn(const char *s)
{
	size_t i = 0;

	while (s[i])
		i++;
	return (i);
}

void ft_strcpy(char *dst, const char *src)
{
	while (*src)
		*dst++ = *src++;
	*dst = '\0';
}

char *ft_strdup(const char *src)
{
	size_t len = ft_strlenn(src) + 1;
	char *dst = malloc(len);

	if (dst == NULL)
		return (NULL);
	ft_strcpy(dst, src);
	return (dst);
}

char *ft_strjoin(char *s1, char *s2)
{
	char *join;

	if (!s1 || !s2)
		return (NULL);
	join = malloc(ft_strlenn(s1) + ft_strlenn(s2) + 1);
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy(join + ft_strlenn(s1), s2);
	free(s1);
	return (join);
}

char *get_next_line(int fd)
{
	static char buf[BUFFER_SIZE + 1];
	char *line;
	char *newline;
	int countread;

	line = ft_strdup(buf);
	while (!(newline = ft_strchrr(line, '\n')) && (countread = read(fd, buf, BUFFER_SIZE)))
	{
		buf[countread] = '\0';
		line = ft_strjoin(line, buf);
	}
	if (ft_strlenn(line) == 0)
		return (free(line), NULL);

	if (newline)
	{
		ft_strcpy(buf, newline + 1);
		*(newline + 1) = '\0';
	}
	else
		buf[0] = '\0';

	return (line);
}