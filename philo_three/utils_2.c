#include "philo.h"
#include "not_libft.h"
#include <fcntl.h>

static const char	*g_fork = "has taken a fork";
static const char	*g_eat = "is eating";
static const char	*g_sleep = "is sleeping";
static const char	*g_think = "is thinking";
static const char	*g_died = "died";
static const char	*g_born = "is thinking (born)";
static const char	*g_chars = "0123456789ABCDEF";

static void	init_philo(t_monitor *mon)
{
	int				i;

	i = -1;
	while (++i < mon->data->nof)
	{
		mon->philo[i].forks = mon->forks;
		mon->philo[i].is_alive = 1;
		mon->philo[i].init = mon->init;
		mon->philo[i].nbr = i + 1;
		mon->philo[i].data = mon->data;
		mon->philo[i].timestamp = mon->timestamp;
		mon->philo[i].fm_start = mon->fm_start;
		mon->philo[i].name[0] = 'f';
		mon->philo[i].name[1] = g_chars[(mon->philo[i].nbr - 1) / 16];
		mon->philo[i].name[2] = g_chars[(mon->philo[i].nbr - 1) % 16];
		mon->philo[i].death_when_eat = get_new_sem(mon->philo[i].name, 1);
		mon->philo[i].name[0] = 't';
		mon->philo[i].take_fork = get_new_sem(mon->philo[i].name, 1);
		sem_wait(mon->philo[i].take_fork);
		mon->philo[i].name[0] = 'r';
		mon->philo[i].release_fork = get_new_sem(mon->philo[i].name, 1);
		sem_wait(mon->philo[i].release_fork);
	}
}

pid_t		init_and_start(t_monitor *monitor, int len)
{
	pthread_t		fm;
	int				i;
	pid_t			pid;

	monitor->is_work = 1;
	monitor->data->nof = len;
	monitor->timestamp = get_new_sem("timestamp", 1);
	monitor->forks = get_new_sem("forks", monitor->data->nof);
	monitor->init = get_new_sem("init", 0);
	monitor->fm_start = get_new_sem("fm_start", 0);
	monitor->sim_stop = get_new_sem("sim_stop", 0);
	init_philo(monitor);
	gettimeofday(&monitor->data->start_time, NULL);
	pid = loop_fork(monitor);
	if (pid > 0)
	{
		pthread_create(&fm, NULL, food_manager, monitor);
		i = -1;
		while (++i < monitor->data->nof)
			sem_wait(monitor->sim_stop);
		i = -1;
		while (++i < monitor->data->nof)
			kill(monitor->pids[i], SIGKILL);
	}
	return (pid);
}

int			check_alive(t_philo *philo)
{
	int				res;
	struct timeval	time;

	sem_wait(philo->death_when_eat);
	gettimeofday(&time, NULL);
	sem_post(philo->death_when_eat);
	if ((diff_timeval_to_usec(&time, &philo->last_feed) >
		timeval_to_usec(philo->data->time_to_die)) &&
		philo->last_feed.tv_sec != 0)
	{
		philo->is_alive = 0;
		res = 0;
	}
	else
		res = 1;
	return (res);
}

void		print_msg(t_philo *philo, char word)
{
	unsigned long	time;
	const char		*msg;
	t_timeval		cur;

	if (word == 'f')
		msg = g_fork;
	else if (word == 'e')
		msg = g_eat;
	else if (word == 's')
		msg = g_sleep;
	else if (word == 't')
		msg = g_think;
	else if (word == 'd')
		msg = g_died;
	else if (word == 'b')
		msg = g_born;
	sem_wait(philo->data->writer);
	gettimeofday(&cur, NULL);
	time = (diff_timeval_to_usec(&cur, &philo->data->start_time) + 500) / 1000;
	printf("%lu ms %03d %s\n", time, philo->nbr, msg);
	fflush(stdout);
	if (word != 'd')
		sem_post(philo->data->writer);
	else
		return ;
}

sem_t		*get_new_sem(char *name, int value)
{
	sem_t			*sem;

	sem = sem_open(name, O_CREAT, 0666, value);
	sem_unlink(name);
	return (sem);
}
