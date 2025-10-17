/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:27:54 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/17 14:44:36 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(int argc, char *argv[], t_data *data)
{
	data->num_philo = ft_atoi(argv[1]);
	if (data->num_philo == 0)
		return (0);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	gettimeofday(&(data->start), NULL);
	if (data->num_philo == 1)
		return (solo_philo(data));
	data->num_eat = -1;
	if (argc == 6)
		data->num_eat = ft_atoi(argv[5]);
	data->philo_arr = (t_philo *) malloc(data->num_philo * sizeof(t_philo));
	if (data->philo_arr == NULL)
		return (1);
	data->stop = 0;
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->stop_m, NULL);
	return (2);
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
	pthread_mutex_init(&philo.time_m, NULL);
	philo.print = &data->print;
	pthread_mutex_init(&philo.dead_m, NULL);
	philo.dead = 0;
	philo.last_meal.min = 0;
	philo.last_meal.sec = 0;
	philo.last_meal.ms = 0;
	philo.stop_m = &data->stop_m;
	philo.stop = &data->stop;
	data->philo_arr[i] = philo;
	assign_forks(data, i);
	pthread_create(&(data->philo_arr[i].tid), NULL,
		ft_thread_routine, &(data->philo_arr[i]));
}

void	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	gettimeofday(&(data->start), NULL);
	while (i < data->num_philo)
	{
		create_philosopher(data, i);
		i++;
	}
	pthread_create(&data->monitor, NULL, monitoring_routine, data);
	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(data->philo_arr[i].tid, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

int	solo_philo(t_data *data)
{
	t_philo	philo;

	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i is sleeping\n", philo.time.min,
		philo.time.sec, philo.time.ms, 1);
	usleep(data->time_to_sleep * 1000);
	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i is thinking\n", philo.time.min,
		philo.time.sec, philo.time.ms, 1);
	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i has taken a fork\n", philo.time.min,
		philo.time.sec, philo.time.ms, 1);
	usleep((data->time_to_die - data->time_to_sleep) * 1000);
	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i died\n", philo.time.min,
		philo.time.sec, philo.time.ms, 1);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;
	int		ret;

	if (argc < 5)
		return (1);
	if (check_input(argc, argv) == 0)
		return (1);
	ret = init_data(argc, argv, &data);
	if (ret != 2)
		return (ret);
	create_philo_threads(&data);
	destroy_mutexes(&data);
	free(data.philo_arr);
	return (0);
}
