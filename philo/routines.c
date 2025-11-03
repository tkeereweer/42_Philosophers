/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:45:18 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/11/03 13:18:07 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fork_operations(t_philo *philo, int num_fork, int mode)
{
	if (mode == 0 && num_fork == 1)
		pthread_mutex_lock(philo->fork_a);
	else if (mode == 0 && num_fork == 2)
		pthread_mutex_lock(&philo->fork_b);
	else if (mode == 1 && num_fork == 1)
		pthread_mutex_unlock(philo->fork_a);
	else if (mode == 1 && num_fork == 2)
		pthread_mutex_unlock(&philo->fork_b);
	if (is_dead(philo) == 0 && do_stop(philo) == 0 && mode == 0)
		print_status(philo, 2);
}

static void	ft_eat(t_philo *philo)
{
	print_status(philo, 3);
	pthread_mutex_lock(&philo->num_eat_m);
	if (philo->num_eat != 0)
		philo->num_eat -= 1;
	pthread_mutex_unlock(&philo->num_eat_m);
	pthread_mutex_lock(&philo->last_meal_m);
	philo->last_meal = get_timestamp(philo->start);
	pthread_mutex_unlock(&philo->last_meal_m);
	usleep(philo->time_to_eat * 1000);
	fork_operations(philo, 1, 1);
	fork_operations(philo, 2, 1);
}

static void	ft_live_even(t_philo *philo)
{
	while (is_dead(philo) == 0 && do_stop(philo) == 0)
	{
		print_status(philo, 0);
		usleep(philo->time_to_sleep * 1000);
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			print_status(philo, 1);
		fork_operations(philo, 1, 0);
		if (is_dead(philo) == 1 || do_stop(philo) == 1)
			fork_operations(philo, 1, 1);
		else if (is_dead(philo) == 0 && do_stop(philo) == 0)
		{
			fork_operations(philo, 2, 0);
			if (is_dead(philo) == 1 || do_stop(philo) == 1)
			{
				fork_operations(philo, 1, 1);
				fork_operations(philo, 2, 1);
			}
		}
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			ft_eat(philo);
	}
}

void	ft_live_uneven(t_philo *philo)
{
	while (is_dead(philo) == 0 && do_stop(philo) == 0)
	{
		fork_operations(philo, 1, 0);
		if (is_dead(philo) == 1 || do_stop(philo) == 1)
			fork_operations(philo, 1, 1);
		else if (is_dead(philo) == 0 && do_stop(philo) == 0)
		{
			fork_operations(philo, 2, 0);
			if (is_dead(philo) == 1 || do_stop(philo) == 1)
			{
				fork_operations(philo, 1, 1);
				fork_operations(philo, 2, 1);
			}
		}
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			ft_eat(philo);
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			print_status(philo, 0);
		usleep(philo->time_to_sleep * 1000);
		if (is_dead(philo) == 0 && do_stop(philo) == 0)
			print_status(philo, 1);
	}
}

void	*ft_thread_routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *) param;
	while (*(philo->sim) != 1)
		continue ;
	if (philo->num % 2 == 0)
		ft_live_even(philo);
	else
		ft_live_uneven(philo);
	return (NULL);
}
