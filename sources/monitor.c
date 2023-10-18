/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/29 13:09:22 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/08/29 13:09:22 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <fcntl.h>

//could I just detach at creation and not need this function? is there merit
// to doing it this way?
// static bool	everyone_full(t_data *data);
static bool	is_alive(t_data *data, t_philo *philo);

bool	monitor_philos(t_data *data)
{
	int	i;
	int	full_philos;

	i = 0;
	full_philos = 0;
	while (1)
	{
		while (i < data->nbr_of_philos)
		{
			pthread_mutex_lock(data->philo_arr[i]->meal_lock);
			if (!is_alive(data, data->philo_arr[i]))
				return (false);
			if (data->philo_arr[i]->meals_eaten >= data->meals_needed && \
				data->meals_needed != -1)
				full_philos++;
			pthread_mutex_unlock(data->philo_arr[i]->meal_lock);
			i++;
		}
		if (full_philos == data->nbr_of_philos)
			return (true);
		i = 0;
	}
}

static bool	is_alive(t_data *data, t_philo *philo)
{
	if (time_since_x(philo->last_mealtime) > data->time_till_death)
	{
		pthread_mutex_unlock(philo->meal_lock);
		pthread_mutex_lock(data->death_lock);
		data->end_simulation = true;
		pthread_mutex_unlock(data->death_lock);
		usleep(1000);
		philo_print(philo, death);
		return (false);
	}
	return (true);
}

// static bool	everyone_full(t_data *data)
// {
// 	int	i;
// 	int	full_philos;

// 	i = 0;
// 	full_philos = 0;
// 	while (i < data->nbr_of_philos)
// 	{
// 		if (data->philo_arr[i]->full)
// 			full_philos++;
// 		i++;
// 	}
// 	if (full_philos == data->nbr_of_philos)
// 	{
// 		pthread_mutex_lock(data->death_lock);
// 		data->end_simulation = true;
// 		pthread_mutex_unlock(data->death_lock);
// 		return (true);
// 	}
// 	return (false);
// }
