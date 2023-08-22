/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/10 17:40:45 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/02/07 15:01:16 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*philo_init(t_data	*data)
{
	int				i;
	t_philo			*philos;

	i = 0;
	philos = malloc(sizeof(t_philo) * (data->nbr_of_philos));
	if (!philos)
		return (NULL);
	while (i < data->nbr_of_philos)
	{
		philos[i].id = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].full = false;
		philos[i].last_mealtime = 0;
		philos[i].data = data;
		i++;
	}
	return (philos);
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

/**
 * @brief takes the program args and assigns them to the members of the data
 * struct, wish I could loop through struct members in C
 * Right now all input errors give the same msg, could expand on that maybee
 * also gosh I feel dirty for
 * @param argc
 * @param argv
 * @param data
 * @return true
 * @return false
 */
bool	data_init(int argc, char **argv, t_data *data)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (false);
	i = 1;
	data->nbr_of_philos = philatoi(argv[i++]);
	data->time_till_death = philatoi(argv[i++]);
	data->time_to_eat = philatoi(argv[i++]);
	data->sleep_time = philatoi(argv[i++]);
	data->full_philos = 0;
	if (argc == 6)
	{
		data->meals_needed = philatoi(argv[i]);
		data->finish_when_full = true;
	}
	else
	{
		data->finish_when_full = false;
		data->meals_needed = 1;
	}
	if (data->nbr_of_philos <= 0 || data->time_till_death <= 0 || \
	data->time_to_eat <= 0 || data->sleep_time <= 0 || data->meals_needed <= 0)
		return (false);
	return (true);
}
