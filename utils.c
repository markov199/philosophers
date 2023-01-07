/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 12:06:07 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/07 13:50:11 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	ft_print_msg(t_philo *philo, char *str)
{
	t_table	*table;
	t_ms	time;

	table = philo->table_data;
	time = philo->time_last_action - table->start_time;
	pthread_mutex_lock(&(table->mutex_print));
	printf("[%d]philo[%d]%s", time, philo->id, str);
	pthread_mutex_unlock(&(table->mutex_print));
}

void	ft_usleep(t_philo *philo, t_ms sleep_time)
{
	t_table	*table;
	t_ms	time;

	table = philo->table_data;
	time = ft_get_time();
	while (1)
	{
		pthread_mutex_lock(&table->death_mutex);
		if (table->philo_dead == true)
		{
			pthread_mutex_unlock(&table->death_mutex);
			return ;
		}
		pthread_mutex_unlock(&table->death_mutex);
		ft_check_dead(philo);
		if ((ft_get_time() - time) >= sleep_time)
			return ;
	}
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
