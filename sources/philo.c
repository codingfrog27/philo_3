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

bool	philo_print(t_philo *philo, t_msg_types msg_type)
{
	const char	*msgs[] = {"actually died", "is pondering their orb", \
						"is munching", "is catching Z's"};
	static int	rainbow_counter;
	const char	*colours[] = {C_DBLUE, C_LBLUE, C_GREEN, C_YELLOW, C_ORANGE, \
								C_RED};

	pthread_mutex_lock(philo->data->death_lock);
	if (!philo->data->all_alive)
	{
		pthread_mutex_unlock(philo->data->death_lock);
		return (false);
	}
	pthread_mutex_unlock(philo->data->death_lock);
	pthread_mutex_lock(philo->print_lock);
	if (time_since_x(philo->last_mealtime) > philo->data->time_till_death)
	{
		printf("Philo %i %s\n", philo->id, msgs[0]);
		pthread_mutex_lock(philo->data->death_lock);
		philo->data->all_alive = false;
		pthread_mutex_unlock(philo->data->death_lock);
		return (false);
	}
	printf("%sPhilo %i %s\n"C_RESET, colours[rainbow_counter + philo->id], \
			philo->id, msgs[msg_type]);
	return (true);
} //idk how to structure these locks tbh

void	*philo_routine(void *para)
{
	t_philo	*philo;

	philo = (t_philo *)para;
	pthread_mutex_lock(philo->meal_lock);
	pthread_mutex_unlock(philo->meal_lock);
	if (philo->id % 2)
		coolsleep(250);
	while (1)
	{
		if (!philo_print(philo, thinking))
			return (NULL); //not sure what to do when philo dies yet
	}
	return (NULL);
}
