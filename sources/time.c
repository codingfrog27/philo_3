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

long	time_since_start(void)
{
	static long	start_time;

	if (!start_time)
		start_time = timestamp();
	return (timestamp() - start_time);
}

bool	coolsleep(useconds_t sleep_time)
{
	long		start_time;
	useconds_t	nap_time;

	start_time = timestamp();
	nap_time = sleep_time / 50;
	while (timestamp() - start_time < sleep_time)
		usleep(nap_time);
}
