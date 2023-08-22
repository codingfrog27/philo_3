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
	const char	*colours[] = {C_DBLUE, C_LBLUE, C_GREEN, C_YELLOW, C_ORANGE, \
								C_RED};

	pthread_mutex_lock(philo->data->death_lock);
	if (!philo->data->all_alive)
		return (false);
	pthread_mutex_unlock(philo->data->death_lock);
	pthread_mutex_lock(philo->data->print_lock);
	if (time_since_x(philo->last_mealtime) > philo->data->time_till_death)
	{
		printf("Philo %i %s\n", philo->id, msgs[0]);
		pthread_mutex_lock(philo->data->death_lock);
		philo->data->all_alive = false;
		return (false);
	}
	printf("%sPhilo %i %s\n"C_RESET, colours[philo->id % 10], \
			philo->id, msgs[msg_type]);
	pthread_mutex_unlock(philo->data->print_lock);
	return (true);
}

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
		{
			pthread_mutex_unlock(philo->data->death_lock);
			return (NULL);
		}
	}
	return (NULL);
}
