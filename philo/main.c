/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:27:54 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/31 11:53:47 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	solo_philo(t_data *data)
{
	pthread_create(&(data->solo_philo), NULL, solo_philo_routine, data);
	pthread_join(data->solo_philo, NULL);
	return (0);
}

static int	init_data(int argc, char *argv[], t_data *data)
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
	data->sim = 0;
	data->philo_arr = (t_philo *) malloc(data->num_philo * sizeof(t_philo));
	if (data->philo_arr == NULL)
		return (1);
	data->stop = 0;
	pthread_mutex_init(&data->print, NULL);
	pthread_mutex_init(&data->stop_m, NULL);
	return (2);
}

static void	create_philosopher(t_data *data, int i)
{
	t_philo	philo;

	philo.num = i + 1;
	philo.time_to_die = data->time_to_die;
	philo.time_to_eat = data->time_to_eat;
	philo.time_to_sleep = data->time_to_sleep;
	philo.num_eat = data->num_eat;
	philo.start = data->start;
	philo.print = &data->print;
	philo.dead = 0;
	philo.last_meal = 0;
	philo.stop_m = &data->stop_m;
	philo.stop = &data->stop;
	philo.sim = &data->sim;
	data->philo_arr[i] = philo;
	pthread_mutex_init(&data->philo_arr[i].num_eat_m, NULL);
	pthread_mutex_init(&data->philo_arr[i].last_meal_m, NULL);
	pthread_mutex_init(&data->philo_arr[i].dead_m, NULL);
	assign_forks(data, i);
	pthread_create(&(data->philo_arr[i].tid), NULL,
		ft_thread_routine, &(data->philo_arr[i]));
}

static void	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	gettimeofday(&(data->start), NULL);
	while (i < data->num_philo)
	{
		create_philosopher(data, i);
		i++;
	}
	pthread_create(&data->monitor, NULL, run_monitoring_routine, data);
	data->sim = 1;
	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(data->philo_arr[i].tid, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
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
