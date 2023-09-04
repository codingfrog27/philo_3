/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/21 17:54:52 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/08/21 17:54:52 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool		all_alive_and_hungry(t_philo *philo);
static bool	mealtime(t_philo *philo);

//print lock never gets unlocked on fail, partially on purpose but will also
// cause deadlock if I dont use detach
// TODO ->UNLOCK PRINT LOCK AND MOVE DEATH CHECK TO MONITOR THREAD
bool	philo_print(t_philo *philo, t_msg_types msg_type)
{
	static const char	*msgs[] = {"actually died", "is pondering their orb", \
						"has taken a fork", "is munching", "is catching Z's"};
	static const char	*colours[] = {C_DBLUE, C_LBLUE, C_GREEN, C_YELLOW, \
									C_ORANGE, C_RED};

	if (!all_alive_and_hungry(philo))
		return (false);
	pthread_mutex_lock(philo->data->print_lock);
	if (time_since_x(philo->last_mealtime) > philo->data->time_till_death)
	{
		printf("Philo %i %s\n", philo->id, msgs[0]);
		pthread_mutex_lock(philo->data->death_lock);
		philo->data->end_simulation = true;
		pthread_mutex_unlock(philo->data->death_lock);
		return (false);
	}
	printf("%sPhilo %i %s\n"C_RESET, colours[philo->id % 10], \
			philo->id, msgs[msg_type]);
	pthread_mutex_unlock(philo->data->print_lock);
	return (true);
}

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

void	*philo_routine(void *para)
{
	t_philo	*philo;

	philo = (t_philo *)para;
	pthread_mutex_lock(philo->meal_lock);
	pthread_mutex_unlock(philo->meal_lock);
	if (philo->id % 2)
		coolsleep(100);
	while (1)
	{
		if (!philo_print(philo, thinking) || !mealtime(philo))
			return (NULL);
	}
	return (NULL);
}

// never returns false but needs to be void anyways
static bool	mealtime(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	philo_print(philo, grabbing_fork);
	pthread_mutex_lock(philo->right_fork);
	philo_print(philo, grabbing_fork);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	philo_print(philo, eating);
	return (true);
}
