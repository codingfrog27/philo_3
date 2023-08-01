/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/05 16:27:38 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/06/05 16:27:38 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->data->print_lock);
}

int	join_threads_and_end(t_data *data, int i)
{
	while (i)
	{
		pthread_join(&data->thread_ids[i], NULL);
		i--;
	}
	free(data->thread_ids);
}

void	philo_has_died(t_data *data, int dead_philo_nbr)
{
	printf("philo %i has died", dead_philo_nbr);
	join_threads_and_end(data, data->nbr_of_philos);
}

int	monitor_in_parent_thread(t_data *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->print_lock); //mb make a seperate death lock?
	while(i < data->nbr_of_philos)
	{
		if (data->philo_arr[i].last_mealtime > data->time_till_death)
			philo_has_died(data, i);
	}
	pthread_mutex_unlock(&data->print_lock);
}

void	start_simulation(t_data *data)
{
	int	i;
	i = 0;
	pthread_mutex_lock(&data->print_lock);
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(data->thread_ids[i], NULL, philo_routine, \
		(void *)&data->philo_arr[i]) != 0)
			return (join_threads_and_end(data, i));
		i++;
	}
	pthread_mutex_unlock(&data->print_lock);
	return (monitor_in_parent_thread(data));
}

int	main(int argc, char **argv)
{
	t_data		data;

	if (!data_init(argc, argv, &data))
		return (printf(C_RED"wrong input buddy"));
	data.philo_arr = philo_init(&data);
	data.thread_ids = malloc(sizeof(pthread_t) * data.nbr_of_philos);
	if (!data.thread_ids)
		return (printf(C_RED"malloc fail"));
	pthread_mutex_init(&data.print_lock, NULL);
	// alloc+init fork mutexes, might move to own function
	return (start_simulation(&data));
}
