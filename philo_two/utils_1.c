#include <stdlib.h>
#include "philo.h"
#include "not_libft.h"

static int	is_valid_nbr(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
		{
			return (0);
		}
		str++;
	}
	return (1);
}

int			is_correct_input(const char **str, int len)
{
	int				i;

	i = -1;
	while (++i < len)
	{
		if (!is_valid_nbr(str[i]))
		{
			return (0);
		}
	}
	return (1);
}

int			alloc_mem(int philos, t_shared **data, t_monitor **monitor)
{
	size_t			size;
	void			*mem;

	size = sizeof(t_monitor) + sizeof(t_shared) + philos *
			(sizeof(t_philo) + sizeof(t_philo *) + sizeof(sem_t) +
					sizeof(int));
	mem = malloc(size);
	if (!mem)
		return (1);
	memset(mem, 0, size);
	*data = mem;
	*monitor = mem + sizeof(t_shared);
	(*monitor)->philo = (void *)(*monitor) + sizeof(t_monitor);
	(*monitor)->forks = (void *)((*monitor)->philo) + sizeof(t_philo) * philos;
	(*monitor)->queue = (void *)((*monitor)->forks) + sizeof(sem_t) *
			philos;
	(*monitor)->data = *data;
	return (0);
}

void		free_res(t_monitor *monitor)
{
	free(monitor->data);
}

void		init_data(int numb_of_philo, char **arr, t_shared *data, int argc)
{
	int				tmp;

	data->nof = numb_of_philo;
	tmp = ft_atoi(arr[1]);
	data->time_to_die.tv_sec = tmp / 1000;
	data->time_to_die.tv_usec = tmp % 1000 * 1000;
	tmp = ft_atoi(arr[2]);
	data->time_to_eat.tv_sec = tmp / 1000;
	data->time_to_eat.tv_usec = tmp % 1000 * 1000;
	tmp = ft_atoi(arr[3]);
	data->time_to_sleep.tv_sec = tmp / 1000;
	data->time_to_sleep.tv_usec = tmp % 1000 * 1000;
	data->times_to_fed_up = (argc == 5) ? ft_atoi(arr[4]) : -1;
	data->writer.mut = get_new_sem("writer", 1);
}
