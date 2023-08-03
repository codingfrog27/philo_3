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

static bool	print_args(t_data *data);

// no differentiation between malloc and bad input parse fail, np i think
int	main(int argc, char **argv)
{
	t_data		data;

	if (argc < 5 || argc > 6 || !data_init(argc, argv, &data))
		return (printf(C_RED PARSE_ERROR));
	if (!print_args(&data))
		return (printf(C_RED MALLOC_ERROR));
	return (0);
}

// bool	start_simulation(t_data *data)
// {
// 	int	i;

// 	i = 0;
// 	while (i < data->nbr_of_philos)
// 	{
// 		if (pthread_create(data->thread_ids[i], NULL, philo_routine, \
// 		(void *)&data->philo_arr[i]) != 0)
// 			return (join_threads_and_end(data, i));
// 		i++;
// 	}
// 	pthread_mutex_unlock(&data->print_lock);
// 	return (monitor_in_parent_thread(data));
// }

static bool	print_args(t_data *data)
{
	printf("deathtime = %i\n there are %i philos\n %i %i %i\n", \
	data->time_till_death, data->nbr_of_philos, \
	data->time_to_eat, data->sleep_time, data->meals_needed);
	return (true);
}
