#include "libft.h"

static char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*new_s;
	size_t	totsize;

	totsize = ft_strlen(s1) + ft_strlen(s2);
	new_s = malloc(sizeof(char) * (totsize + 1));
	if (!new_s)
		new_s = NULL;
	else
	{
		new_s[0] = 0;
		ft_strlcpy(new_s, s1, totsize + 1);
		ft_strlcat(new_s, s2, totsize + 1);
	}
	if (s1)
	{
		free(s1);
		s1 = NULL;
	}
	return (new_s);
}

static int	read_from_save(int fd, char **save, char **line)
{
	char	*new_save;
	int		reading;

	if (fd < 0)
		return (1);
	reading = 0;
	new_save = ft_strchr(save[fd], '\n');
	if (!new_save)
		reading = 1;
	else
		*new_save = 0;
	*line = ft_strdup(save[fd]);
	if (new_save)
		new_save = ft_strdup(new_save + 1);
	if (save[fd])
		free(save[fd]);
	save[fd] = new_save;
	return (reading);
}

int	get_next_line(int fd, char **line)
{
	char		buff[BUFFER_SIZE + 1];
	static char	*save[1000];
	int			c_readed;
	int			reading;

	reading = read_from_save(fd, save, line);
	while (reading)
	{
		reading = 0;
		c_readed = read(fd, buff, BUFFER_SIZE);
		if (c_readed == 0 && !*line)
			*line = ft_strdup("");
		if (c_readed <= 0)
			return (c_readed);
		buff[c_readed] = 0;
		save[fd] = ft_strchr(buff, '\n');
		if (!save[fd])
			reading = 1;
		else
			*(save[fd]) = 0;
		*line = ft_strjoin_and_free(*line, buff);
		if (save[fd])
			save[fd] = ft_strdup(save[fd] + 1);
	}
	return (1);
}
