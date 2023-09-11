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

//could I just detach at creation and not need this function? is there merit
// to doing it this way?
void	cleanup_threads_and_end(t_data *data);
bool	everyone_full(t_data *data);

void	monitor_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_lock(data->philo_arr[i]->meal_lock);
		if (data->philo_arr[i]->last_mealtime > data->time_till_death)
		{
			pthread_mutex_lock(data->death_lock);
			data->end_simulation = true;
			pthread_mutex_unlock(data->death_lock);
			pthread_mutex_unlock(data->philo_arr[i]->meal_lock);
			return (cleanup_threads_and_end(data));
		}
		if (data->philo_arr[i]->meals_eaten >= data->meals_needed)
			data->philo_arr[i]->full = true;
		pthread_mutex_unlock(data->philo_arr[i]->meal_lock);
		i++;
	}
	i = 0;
	if (everyone_full(data))
		return (cleanup_threads_and_end(data));
}

void	cleanup_threads_and_end(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_join(*data->philo_arr[i]->thread_id, NULL);
		i++;
	}
}

bool	everyone_full(t_data *data)
{
	int	i;
	int	full_philos;

	i = 0;
	full_philos = 0;
	while (i < data->nbr_of_philos)
	{
		if (data->philo_arr[i]->full)
			full_philos++;
		i++;
	}
	if (full_philos == data->nbr_of_philos)
	{
		pthread_mutex_lock(data->death_lock);
		data->end_simulation = true;
		pthread_mutex_unlock(data->death_lock);
		return (true);
	}
	return (false);
}
