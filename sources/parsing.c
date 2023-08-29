/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/24 17:34:42 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/07/24 17:34:42 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	parsing(t_data *data, char **argv, int argc);
static int	philatoi(char *str);
bool		philo_init(t_data	*data);
static bool	init_all_mutex(t_data *data);

//pthread create not protected atm
bool	data_init(int ac, char **av, t_data *data)
{
	int	i;

	i = 0;
	if (!parsing(data, av, ac) || !philo_init(data) || !init_all_mutex(data))
		return (false);
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_lock(data->philo_arr[i]->meal_lock);
		pthread_create(data->philo_arr[i]->thread_id, NULL, philo_routine, \
		(void *)data->philo_arr[i]); //protection?
		i++;
	}
	i--;
	while (i >= 0)
	{
		pthread_mutex_unlock(data->philo_arr[i]->meal_lock);
		i--;
	}
	printf("hello?");
	monitor_philos(data);
	return (true);
}

//so sad C doesn't allow you to while loop through struct members
static bool	parsing(t_data *data, char **argv, int argc)
{
	int	i;

	i = 1;
	data->nbr_of_philos = philatoi(argv[i++]);
	data->time_till_death = philatoi(argv[i++]);
	data->time_to_eat = philatoi(argv[i++]);
	data->sleep_time = philatoi(argv[i++]);
	data->end_simulation = false;
	data->full_philos = 0;
	data->meals_needed = -1;
	if (argc == 6)
		data->meals_needed = philatoi(argv[i]);
	if (data->nbr_of_philos == 0 || data->time_till_death == 0 || \
	data->time_to_eat == 0 || data->sleep_time == 0 || data->meals_needed == 0)
		return (false);
	return (true);
}

bool	philo_init(t_data	*data)
{
	int				i;
	t_philo			**philos;
	pthread_t		*thread;

	i = 0;
	philos = malloc(sizeof(t_philo *) * (data->nbr_of_philos));
	if (!philos)
		return (false);
	while (i < data->nbr_of_philos)
	{
		philos[i] = malloc(sizeof(t_philo));
		thread = malloc(sizeof(pthread_t));
		if (!philos[i] || !thread)
			return (false);
		philos[i]->thread_id = thread;
		philos[i]->id = i + 1;
		philos[i]->meals_eaten = 0;
		philos[i]->full = false;
		philos[i]->last_mealtime = 0;
		philos[i]->data = data;
		i++;
	}
	data->philo_arr = philos;
	return (true);
}

/**
 * @brief just a small atoi for philo (libft not allowed)
 * since only positive nbrs are allowed for input >0 can be error return
 * So it might look like it misses checks but I think the isnum is sufficient?
 * @param str
 * @return converted int
 */
static int	philatoi(char *str)
{
	long	ret;
	int		i;

	ret = 0;
	i = 0;
	while (str[i])
	{
		ret = (ret * 10) + (str[i] - '0');
		if (str[i] < '0' || str[i] > '9' || ret > INT_MAX)
			return (0);
		i++;
	}
	return ((int)ret);
}

static bool	init_all_mutex(t_data *data)
{
	int		i;

	i = 0;
	data->death_lock = malloc(sizeof(pthread_mutex_t));
	data->forks = malloc(sizeof(pthread_mutex_t *) * (data->nbr_of_philos - 1));
	data->print_lock = malloc(sizeof(pthread_mutex_t));
	if (!data->death_lock || !data->forks)
		return (false);
	pthread_mutex_init(data->print_lock, NULL);
	pthread_mutex_init(data->death_lock, NULL);
	while (i < data->nbr_of_philos)
	{
		data->forks[i] = malloc(sizeof(pthread_mutex_t));
		data->philo_arr[i]->meal_lock = malloc(sizeof(pthread_mutex_t));
		if (!data->forks[i] || !data->philo_arr[i]->meal_lock)
			return (false);
		pthread_mutex_init(data->forks[i], NULL);
		pthread_mutex_init(data->philo_arr[i]->meal_lock, NULL);
		data->philo_arr[i]->left_fork = data->forks[i];
		i++;
	}
	return (true);
}
