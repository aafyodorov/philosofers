#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <semaphore.h>
# include <sys/types.h>
# include <signal.h>
# include <stdlib.h>

typedef struct timeval	t_timeval;

typedef struct {
	int				nof;
	int				times_to_fed_up;
	int				must_die;
	t_timeval		time_to_die;
	t_timeval		time_to_eat;
	t_timeval		time_to_sleep;
	t_timeval		start_time;
	sem_t			*writer;
}					t_shared;

typedef struct {
	int				is_alive;
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
	sem_t			*fm_start;
}					t_philo;

typedef struct {
	int				head;
	t_shared		*data;
	t_philo			*philo;
	sem_t			*forks;
	int				*queue;
	int				is_work;
	int				is_fed_up;
	sem_t			*init;
	sem_t			*fm_start;
	sem_t			*timestamp;
	sem_t			*sim_stop;
	pid_t			*pids;
}					t_monitor;

int					is_correct_input(const char **str, int len);
int					alloc_mem(int philos, t_shared **data, t_monitor **monitor);
void				init_data(int amount_of_philo, char **arr, t_shared *data,
						int argc);
pid_t				init_and_start(t_monitor *monitor, int len);
void				*loop(void *philo);
void				*monitor_loop(void *monitor);
int					check_alive(t_philo *philo);
void				print_msg(t_philo *philo, char case_);
unsigned long		timeval_to_usec(struct timeval timeval);
void				free_res(t_monitor *monitor);
void				*food_manager(void *monitor);
unsigned long		diff_timeval_to_usec(struct timeval *t1,
						struct timeval *t2);
void				take_forks(t_philo *ph);
sem_t				*get_new_sem(char *name, int value);
pid_t				loop_fork(t_monitor *monitor);
void				dih_sleep(t_monitor *monitor);
#endif
