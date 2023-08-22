/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/09 12:55:20 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/02/07 14:59:51 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "h_colors.h"
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>
# include <sys/time.h>

# define PARSE_ERROR "ERROR, input could not be parsed. Make sure to give 5 or \
6 numeric args\n"

# define MALLOC_ERROR "ERROR occured when allocating memory\n"

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	bool			full;
	long			last_mealtime;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*meal_lock;
	pthread_t		*thread_id;
	t_data			*data;
}				t_philo;

typedef enum e_msg_types
{
	death,
	thinking,
	eating,
	sleeping,
}	t_msg_types;

//also using mealcheck to see if philo is alive + as start lock?
struct s_data
{
	int				nbr_of_philos;
	int				time_till_death;
	int				time_to_eat;
	int				sleep_time;
	int				meals_needed;
	int				full_philos;
	long			start_time;
	bool			all_alive;
	pthread_mutex_t	*death_lock;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	**forks;
	t_philo			**philo_arr;
};

bool	data_init(int argc, char **argv, t_data *data);
bool	philo_init(t_data	*data);
bool	setting_the_table(t_data *data);
bool	starting_threads(t_data *data);
void	*philo_routine(void *para);

//time
long	time_since_x(long start);
long	whattimeisitrightnow(void);
bool	coolsleep(useconds_t sleep_time);
#endif // !PHILO_H
