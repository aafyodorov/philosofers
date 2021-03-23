#include "philo.h"

unsigned long	timeval_to_usec(struct timeval timeval)
{
	return (timeval.tv_sec * 1000000 + timeval.tv_usec);
}

t_writer		*get_sing_writer(void)
{
	static t_writer	writer;
	static int		is_new = 1;

	if (is_new)
	{
		is_new = 0;
		writer.state = 0;
		pthread_mutex_init(&writer.mut, NULL);
	}
	return (&writer);
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
			pthread_mutex_unlock(&monitor->philo[i].take_fork);
		if ((monitor->philo[i].locks | 2))
			pthread_mutex_unlock(monitor->philo[i].fork_l);
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
