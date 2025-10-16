/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:27:54 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/16 14:13:36 by mkeerewe         ###   ########.fr       */
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

void	print_status(t_philo *philo, int msg)
{
	philo->time = get_timestamp(philo->start);
	pthread_mutex_lock(philo->print);
	if (msg == 0)
		printf("%i:%i.%.3i %i is sleeping\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
	else if (msg == 1)
		printf("%i:%i.%.3i %i is thinking\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
	else if (msg == 2)
		printf("%i:%i.%.3i %i has taken a fork\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
	else if (msg == 3)
		printf("%i:%i.%.3i %i is eating\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
	else if (msg == 4)
		printf("%i:%i.%.3i %i died\n", philo->time.min, philo->time.sec, philo->time.ms, philo->num);
	pthread_mutex_unlock(philo->print);
}

void	ft_live(t_philo *philo)
{
	while (philo->num_eat != 0 && philo->dead == 0)
	{
		print_status(philo, 0);
		usleep(philo->time_to_sleep * 1000);
		print_status(philo, 1);
		pthread_mutex_lock(philo->fork_a);
		print_status(philo, 2);
		pthread_mutex_lock(&philo->fork_b);
		print_status(philo, 2);
		print_status(philo, 3);
		philo->num_eat -= 1;
		usleep(philo->time_to_eat * 1000);
		pthread_mutex_unlock(philo->fork_a);
		pthread_mutex_unlock(&philo->fork_b);
	}
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
	philo.print = &data->print;
	philo.dead = 0;
	philo.last_meal.min = 0;
	philo.last_meal.sec = 0;
	philo.last_meal.ms = 0;
	data->philo_arr[i] = philo;
	assign_forks(data, i);
	pthread_create(&(data->philo_arr[i].tid), NULL, ft_thread_routine, &(data->philo_arr[i]));
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
	while (deaths < data->num_philo)
	{
		i = 0;
		while (i < data->num_philo)
		{
			now = get_timestamp(data->start);
			tslm = now.min * 60000 + now.sec * 1000 + now.ms
				- data->philo_arr[i].last_meal.min * 60000
				- data->philo_arr[i].last_meal.sec * 1000
				- data->philo_arr[i].last_meal.ms;
			if ((tslm >= data->time_to_die || data->philo_arr[i].num_eat == 0)
				&& data->philo_arr[i].dead != 1)
			{
				if (tslm >= data->time_to_die)
					print_status(&data->philo_arr[i], 4);
				data->philo_arr[i].dead = 1;
				deaths++;
			}
		}
	}
	return (NULL);
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
	// i = 0;
	// while (i < data->num_philo)
	// {
	// 	pthread_create(&(data->philo_arr[i].tid), NULL, ft_thread_routine, &(data->philo_arr[i]));
	// 	i++;
	// }
	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(data->philo_arr[i].tid, NULL);
		i++;
	}
	pthread_join(data->monitor, NULL);
}

int	solo_philo(t_data *data) // fix time to die
{
	t_philo	philo;

	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i is sleeping\n", philo.time.min, philo.time.sec, philo.time.ms, 1);
	usleep(data->time_to_sleep * 1000);
	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i is thinking\n", philo.time.min, philo.time.sec, philo.time.ms, 1);
	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i has taken a fork\n", philo.time.min, philo.time.sec, philo.time.ms, 1);
	usleep(data->time_to_die * 1000);
	philo.time = get_timestamp(data->start);
	printf("%i:%i.%.3i %i died\n", philo.time.min, philo.time.sec, philo.time.ms, 1);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_data	data;

	if (argc < 5)
		return (1);
	data.num_philo = ft_atoi(argv[1]);
	if (data.num_philo == 0)
		return (0);
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	gettimeofday(&(data.start), NULL);
	if (data.num_philo == 1)
		return (solo_philo(&data));
	data.num_eat = -1;
	if (argc == 6)
		data.num_eat = ft_atoi(argv[5]);
	data.philo_arr = (t_philo *) malloc(data.num_philo * sizeof(t_philo));
	if (data.philo_arr == NULL)
		return (1);
	pthread_mutex_init(&data.print, NULL);
	create_philo_threads(&data);
	free(data.philo_arr);
	return (0);
}
