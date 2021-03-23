#include <unistd.h>
#include "philo.h"

#include "not_libft.h"

void	*loop(void *philo)
{
	t_philo			*ph;

	ph = philo;
	sem_wait(ph->init);
	sem_post(ph->init);
	print_msg(philo, 'b');
	gettimeofday(&ph->last_feed, NULL);
	while (ph->is_alive)
	{
		take_forks(ph);
		sem_wait(ph->death_when_eat);
		gettimeofday(&ph->last_feed, NULL);
		sem_post(ph->death_when_eat);
		print_msg(philo, 'e');
		++(ph->fed);
		usleep(timeval_to_usec(ph->data->time_to_eat));
		sem_post(ph->forks);
		sem_post(ph->forks);
		ph->locks &= ~2;
		print_msg(ph, 's');
		usleep(timeval_to_usec(ph->data->time_to_sleep));
		print_msg(ph, 't');
	}
	return (NULL);
}

void	*monitor_loop(void *monitor)
{
	t_monitor		*mon;
	int				i;
	int				well_fed;

	mon = monitor;
	init_monitor_loop(mon);
	while (1)
	{
		i = -1;
		well_fed = 0;
		while (++i < mon->data->nof)
		{
			if (get_last_feed(mon, i) != 0 && !check_alive(&mon->philo[i]))
			{
				print_msg(mon->philo + i, 'd');
				return (terminate(mon));
			}
			if (mon->philo[i].fed >= mon->data->times_to_fed_up)
				++well_fed;
		}
		if (mon->data->times_to_fed_up != -1 && well_fed >= mon->data->nof)
			return (terminate(mon));
	}
}

void	*food_manager(void *mon)
{
	t_monitor		*monitor;
	int				cur;

	monitor = mon;
	sem_wait(monitor->fm_start);
	cur = -1;
	while (++cur < monitor->data->nof)
		sem_post(monitor->philo[monitor->queue[cur]].init);
	cur = -1;
	while (monitor->is_work)
	{
		cur = (cur + 1) % monitor->data->nof;
		sem_post(monitor->philo[cur].take_fork);
		monitor->philo[cur].locks &= ~1;
		sem_wait(monitor->philo[cur].release_fork);
		inc_head(monitor);
	}
	return (NULL);
}
