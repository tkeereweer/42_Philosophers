/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:45:18 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/27 11:00:51 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_eat(t_philo *philo)
{
	print_status(philo, 3);
	pthread_mutex_lock(&philo->num_eat_m);
	philo->num_eat -= 1;
	pthread_mutex_unlock(&philo->num_eat_m);
	usleep(philo->time_to_eat * 1000);
	philo->last_meal = get_timestamp(philo->start);
	pthread_mutex_unlock(philo->fork_a);
	pthread_mutex_unlock(&philo->fork_b);
}

static void	ft_live(t_philo *philo)
{
	while (done_eat(philo) != 0 && is_dead(philo) == 0 && do_stop(philo) == 0)
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

void	*solo_philo_routine(void *param)
{
	t_data	*data;
	t_time	time;

	data = (t_data *) param;
	time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i is sleeping\n", time.min,
		time.sec, time.ms, 1);
	usleep(data->time_to_sleep * 1000);
	time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i is thinking\n", time.min,
		time.sec, time.ms, 1);
	time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i has taken a fork\n", time.min,
		time.sec, time.ms, 1);
	usleep((data->time_to_die - data->time_to_sleep) * 1000);
	time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i died\n", time.min,
		time.sec, time.ms, 1);
	return (NULL);
}
