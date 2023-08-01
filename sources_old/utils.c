/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: mde-cloe <mde-cloe@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/06 18:52:05 by mde-cloe      #+#    #+#                 */
/*   Updated: 2023/06/06 18:52:05 by mde-cloe      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_print(long timestamp, t_msg_types msg, int philo_id, t_data *data)
{
	const char	str[][27] = {"has yoinked a fork", "is munching", \
	"is catching z's", "is pondering their orb", "Took an L in staying alive"};

	pthread_mutex_lock(&data->print_lock);
	printf("%lu philo %i %s", timestamp, philo_id, str[msg]);
	pthread_mutex_unlock(&data->print_lock);
}
