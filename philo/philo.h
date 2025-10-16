/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkeerewe <mkeerewe@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 15:44:19 by mkeerewe          #+#    #+#             */
/*   Updated: 2025/10/16 16:13:43 by mkeerewe         ###   ########.fr       */
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
	pthread_mutex_t	*fork_a;
	pthread_mutex_t	fork_b;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	struct timeval	start;
	pthread_mutex_t	*print;
	t_time			time;
	pthread_mutex_t	dead_m;
	int				dead;
	t_time			last_meal;
	pthread_mutex_t	*stop_m;
	int				*stop;
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
	pthread_mutex_t	print;
	pthread_t		monitor;
	pthread_mutex_t	stop_m;
	int				stop;
}	t_data;

int		ft_atoi(const char *str);
t_time	get_timestamp(struct timeval start);
void	assign_forks(t_data *data, int i);
int		do_stop(t_philo *philo);
int		is_dead(t_philo *philo);
void	print_status(t_philo *philo, int msg);
void	destroy_mutexes(t_data *data);

void	*ft_thread_routine(void *param);
void	*monitoring_routine(void *param);
int		solo_philo(t_data *data);

#endif
