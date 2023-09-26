/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 17:33:52 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/07/24 17:33:52 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// mem management notes
// on heap = philos, fork array, pthread ptrs all locks
// forks are a mutex array in data since it saves me from having to allocate
// each fork seperately and they should only be used by the philo threads
// anyways && // currently not freeing old data in parsing if malloc fails
// cuz im exiting anyways

#include "philo.h"

static void	start_simulation(t_data *data);


// static bool	print_args(t_data *data);

// no differentiation between malloc and bad input parse fail, but thats ok
int	main(int argc, char **argv)
{
	t_data		data;

	if (argc < 5 || argc > 6 || !parsing(&data, argv, argc))
		return (printf(C_RED PARSE_ERROR));
	if (!philo_init(&data) || !init_all_mutex(&data))
		return (printf(C_RED MALLOC_ERROR));
	start_simulation(&data);
	return (0);
}

static void	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_lock(data->philo_arr[i]->meal_lock);
		pthread_create(data->philo_arr[i]->thread_id, NULL, philo_routine, \
		(void *)data->philo_arr[i]); //protect!
		i++;
	}
	i--;
	data->start_time = timestamp();
	while (i >= 0)
	{
		data->philo_arr[i]->last_mealtime = data->start_time;
		pthread_mutex_unlock(data->philo_arr[i]->meal_lock);
		i--;
	}
	monitor_philos(data);
}


// bool	start_simulation(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->nbr_of_philos)
// 	{
// 		if (pthread_create(data->thread_id[i], NULL, philo_routine,
// 		(void *)&data->philo_arr[i]) != 0)
// 			return (join_threads_and_end(data, i));
// 		i++;
// 	}
// 	pthread_mutex_unlock(&data->print_lock);
// 	return (monitor_in_parent_thread(data));
// }
