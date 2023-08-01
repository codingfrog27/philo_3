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
// data = on stack
// philos = heap

#include "philo.h"

static bool	print_args(t_data *data);

// no differentiation between malloc and bad input parse fail, np i think
// inputing meals needed on 0 will ignore the option
int	main(int argc, char **argv)
{
	t_data		data;

	if (argc < 5 || argc > 6 || !data_init(argc, argv, &data) || \
		!philo_init(&data))
		return (printf(C_RED PARSE_ERROR));
	pthread_mutex_init(&data.print_lock, NULL);
	if (!print_args(&data))
		return (printf(C_RED MALLOC_ERROR));
	return (0);
}

bool	start_simulation(t_data *data)
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

static bool	print_args(t_data *data)
{
	printf("deathtime = %i\n there are %i philos\n %i %i %i\n", \
	data->time_till_death, data->nbr_of_philos, \
	data->time_to_eat, data->sleep_time, data->meals_needed);
	return (true);
}
