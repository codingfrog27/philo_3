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

static bool	is_alive_monitor(t_data *data, t_philo *philo);

bool	monitor_philos(t_data *data)
{
	int	i;
	int	full_philos;

	i = 0;
	full_philos = 0;
	usleep(1000);
	while (1)
	{
		while (i < data->nbr_of_philos)
		{
			pthread_mutex_lock(data->philo_arr[i]->philo_lock);
			if (!is_alive_monitor(data, data->philo_arr[i]))
				return (false);
			if (data->philo_arr[i]->meals_eaten >= data->meals_needed && \
				data->meals_needed != -1)
				full_philos++;
			pthread_mutex_unlock(data->philo_arr[i]->philo_lock);
			i++;
		}
		if (full_philos == data->nbr_of_philos)
			return (true);
		i = 0;
		full_philos = 0;
		usleep(1000);
	}
}

static bool	is_alive_monitor(t_data *data, t_philo *philo)
{
	if (time_since_x(philo->last_mealtime) > data->time_till_death)
	{
		philo->alive = false;
		pthread_mutex_unlock(philo->philo_lock);
		// kill_everyone(data);
		// usleep(1000);
		pthread_mutex_lock(data->print_lock);
		printf("%li %i died\n", time_since_start(philo->data), \
		philo->id);
		data->all_alive = false;
		pthread_mutex_unlock(data->print_lock);
		return (false);
	}
	return (true);
}

bool	is_alive(t_philo *philo)
{
	bool	alive;

	alive = true;
	pthread_mutex_lock(philo->philo_lock);
	if (time_since_x(philo->last_mealtime) > philo->data->time_till_death)
	{
		philo->alive = false;
		alive = false;
	}
	pthread_mutex_unlock(philo->philo_lock);
	return (alive);
}

void	kill_everyone(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(data->print_lock);
	data->all_alive = false;
	pthread_mutex_unlock(data->print_lock);
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_lock(data->philo_arr[i]->philo_lock);
		data->philo_arr[i]->alive = false;
		pthread_mutex_unlock(data->philo_arr[i]->philo_lock);
		i++;
	}
}
