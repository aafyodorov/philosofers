#include "philo.h"
#include "not_libft.h"

static const char	*g_fork = "has taken a fork";
static const char	*g_eat = "is eating";
static const char	*g_sleep = "is sleeping";
static const char	*g_think = "is thinking";
static const char	*g_died = "died";
static const char	*g_born = "is thinking (born)";

static void	init_philo(t_monitor *mon)
{
	int				i;

	i = -1;
	while (++i < mon->data->nof)
	{
		mon->philo[i].fork_l = (i != mon->data->nof - 1) ? &mon->forks[i] :
			&mon->forks[0];
		pthread_mutex_init(mon->philo[i].fork_l, NULL);
		mon->philo[i].fork_r = (i != mon->data->nof - 1) ? &mon->forks[i + 1] :
			&mon->forks[i];
		pthread_mutex_init(mon->philo[i].fork_r, NULL);
		mon->philo[i].is_alive = 1;
		mon->philo[i].init = &mon->init;
		mon->philo[i].nbr = i + 1;
		mon->philo[i].data = mon->data;
		mon->philo[i].timestamp = &mon->timestamp;
		pthread_mutex_init(&mon->philo[i].death_when_eat, NULL);
		pthread_mutex_init(&mon->philo[i].take_fork, NULL);
		pthread_mutex_lock(&mon->philo[i].take_fork);
		pthread_mutex_init(&mon->philo[i].release_fork, NULL);
		pthread_mutex_lock(&mon->philo[i].release_fork);
	}
}

void		init_and_start(t_monitor *monitor, int len)
{
	pthread_t		mon;
	pthread_t		fm;
	int				i;

	monitor->is_work = 1;
	monitor->data->nof = len;
	pthread_mutex_init(&monitor->init, NULL);
	pthread_mutex_init(&monitor->timestamp, NULL);
	pthread_mutex_lock(&monitor->init);
	pthread_mutex_init(&monitor->fm_start, NULL);
	pthread_mutex_lock(&monitor->fm_start);
	init_queue(monitor);
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

	pthread_mutex_lock(&philo->death_when_eat);
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
	pthread_mutex_unlock(&philo->death_when_eat);
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
	if (get_sing_writer()->state != 1)
	{
		pthread_mutex_lock(&get_sing_writer()->mut);
		time = (diff_timeval_to_usec(get_timestamp(philo),
			&philo->data->start_time)) / 1000;
		printf("%lu ms %03d %s\n", time, philo->nbr, msg);
		pthread_mutex_unlock(&get_sing_writer()->mut);
	}
	if (word == 'd')
		get_sing_writer()->state = 1;
}
