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

long	whattimeisitrightnow(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long	time_since_x(long x)
{
	static long	start_time;

	if (!start_time)
	{
		start_time = whattimeisitrightnow();
		printf("START TIME == %li", start_time);
	}
	return (whattimeisitrightnow() - start_time - x);
}

bool	coolsleep(useconds_t sleep_time)
{
	long		start_time;
	long		time_passed;
	useconds_t	nap_time;
	//useconds is just an unsigned int?
	//still needs to be tested, idk if the all the values of usleep and the
	// time functions translate properly
	start_time = whattimeisitrightnow();
	time_passed = 0;
	nap_time = sleep_time / 100;
	while (time_passed < sleep_time)
	{
		if (usleep(nap_time) == -1)
			return (false);
		time_passed = whattimeisitrightnow() - start_time;
	}
	return (true);
}
// could be made into one function with a passed bool maybe, but like this I
// also think it's fine
