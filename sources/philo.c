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
static void	mealtime(t_philo *philo);
static void	update_last_mealtime(t_philo *philo);

// bool	colour_philo_print(t_philo *philo, t_msg_types msg_type)
// {
// 	static const char	*msgs[] = {C_RED"died", C_LCYAN"is thinking",
// 						C_LVIOLET"has taken a fork", C_CHRT"is eating",
// 						C_SPRGR"is sleeping"};
// 	static const char	*colours[] = {C_DBLUE, C_LBLUE, C_GREEN, C_YELLOW,
// 									C_ORANGE, C_PINK};

bool	philo_print(t_philo *philo, t_msg_types msg_type)
{
	static const char	*msgs[] = {"died", "is thinking", \
						"has taken a fork", "is eating", \
						"is sleeping"};
	bool				alive;

	pthread_mutex_lock(philo->philo_lock);
	alive = philo->alive;
	pthread_mutex_unlock(philo->philo_lock);
	if (!alive && msg_type != death)
		return (false);
	pthread_mutex_lock(philo->data->print_lock);
	printf("%li %i %s\n"C_RESET, time_since_start(philo->data), \
	philo->id, msgs[msg_type]);
	pthread_mutex_unlock(philo->data->print_lock);
	return (true);
}

void	*philo_routine(void *para)
{
	t_philo			*philo;
	pthread_mutex_t	*tmp_fork;

	tmp_fork = NULL;
	philo = (t_philo *)para;
	pthread_mutex_lock(philo->philo_lock);
	pthread_mutex_unlock(philo->philo_lock);
	if (philo->id % 2)
	{
		tmp_fork = philo->left_fork;
		philo->left_fork = philo->right_fork;
		philo->right_fork = tmp_fork;
		philo_sleep(philo->data->time_to_eat / 2, philo);
	}
	while (1)
	{
		if (!philo_print(philo, thinking))
			return (NULL);
		mealtime(philo);
		if (!philo_print(philo, sleeping))
			return (NULL);
		if (!philo_sleep(philo->data->sleep_time, philo))
			return (NULL);
	}
	return (NULL);
}

static void	mealtime(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	philo_print(philo, grabbing_fork);
	pthread_mutex_lock(philo->right_fork);
	philo_print(philo, grabbing_fork);
	update_last_mealtime(philo);
	philo_print(philo, eating);
	philo_sleep(philo->data->time_to_eat, philo);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	update_last_mealtime(t_philo *philo)
{
	pthread_mutex_lock(philo->philo_lock);
	philo->last_mealtime = timestamp();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->philo_lock);
}
