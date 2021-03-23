#include "philo.h"

unsigned long	timeval_to_usec(struct timeval timeval)
{
	return (timeval.tv_sec * 1000000 + timeval.tv_usec);
}

unsigned long	diff_timeval_to_usec(struct timeval *t1, struct timeval *t2)
{
	unsigned long	usec;
	unsigned long	sec;

	sec = t1->tv_sec - t2->tv_sec;
	usec = t1->tv_usec - t2->tv_usec;
	return (sec * 1000000 + usec);
}

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

pid_t			loop_fork(t_monitor *monitor)
{
	int					i;
	pid_t				pid;
	pthread_t			mon;

	i = -1;
	while (++i < monitor->data->nof)
	{
		pid = fork();
		if (pid == 0)
		{
			monitor->head = i;
			pthread_create(&mon, NULL, monitor_loop, monitor);
			loop(&monitor->philo[i]);
		}
		else
			monitor->pids[i] = pid;
	}
	return (pid);
}

void			dih_sleep(t_monitor *monitor)
{
	t_timeval			cur;
	unsigned long		diff;

	if (monitor->data->must_die)
	{
		usleep(1000);
		return ;
	}
	gettimeofday(&cur, NULL);
	diff = diff_timeval_to_usec(&cur, &monitor->philo[monitor->head].last_feed);
	diff = timeval_to_usec(monitor->data->time_to_die) - diff;
	if (diff > 10000)
		usleep(diff / 2);
}
