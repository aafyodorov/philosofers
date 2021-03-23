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

void		init_and_start(t_monitor *monitor, int len)
{
	pthread_t		mon;
	pthread_t		fm;
	int				i;

	monitor->is_work = 1;
	monitor->data->nof = len;
	monitor->timestamp = get_new_sem("timestamp", 1);
	monitor->forks = get_new_sem("forks", monitor->data->nof);
	monitor->init = get_new_sem("init", 1);
	sem_wait(monitor->init);
	monitor->fm_start = get_new_sem("fm_start", 1);
	sem_wait(monitor->fm_start);
	init_philo(monitor);
	pthread_create(&mon, NULL, monitor_loop, monitor);
	pthread_create(&fm, NULL, food_manager, monitor);
	i = -1;
	while (++i < monitor->data->nof)
		pthread_create(&monitor->philo[i].pthread, NULL, loop,
			&monitor->philo[i]);
	i = -1;
	while (++i < monitor->data->nof)
		pthread_join(monitor->philo[i].pthread, NULL);
	pthread_join(mon, NULL);
}

int			check_alive(t_philo *philo)
{
	int				res;
	struct timeval	time;

	sem_wait(philo->death_when_eat);
	gettimeofday(&time, NULL);
	if ((diff_timeval_to_usec(&time, &philo->last_feed) >
		timeval_to_usec(philo->data->time_to_die)) &&
		philo->last_feed.tv_sec != 0)
	{
		philo->is_alive = 0;
		res = 0;
	}
	else
		res = 1;
	sem_post(philo->death_when_eat);
	return (res);
}

void		print_msg(t_philo *philo, char word)
{
	unsigned long	time;
	const char		*msg;

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
	if (philo->data->writer.state != 1)
	{
		sem_wait(philo->data->writer.mut);
		time = (diff_timeval_to_usec(get_timestamp(philo),
			&philo->data->start_time)) / 1000;
		printf("%lu ms %03d %s\n", time, philo->nbr, msg);
		sem_post(philo->data->writer.mut);
	}
	if (word == 'd')
		philo->data->writer.state = 1;
}

sem_t		*get_new_sem(char *name, int value)
{
	sem_t			*sem;

	sem = sem_open(name, O_CREAT, 0666, value);
	sem_unlink(name);
	return (sem);
}
