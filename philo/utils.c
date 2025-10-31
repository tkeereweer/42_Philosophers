/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:24:56 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/31 11:50:05 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (ft_isspace(str[i]) == 1)
		i++;
	if (str[i] == '-')
	{
		sign *= -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

int	check_input(int argc, char *argv[])
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	get_timestamp(struct timeval start)
{
	struct timeval	tv;
	int				sec_diff;
	int				ms_diff;

	gettimeofday(&tv, NULL);
	sec_diff = tv.tv_sec - start.tv_sec;
	ms_diff = (tv.tv_usec - start.tv_usec) / 1000;
	if (ms_diff < 0)
	{
		sec_diff--;
		ms_diff = 1000 + ms_diff;
	}
	return (sec_diff * 1000 + ms_diff);
}

void	print_status(t_philo *philo, int msg)
{
	philo->time = get_timestamp(philo->start);
	pthread_mutex_lock(philo->print);
	if (msg == 0)
		printf("%i %i is sleeping\n", philo->time, philo->num);
	else if (msg == 1)
		printf("%i %i is thinking\n", philo->time, philo->num);
	else if (msg == 2)
		printf("%i %i has taken a fork\n", philo->time, philo->num);
	else if (msg == 3)
		printf("%i %i is eating\n", philo->time, philo->num);
	else if (msg == 4)
		printf("%i %i died\n", philo->time, philo->num);
	pthread_mutex_unlock(philo->print);
}
