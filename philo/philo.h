/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:44:19 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/05 14:38:45 by mkeerewe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_time
{
	int	min;
	int	sec;
	int	ms;
}	t_time;

typedef struct s_philo
{
	pthread_t		tid;
	int				num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	struct timeval	start;
}	t_philo;

typedef struct s_data
{
	t_philo			*philo_arr;
	int				num_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	struct timeval	start;
}	t_data;

int	ft_atoi(const char *str);

#endif

