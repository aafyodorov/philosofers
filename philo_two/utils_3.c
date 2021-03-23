#include "philo.h"

unsigned long	timeval_to_usec(struct timeval timeval)
{
	return (timeval.tv_sec * 1000000 + timeval.tv_usec);
}

void			*terminate(t_monitor *monitor)
{
	int				i;

	monitor->is_work = 0;
	i = -1;
	while (++i < monitor->data->nof)
	{
		monitor->philo[i].is_alive = 0;
		if ((monitor->philo[i].locks | 1))
			sem_post(monitor->philo[i].take_fork);
		if ((monitor->philo[i].locks | 2))
			sem_post(monitor->philo[i].forks);
	}
	return (NULL);
}

void			inc_head(t_monitor *monitor)
{
	int				next;

	next = ++(monitor->head) % monitor->data->nof;
	monitor->head = next;
}

unsigned long	diff_timeval_to_usec(struct timeval *t1, struct timeval *t2)
{
	unsigned long	usec;
	unsigned long	sec;

	sec = t1->tv_sec - t2->tv_sec;
	usec = t1->tv_usec - t2->tv_usec;
	return (sec * 1000000 + usec);
}
