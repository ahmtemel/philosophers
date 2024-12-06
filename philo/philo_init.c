/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 12:30:40 by ahmtemel          #+#    #+#             */
/*   Updated: 2023/09/17 12:32:38 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_arg_check(int ac, char **av)
{
	int	i;
	int	j;

	if (ac == 5 || ac == 6)
	{
		i = 1;
		while (av[i])
		{
			j = 0;
			while (av[i][j])
			{
				if (!(av[i][j] >= '0' && av[i][j] <= '9'))
					return (0);
				j++;
			}
			i++;
		}
	}
	else
		return (0);
	return (1);
}

int	ft_atoi(const char *str)
{
	int				i;
	long long int	is_neg;
	long long int	res;

	i = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' \
			|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-')
		is_neg = -1;
	else
		is_neg = 1;
	if (is_neg == -1 || str[i] == '+')
		i++;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - '0') * is_neg;
		if (res > 2147483647)
			return (-1);
		if (res < -2147483648)
			return (0);
		i++;
	}
	return ((int)res);
}

int	ft_arg_init(t_philo *philo, int ac, char **av, int *check_dead)
{
	int	i;

	i = -1;
	while (++i < ft_atoi(av[1]))
	{
		philo[i].philo_id = i + 1;
		philo[i].num_of_philo = ft_atoi(av[1]);
		philo[i].death_time = ft_atoi(av[2]);
		philo[i].eat_time = ft_atoi(av[3]);
		philo[i].sleep_time = ft_atoi(av[4]);
		if (ac == 6)
			philo[i].must_eat = ft_atoi(av[5]);
		else
			philo[i].must_eat = -1;
		if (!philo[i].death_time || !philo[i].eat_time || !philo[i].sleep_time
			|| !philo[i].must_eat)
			return (1);
		philo[i].last_meal = ft_get_time_of_ms();
		philo[i].start_time = ft_get_time_of_ms();
		philo[i].total_eaten = 0;
		philo[i].check_dead = check_dead;
		pthread_mutex_init(&philo[i].last, NULL);
		pthread_mutex_init(&philo[i].total, NULL);
	}
	return (0);
}

t_time	ft_get_time_of_ms(void)
{
	struct timeval	tp;
	t_time			time;

	gettimeofday(&tp, NULL);
	time = tp.tv_sec * 1000 + tp.tv_usec / 1000;
	return (time);
}

void	ft_mutex_init(t_philo *philo, pthread_mutex_t *forks,
		pthread_mutex_t *death)
{
	int	i;

	i = 0;
	while (i < philo->num_of_philo)
	{
		philo[i].left_fork = &forks[i];
		philo[i].right_fork = &forks[(i + 1) % philo->num_of_philo];
		pthread_mutex_init(&forks[i], NULL);
		philo[i].death = death;
		i++;
	}
	pthread_mutex_init(philo->death, NULL);
}
