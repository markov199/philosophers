/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 12:11:08 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/09 15:12:36 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

int	ft_think(t_philo *philo)
{
	t_ms	time;
	
	if (ft_check_dead(philo) == -1)
		return (-1);
	if (philo->state != 1 && philo->state != 0)
	{
		philo->state = 1;
		philo->time_last_action = ft_get_time();
		time = (ft_get_time() - philo->table_data->start_time);
		ft_print_msg(philo, "is thinking\n", time);
	}
	usleep(100);
	return (0);
}

int	ft_sleep(t_philo *philo)
{
	t_ms	time;
	t_table	*table;

	table = philo->table_data;
	philo->state = 4;
	philo->time_last_action = ft_get_time();
	if (ft_check_dead(philo) == -1)
		return(-1) ;
	time = (ft_get_time() - philo->table_data->start_time);
	ft_print_msg(philo, "is sleeping\n", time);
	if (ft_usleep(philo, table->time_to_sleep) == -1)
		return (-1);
	return (0);
}

int	ft_eat(t_philo *philo)
{
	t_table	*table;
	t_ms	time;

	table = philo->table_data;
	philo->state = 3;
	philo->meals_eaten++;
	philo->last_meal = ft_get_time();
	philo->time_last_action = philo->last_meal;
	if (ft_check_dead(philo) == -1)
		return(-1) ;
	time = (ft_get_time() - philo->table_data->start_time);
	ft_print_msg(philo, "is eating\n", time);
	if (ft_usleep(philo, table->time_to_eat) == -1)
		return(-1);	
	ft_drop_forks(philo);
	if (table->max_meals - philo->meals_eaten == 0)
		return(-1) ;
	return (0);
}

int	ft_check_dead(t_philo *philo)
{
	t_table	*table;
	t_ms	time;
	t_ms	time2;

	table = philo->table_data;
	time = ft_get_time();
	pthread_mutex_lock(&table->death_mutex);
	if (table->philo_dead == true)
	{
		pthread_mutex_unlock(&table->death_mutex);
		return (-1);
	}
	if ((time - philo->last_meal) > table->time_to_die)
	{
		table->philo_dead = true;
		philo->time_last_action = ft_get_time();
		pthread_mutex_unlock(&table->death_mutex);
		time2 = (ft_get_time() - philo->table_data->start_time);
		ft_print_msg(philo, "is dead\n", time2);
		return (-1);
	}
	pthread_mutex_unlock(&table->death_mutex);
	return (0);
}
