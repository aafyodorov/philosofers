#include "philo.h"

void			init_queue(t_monitor *mon)
{
	int				i;
	int				shift;

	i = 0;
	shift = 0;
	while (i < mon->data->nof)
	{
		mon->queue[shift++] = i;
		i += 2;
	}
	i = 1;
	while (i < mon->data->nof)
	{
		mon->queue[shift++] = i;
		i += 2;
	}
}

void			take_forks(t_philo *ph)
{
	ph->locks |= 1;
	pthread_mutex_lock(&ph->take_fork);
	ph->locks |= 2;
	pthread_mutex_lock(ph->fork_l);
	print_msg(ph, 'f');
	pthread_mutex_lock(ph->fork_r);
	print_msg(ph, 'f');
	pthread_mutex_unlock(&ph->release_fork);
}

void			init_monitor_loop(t_monitor *mon)
{
	struct timeval	time;

	gettimeofday(&mon->data->start_time, NULL);
	gettimeofday(&time, NULL);
	while (diff_timeval_to_usec(&time, &mon->data->start_time) < 100000)
		gettimeofday(&time, NULL);
	gettimeofday(&mon->data->start_time, NULL);
	pthread_mutex_unlock(&mon->fm_start);
}

unsigned long	get_last_feed(t_monitor *mon, int i)
{
	return (mon->philo[i].last_feed.tv_sec);
}

t_timeval		*get_timestamp(t_philo *philo)
{
	static t_timeval	cur_time;

	pthread_mutex_lock(philo->timestamp);
	gettimeofday(&cur_time, NULL);
	pthread_mutex_unlock(philo->timestamp);
	return (&cur_time);
}
