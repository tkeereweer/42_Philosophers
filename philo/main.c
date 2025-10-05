/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:27:54 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/05 14:38:31 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_time	get_timestamp(struct timeval start)
{
	struct timeval	tv;
	t_time			time;
	int				sec_diff;

	time.min = 0;
	gettimeofday(&tv, NULL);
	sec_diff = tv.tv_sec - start.tv_sec;
	if (sec_diff / 60 > 0)
	{
		time.min = sec_diff / 60;
		time.sec = sec_diff - (time.min * 60);
	}
	else
		time.sec = sec_diff;
	time.ms = (tv.tv_usec - start.tv_usec) / 1000;
	return (time);
}

void	*ft_live(void *param)
{
	t_philo	*philo;
	t_time	time;

	philo = (t_philo *) param;
	time = get_timestamp(philo->start);
	printf("%i:%i.%i %i is sleeping\n", time.min, time.sec, time.ms, philo->num);
	usleep(philo->time_to_sleep);
	time = get_timestamp(philo->start);
	printf("%i:%i.%i %i woke up\n", time.min, time.sec, time.ms, philo->num);
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
	pthread_create(&(data->philo_arr[i].tid), NULL, ft_live, &(data->philo_arr[i]));
}

void	create_philo_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{

		create_philosopher(data, i);
		usleep(1000000);
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
