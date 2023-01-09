/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 12:06:07 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/09 15:03:59 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	ft_print_msg(t_philo *philo, char *str, t_ms time)
{
	pthread_mutex_lock(&philo->table_data->mutex_print);
	if (philo->state == 1)
		printf("\033[32m[%u] %d %s\033[0m", time, philo->id, str);
	else if (philo->state == 2)
		printf("\033[35m[%u] %d %s\033[0m", time, philo->id, str);
	else if (philo->state == 3)
		printf("\033[33m[%u] %d %s\033[0m", time, philo->id, str);
	else if (philo->state == 4)
		printf("\033[31m[%u] %d %s\033[0m", time, philo->id, str);
	else if (philo->state == 5)
		printf("\033[34m[%u] %d %s\033[0m", time, philo->id, str);
	else
		printf("[%u] %d %s", time, philo->id, str);
	pthread_mutex_unlock(&philo->table_data->mutex_print);
}

int	ft_usleep(t_philo *philo, t_ms sleep_time)
{
	t_ms	t;

	t = ft_get_time();
	while (1)
	{
		pthread_mutex_lock(&philo->table_data->mutex_check);
		if (philo->table_data->philo_dead == true)
		{
			pthread_mutex_unlock(&philo->table_data->mutex_check);
			return (-1);
		}
		pthread_mutex_unlock(&philo->table_data->mutex_check);
		if (ft_check_dead(philo) == -1)
			return (-1);
		if ((ft_get_time() - t) >= sleep_time)
			return (0);
		usleep(100);
	}
	return (0);
}

int	ft_atoi(int *number, char *str, t_error *error)
{
	int	i;
	int	sign;

	i = 0;
	sign = 0;
	if (str[sign] == '-' || str[sign] == '+')
	{
		if (str[sign] == '-')
			*error = INVALID_ARGS;
		sign = 1;
	}
	i = sign;
	while (str[i])
		if (str[i] < '0' || str[i++] > '9')
				*error = INVALID_ARGS;
	if (*error)
		return (0);
	i = sign;
	*number = 0;
	while (str[i] >= '0' && str[i] <= '9')
		*number = (*number * 10 +(str[i++] - '0'));
	if (*number == 0)
		*error = INVALID_ARGS;
	return (*number);
}

t_ms	ft_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((t_ms)time.tv_sec * 1000 + time.tv_usec / 1000);
}
