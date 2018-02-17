#include <pt.h>

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	int		count_zeros = 0;

	if (argc != 2 || (fd = open(argv[1], O_RDONLY)) == -1)
	{
		ft_printf("usage: %{rd}%s%{nc} \"filepath\"\n", argv[0]);
		exit(0);
	}
	while (get_next_line(fd, &line) > 0)
	{
		if (ft_strstr("0.000  0.000  0.000", line))
			++count_zeros;
		else
			ft_printf("%s\n", line);
		ft_strdel(&line);
	}
	ft_printf("number of lines == 0 is %d\n", count_zeros);
	return (0);
}
