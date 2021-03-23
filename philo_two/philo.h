#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <semaphore.h>

typedef struct timeval	t_timeval;

typedef struct {
	int				state;
	sem_t			*mut;
}					t_writer;

typedef struct {
	int				nof;
	int				times_to_fed_up;
	t_timeval		time_to_die;
	t_timeval		time_to_eat;
	t_timeval		time_to_sleep;
	t_timeval		start_time;
	t_writer		writer;
}					t_shared;

typedef struct {
	int				is_alive;
	pthread_t		pthread;
	sem_t			*forks;
	sem_t			*release_fork;
	sem_t			*take_fork;
	t_shared		*data;
	t_timeval		last_feed;
	char			name[4];
	int				nbr;
	int				fed;
	unsigned		locks;
	sem_t			*death_when_eat;
	sem_t			*init;
	sem_t			*timestamp;
}					t_philo;

typedef struct {
	int				head;
	t_shared		*data;
	t_philo			*philo;
	sem_t			*forks;
	int				*queue;
	int				is_work;
	sem_t			*init;
	sem_t			*fm_start;
	sem_t			*timestamp;
}					t_monitor;

int					is_correct_input(const char **str, int len);
int					alloc_mem(int philos, t_shared **data, t_monitor **monitor);
void				init_data(int amount_of_philo, char **arr, t_shared *data,
						int argc);
void				init_and_start(t_monitor *monitor, int len);
void				*loop(void *philo);
void				*monitor_loop(void *monitor);
int					check_alive(t_philo *philo);
void				print_msg(t_philo *philo, char case_);
unsigned long		timeval_to_usec(struct timeval timeval);
void				free_res(t_monitor *monitor);
void				*terminate(t_monitor *monitor);
void				inc_head(t_monitor *monitor);
void				*food_manager(void *monitor);
unsigned long		diff_timeval_to_usec(struct timeval *t1,
						struct timeval *t2);
void				take_forks(t_philo *ph);
void				init_monitor_loop(t_monitor *mon);
unsigned long		get_last_feed(t_monitor *mon, int i);
t_timeval			*get_timestamp(t_philo *philo);
sem_t				*get_new_sem(char *name, int value);
#endif
