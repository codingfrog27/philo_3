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
void		cleanup_threads_and_end(t_data *data, bool full);
static void	free_data(t_data *data);


// no differentiation between malloc and bad input parse fail, but thats ok
int	main(int argc, char **argv)
{
	t_data		data;
	bool		all_alive;


	if (argc < 5 || argc > 6 || !parsing(&data, argv, argc))
		return (printf(C_RED PARSE_ERROR));
	if (data.nbr_of_philos == 1)
		return (printf(SINGLE_PHILO_ERR));
	if (!philo_init(&data) || !init_all_mutex(&data))
		return (printf(C_RED MALLOC_ERROR));
	start_simulation(&data);
	all_alive = monitor_philos(&data);
	cleanup_threads_and_end(&data, all_alive);
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
}

//sleeps for a moment to let all threads finish exiting
void	cleanup_threads_and_end(t_data *data, bool full)
{
	int	i;
	pthread_mutex_lock(data->death_lock);
	data->end_simulation = true;
	pthread_mutex_unlock(data->death_lock); //change to lock per philo?
	usleep(1000);
	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_join(*data->philo_arr[i]->thread_id, NULL);
		i++;
	}
	free_data(data);
	if (full)
		printf("EVERYONE FULL YIPPIE\n");
}

// do i want ifs at every free for safety? shouldnt be needed tho
static void	free_data(t_data *data)
{
	int		i;
	t_philo	*philo;

	i = 0;
	pthread_mutex_destroy(data->death_lock);
	free(data->death_lock);
	pthread_mutex_destroy(data->print_lock);
	free(data->print_lock);
	while (i < data->nbr_of_philos)
	{
		philo = data->philo_arr[i];
		pthread_mutex_destroy(philo->meal_lock);
		free(philo->meal_lock);
		// pthread_mutex_destroy(philo->left_fork);
		free(philo->left_fork);
		free(philo->thread_id);
		free(philo);
		i++;
	}
	free(data->philo_arr);
	free(data->forks);
}
