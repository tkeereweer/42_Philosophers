/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:27:54 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/12 16:06:04 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_data *data, int i)
{
	pthread_mutex_t	fork_a;
	pthread_mutex_t	fork_b;

	if (data->philo_arr[i].num == 1)
	{
		pthread_mutex_init(&fork_a, NULL);
		pthread_mutex_init(&fork_b, NULL);
		data->philo_arr[i].fork_a = &fork_a;
		data->philo_arr[i].fork_b = &fork_b;
	}
	else if (data->philo_arr[i].num == data->num_philo)
	{
		data->philo_arr[i].fork_a = data->philo_arr[i - 1].fork_b;
		data->philo_arr[i].fork_b = data->philo_arr[0].fork_a;
	}
	else
	{
		data->philo_arr[i].fork_a = data->philo_arr[i - 1].fork_b;
		pthread_mutex_init(&fork_b, NULL);
		data->philo_arr[i].fork_b = &fork_b;
	}
}

void	ft_live(t_philo *philo)
{
	while (philo->num_eat != 0)
	{
		philo->time = get_timestamp(philo->start);
		printf("%i:%i.%.6i %i is sleeping\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
		usleep(philo->time_to_sleep * 1000);
		philo->time = get_timestamp(philo->start);
		printf("%i:%i.%.6i %i is thinking\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
		pthread_mutex_lock(philo->fork_a);
		pthread_mutex_lock(philo->fork_b);
		philo->time = get_timestamp(philo->start);
		printf("%i:%i.%.6i %i is eating\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
		usleep(philo->time_to_eat * 1000);
		philo->num_eat -= 1;
		pthread_mutex_unlock(philo->fork_a);
		pthread_mutex_unlock(philo->fork_b);
	}
	// time = get_timestamp(philo->start);
	// printf("%i:%i.%i %i woke up\n", time.min, time.sec, time.ms, philo->num);
	// return (NULL);
}

void	*ft_thread_routine(void *param)
{
	t_philo	*philo;

	philo = (t_philo *) param;

	ft_live(philo);
	return (NULL);
}

void	create_philosopher(t_data *data, int i)
{
	t_philo	philo;

	philo.num = i + 1;
	philo.time_to_die = data->time_to_die;
	philo.time_to_eat = data->time_to_eat;
	philo.time_to_sleep = data->time_to_sleep;
	philo.num_eat = data->num_eat;
	philo.start = data->start;
	data->philo_arr[i] = philo;
	assign_forks(data, i);
	pthread_create(&(data->philo_arr[i].tid), NULL, ft_thread_routine, &(data->philo_arr[i]));
}

void	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		create_philosopher(data, i);
		i++;
	}
	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(data->philo_arr[i].tid, NULL);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc < 5)
		return (1);
	data.num_philo = ft_atoi(argv[1]);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	data.num_eat = -1;
	if (argc == 6)
		data.num_eat = ft_atoi(argv[5]);
	data.philo_arr = (t_philo *) malloc(data.num_philo * sizeof(t_philo));
	if (data.philo_arr == NULL)
		return (1);
	gettimeofday(&(data.start), NULL);
	create_philo_threads(&data);
	free(data.philo_arr);
	return (0);
}
