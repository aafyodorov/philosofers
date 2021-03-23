#include <unistd.h>
#include "philo.h"

#include "not_libft.h"

void	*loop(void *philo)
{
	t_philo			*ph;

	ph = philo;
	sem_post(ph->fm_start);
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

	mon = monitor;
	while (1)
	{
		if (mon->philo[mon->head].last_feed.tv_sec != 0 &&
				!check_alive(&mon->philo[mon->head]))
		{
			print_msg(mon->philo + mon->head, 'd');
			i = -1;
			while (++i < mon->data->nof)
				sem_post(mon->sim_stop);
			return (NULL);
		}
		if (!mon->is_fed_up && mon->data->times_to_fed_up != -1 &&
				mon->philo[mon->head].fed >= mon->data->times_to_fed_up)
		{
			mon->is_fed_up = 1;
			sem_post(mon->sim_stop);
		}
		if (mon->philo[mon->head].last_feed.tv_sec)
			dih_sleep(mon);
	}
}

void	*food_manager(void *mon)
{
	t_monitor		*monitor;
	int				cur;

	monitor = mon;
	cur = -1;
	while (++cur < monitor->data->nof)
		sem_wait(monitor->fm_start);
	cur = -1;
	while (++cur < monitor->data->nof)
		sem_post(monitor->philo[cur].init);
	cur = -1;
	while (1)
	{
		cur = (cur + 1) % monitor->data->nof;
		sem_post(monitor->philo[cur].take_fork);
		monitor->philo[cur].locks &= ~1;
		sem_wait(monitor->philo[cur].release_fork);
	}
}
