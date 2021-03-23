#include "philo.h"
#include "not_libft.h"
#include <unistd.h>

int		main(int argc, char **argv)
{
	t_shared		*data;
	t_monitor		*monitor;

	if (argc > 6 || argc < 5 || !is_correct_input((const char **)(argv + 1),
		argc - 1) || ft_atoi(argv[1]) == 0
		|| alloc_mem(ft_atoi(argv[1]), &data, &monitor))
	{
		write(1, "Error: Invalid arguments or out of memory.\n", 43);
		return (1);
	}
	init_data(ft_atoi(argv[1]), argv + 1, data,
		argc - 1);
	if (init_and_start(monitor, ft_atoi(argv[1])))
		free_res(monitor);
	return (0);
}
