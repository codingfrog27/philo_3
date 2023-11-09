/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   time.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/10 18:02:24 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/02/08 18:45:38 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "philo.h"

long	timestamp(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long	time_since_x(long x)
{
	return (timestamp() - x);
}

long	time_since_start(t_data *data)
{
	return (timestamp() - data->start_time);
}

bool	philo_sleep(useconds_t sleep_time, t_philo *philo)
{
	long		start_time;
	// int			death_check_count;
	(void)philo;

	start_time = timestamp();
	while (timestamp() - start_time < sleep_time)
	{
		usleep(250);
		// death_check_count++;
		// if (!death_check_count % 3)
		// {
		// 	pthread_mutex_lock(philo->philo_lock);
		// 	// if (!is_alive(philo->data, philo)) //results in double free
		// 	if (!philo->alive)
		// 	{
		// 		pthread_mutex_unlock(philo->philo_lock);
		// 		return (false);
		// 	}
		// 	pthread_mutex_unlock(philo->philo_lock);
		// }
	}
	return (true);
}
