#include "philo.h"

void			take_forks(t_philo *ph)
{
	ph->locks |= 1;
	sem_wait(ph->take_fork);
	ph->locks |= 2;
	sem_wait(ph->forks);
	print_msg(ph, 'f');
	sem_wait(ph->forks);
	print_msg(ph, 'f');
	sem_post(ph->release_fork);
}

void			init_monitor_loop(t_monitor *mon)
{
	struct timeval	time;

	gettimeofday(&mon->data->start_time, NULL);
	gettimeofday(&time, NULL);
	while (diff_timeval_to_usec(&time, &mon->data->start_time) < 100000)
		gettimeofday(&time, NULL);
	gettimeofday(&mon->data->start_time, NULL);
	sem_post(mon->fm_start);
}

unsigned long	get_last_feed(t_monitor *mon, int i)
{
	return (mon->philo[i].last_feed.tv_sec);
}

t_timeval		*get_timestamp(t_philo *philo)
{
	static t_timeval	cur_time;

	sem_wait(philo->timestamp);
	gettimeofday(&cur_time, NULL);
	sem_post(philo->timestamp);
	return (&cur_time);
}
