/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:31:53 by ahmtemel          #+#    #+#             */
/*   Updated: 2023/09/17 12:32:02 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->last);
	if ((ft_get_time_of_ms() - philo->last_meal) > philo->death_time)
	{
		pthread_mutex_unlock(&philo->last);
		ft_philo_print(philo, "died");
		return (1);
	}
	pthread_mutex_unlock(&philo->last);
	return (0);
}

int	ft_is_served(t_philo *philo)
{
	if (philo->must_eat != -1)
	{
		pthread_mutex_lock(&philo->total);
		if (philo->total_eaten == philo->must_eat)
		{
			pthread_mutex_unlock(&philo->total);
			return (1);
		}
		pthread_mutex_unlock(&philo->total);
	}
	return (0);
}

int	ft_check_death(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_of_philo)
	{
		if (ft_is_dead(&philo[i]) || ft_is_served(&philo[i]))
			return (1);
		i++;
	}
	return (0);
}

void	ft_create_thread(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->num_of_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]))
			return ;
		usleep(50);
		i++;
	}
	while (1)
	{
		if (ft_check_death(philo))
			break ;
	}
	i = 0;
	while (i < philo->num_of_philo)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (1)
	{
		pthread_mutex_lock(philo->death);
		if (*philo->check_dead)
		{
			pthread_mutex_unlock(philo->death);
			break ;
		}
		pthread_mutex_unlock(philo->death);
		if (ft_philo_eat(philo))
			break ;
		pthread_mutex_lock(&philo->last);
		if (philo->total_eaten == philo->must_eat)
			break ;
		pthread_mutex_unlock(&philo->last);
		ft_philo_sleep(philo);
		ft_philo_print(philo, "is thinking");
	}
	pthread_mutex_unlock(&philo->last);
	return (NULL);
}
