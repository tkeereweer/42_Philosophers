/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 10:21:13 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/27 10:25:26 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_die(t_data *data, int tslm, int i, int deaths)
{
	if (tslm >= data->time_to_die)
		print_status(&data->philo_arr[i], 4);
	if (tslm >= data->time_to_die && data->philo_arr[i].num_eat < 0)
	{
		pthread_mutex_lock(&data->stop_m);
		data->stop = 1;
		pthread_mutex_unlock(&data->stop_m);
	}
	pthread_mutex_lock(&data->philo_arr[i].dead_m);
	data->philo_arr[i].dead = 1;
	pthread_mutex_unlock(&data->philo_arr[i].dead_m);
	deaths++;
	return (deaths);
}

static void	monitoring_routine(t_data *data)
{
	int		i;
	t_time	now;
	int		tslm;
	int		deaths;

	deaths = 0;
	while (deaths < data->num_philo && data->stop == 0)
	{
		i = 0;
		while (i < data->num_philo && data->stop == 0)
		{
			now = get_timestamp(data->start);
			tslm = now.min * 60000 + now.sec * 1000 + now.ms
				- data->philo_arr[i].last_meal.min * 60000
				- data->philo_arr[i].last_meal.sec * 1000
				- data->philo_arr[i].last_meal.ms;
			pthread_mutex_lock(&data->philo_arr[i].num_eat_m);
			if ((tslm >= data->time_to_die || data->philo_arr[i].num_eat == 0)
				&& data->philo_arr[i].dead != 1)
				deaths = ft_die(data, tslm, i, deaths);
			pthread_mutex_unlock(&data->philo_arr[i].num_eat_m);
			i++;
		}
	}
}

void	*run_monitoring_routine(void *param)
{
	t_data	*data;

	data = (t_data *) param;
	monitoring_routine(data);
	return (NULL);
}
