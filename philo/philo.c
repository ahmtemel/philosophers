/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:31:15 by ahmtemel          #+#    #+#             */
/*   Updated: 2023/09/17 12:31:23 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_philo_eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	ft_philo_print(philo, "has taken a fork");
	if (philo->num_of_philo == 1)
	{
		ft_philo_wait(philo->death_time);
		ft_philo_print(philo, "died");
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	pthread_mutex_lock(philo->right_fork);
	ft_philo_print(philo, "has taken a fork");
	ft_philo_print(philo, "is eating");
	pthread_mutex_lock(&philo->last);
	philo->last_meal = ft_get_time_of_ms();
	pthread_mutex_unlock(&philo->last);
	ft_philo_wait(philo->eat_time);
	pthread_mutex_lock(&philo->total);
	philo->total_eaten++;
	pthread_mutex_unlock(&philo->total);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	return (0);
}

void	ft_philo_sleep(t_philo *philo)
{
	ft_philo_print(philo, "is sleeping");
	ft_philo_wait(philo->sleep_time);
}

void	ft_philo_print(t_philo *philo, char *status)
{
	pthread_mutex_lock(philo->death);
	if (!*philo->check_dead)
	{
		printf("%llu %d %s\n", (ft_get_time_of_ms() - philo->start_time),
			philo->philo_id, status);
		if (!ft_strcmp(status, "died"))
			*philo->check_dead = 1;
	}
	pthread_mutex_unlock(philo->death);
}

int	ft_philo_wait(t_time wait_time)
{
	t_time	time;

	time = ft_get_time_of_ms();
	while (ft_get_time_of_ms() - time < wait_time)
	{
		usleep(100);
	}
	return (0);
}
