/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:45:18 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/16 16:13:55 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_eat(t_philo *philo)
{
	print_status(philo, 3);
	philo->num_eat -= 1;
	usleep(philo->time_to_eat * 1000);
	philo->last_meal = get_timestamp(philo->start);
	pthread_mutex_unlock(philo->fork_a);
	pthread_mutex_unlock(&philo->fork_b);
}

static void	ft_live(t_philo *philo)
{
	while (philo->num_eat != 0 && is_dead(philo) == 0 && do_stop(philo) == 0)
	{
		print_status(philo, 0);
		usleep(philo->time_to_sleep * 1000);
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			print_status(philo, 1);
		pthread_mutex_lock(philo->fork_a);
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			print_status(philo, 2);
		if (is_dead(philo) == 1 || do_stop(philo) == 1)
			pthread_mutex_unlock(philo->fork_a);
		else if (is_dead(philo) == 0 && do_stop(philo) == 0)
		{
			pthread_mutex_lock(&philo->fork_b);
			if (is_dead(philo) == 0 && do_stop(philo) == 0)
				print_status(philo, 2);
			if (is_dead(philo) == 1 || do_stop(philo) == 1)
			{
				pthread_mutex_unlock(philo->fork_a);
				pthread_mutex_unlock(&philo->fork_b);
			}
		}
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			ft_eat(philo);
	}
}

void	*ft_thread_routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *) param;
	ft_live(philo);
	return (NULL);
}

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

void	*monitoring_routine(void *param)
{
	t_data	*data;
	int		i;
	t_time	now;
	int		tslm;
	int		deaths;

	data = (t_data *) param;
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
			if ((tslm >= data->time_to_die || data->philo_arr[i].num_eat == 0)
				&& data->philo_arr[i].dead != 1)
				deaths = ft_die(data, tslm, i, deaths);
			i++;
		}
	}
	return (NULL);
}
