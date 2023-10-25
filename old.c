/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   old.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/11 17:46:13 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/09/11 17:46:13 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */



//print lock never gets unlocked on fail, partially on purpose but will also
// cause deadlock if I dont use detach
// TODO ->UNLOCK PRINT LOCK AND MOVE DEATH CHECK TO MONITOR THREAD
bool	philo_print(t_philo *philo, t_msg_types msg_type)
{
	static const char	*msgs[] = {C_PINK"actually died", C_LCYAN"is pondering their orb", \
						C_LVIOLET"has taken a fork", C_CHRT"is munching", C_SPRGR"is catching Z's"};
	static const char	*colours[] = {C_DBLUE, C_LBLUE, C_GREEN, C_YELLOW, \
									C_ORANGE, C_RED};
	static const char	*activity_colours[] = {C_PINK, C_LCYAN, C_LVIOLET, \
											C_CHRT, C_SPRGR, C_DBLUE};

	if (!all_alive_and_hungry(philo))
		return (false);
	pthread_mutex_lock(philo->data->print_lock);
	printf("%s%li Philo %i %s\n"C_RESET, colours[philo->id % 5], \
	time_since_x(0), philo->id, msgs[msg_type]);
	pthread_mutex_unlock(philo->data->print_lock);
	return (true);
}
// activity_colours[msg_type]

static bool	print_args(t_data *data)
{
	printf("deathtime = %li\n there are %li philos\n %li %li %li\n", \
	data->time_till_death, data->nbr_of_philos, \
	data->time_to_eat, data->sleep_time, data->meals_needed);
	return (true);
}

static const char	*no_colour_msgs[] = {"died", "is thinking", \
						"has taken a fork", "is eating", \
						"is sleeping"};


//do I wanna change this lock to the meal check lock? or another individual lock
// ? Less bottleneck but also its 1 if check and more delay when philo dies
bool	all_alive_and_hungry(t_philo *philo)
{
	bool	continue_sim;

	continue_sim = true;
	pthread_mutex_lock(philo->data->death_lock);
	if (philo->data->end_simulation)
		continue_sim = false;
	pthread_mutex_unlock(philo->data->death_lock);
	return (continue_sim);
}

// from monitoring.c

	// int fd = open("test.txt", O_RDWR);
			// printf("%i last_mealtime == %li\t deathtime == %li\n", \
			// data->philo_arr[i]->id,time_since_x(data->philo_arr[i]->\
			// last_mealtime), data->time_till_death);
				// dprintf(fd, "philo %i last mealtime was %li but deathtime is %li"\
				// , data->philo_arr[i]->id, data->philo_arr[i]->last_mealtime, \
				// data->time_till_death);
