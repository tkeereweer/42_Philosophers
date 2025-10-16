/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:00:34 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/16 16:13:44 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_data *data, int i)
{
	if (data->philo_arr[i].num == 1)
	{
		pthread_mutex_init(&data->philo_arr[i].fork_b, NULL);
	}
	else if (data->philo_arr[i].num == data->num_philo)
	{
		data->philo_arr[i].fork_a = &data->philo_arr[i - 1].fork_b;
		pthread_mutex_init(&data->philo_arr[i].fork_b, NULL);
		data->philo_arr[0].fork_a = &data->philo_arr[i].fork_b;
	}
	else
	{
		data->philo_arr[i].fork_a = &data->philo_arr[i - 1].fork_b;
		pthread_mutex_init(&data->philo_arr[i].fork_b, NULL);
	}
}

int	do_stop(t_philo *philo)
{
	int	ret;

	pthread_mutex_lock(philo->stop_m);
	ret = *(philo->stop);
	pthread_mutex_unlock(philo->stop_m);
	return (ret);
}

int	is_dead(t_philo *philo)
{
	int	ret;

	pthread_mutex_lock(&philo->dead_m);
	ret = philo->dead;
	pthread_mutex_unlock(&philo->dead_m);
	return (ret);
}

void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		pthread_mutex_destroy(&data->philo_arr[i].fork_b);
		pthread_mutex_destroy(&data->philo_arr[i].dead_m);
		i++;
	}
	pthread_mutex_destroy(&data->stop_m);
	pthread_mutex_destroy(&data->print);
}
