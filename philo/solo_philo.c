/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solo_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:17:40 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/03 13:18:14 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*solo_philo_routine(void *param)
{
	t_data	*data;
	int		time;

	data = (t_data *) param;
	time = get_timestamp(data->start);
	printf("%i %i is sleeping\n", time, 1);
	usleep(data->time_to_sleep * 1000);
	time = get_timestamp(data->start);
	printf("%i %i is thinking\n", time, 1);
	time = get_timestamp(data->start);
	printf("%i %i has taken a fork\n", time, 1);
	usleep((data->time_to_die - data->time_to_sleep) * 1000);
	time = get_timestamp(data->start);
	printf("%i %i died\n", time, 1);
	return (NULL);
}
