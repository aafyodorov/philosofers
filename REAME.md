### Description:
My solutions to the [dining philosophers problem][1]. There are three solutions:
1. Using threads and mutexes:
    * One fork between each philosopher, therefore there will be a fork at the right and at the left of each 
      philosopher.
    * Each philosopher is a thread.
2. Using threads and semaphores:
    * All the forks are in the middle of the table.
    * Each philosopher is a thread.
3. Using processes and semaphores:
    * All the forks are in the middle of the table.
    * Each philosopher is a process, and the main process is not a philosopher.

### How to run:
1. Use the `make` command in each directory. <br>
2. Each program requires have the same options: `number_of_philosophers` `time_to_die` `time_to_eat` `time_to_sleep` 
   `number_of_times_each_philosopher_must_eat`. Last argument is optional.
    * `number_of_philosophers` - is the number of philosophers and also the number of forks.
    * `time_to_die` - is in milliseconds, if a philosopher doesn't start eating `time_to_die` milliseconds after 
      starting his last meal or the beginning of the simulation, it dies.
    * `time_to_eat` - is in milliseconds and is the time it takes for a philosopher to eat. During that time he will
      need to keep the two forks.
    * `time_to_sleep` - is in milliseconds and is the time the philosopher will spend sleeping.
    * `number_of_times_each_philosopher_must_eat` - argument is optional, if all philosophers eat at least 
      `number_of_times_each_philosopher_must_eat` the simulation will stop. If not specified, the simulation will 
      stop only at the death of a philosopher.

#### Example:
`./philo_one 4 410 200 200` - the four philosophers will eat for 200ms, then they will sleep for 200ms. Every
philosopher can 'live' without foot 410ms, the simulation never stops.

[1]: https://en.wikipedia.org/wiki/Dining_philosophers_problem
