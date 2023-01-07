/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 12:11:08 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/07 13:15:34 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	ft_think(t_philo *philo)
{
	t_table	*table;

	if (ft_check_dead(philo) == -1)
		return ;
	table = philo->table_data;
	if (philo->state != 1 && philo->state != 0)
	{
		philo->state = 1;
		philo->time_last_action = ft_get_time();
		ft_print_msg(philo, "is thinking\n");
	}
	ft_usleep(philo, 1);
}

void	ft_sleep(t_philo *philo)
{
	t_table	*table;

	if (ft_check_dead(philo) == -1)
		return ;
	table = philo->table_data;
	philo->state = 4;
	philo->time_last_action = ft_get_time();
	ft_print_msg(philo, "is sleeping\n");
	ft_usleep(philo, table->time_to_sleep);
}

void	ft_eat(t_philo *philo)
{
	t_table	*table;

	if (ft_check_dead(philo) == -1)
		return ;
	table = philo->table_data;
	philo->state = 3;
	philo->meals_eaten++;
	philo->time_last_action = ft_get_time();
	ft_print_msg(philo, "is eating\n");
	ft_usleep(philo, table->time_to_eat);
	philo->last_meal = ft_get_time();
	ft_drop_forks(philo);
	if (table->max_meals - philo->meals_eaten == 0)
		return ;
	ft_sleep(philo);
}

int	ft_check_dead(t_philo *philo)
{
	t_table	*table;
	t_ms	time;

	table = philo->table_data;
	time = ft_get_time();
	pthread_mutex_lock(&table->death_mutex);
	if (table->philo_dead == true)
	{
		pthread_mutex_unlock(&table->death_mutex);
		return (-1);
	}
	if ((time - philo->last_meal) >= table->time_to_die)
	{
		table->philo_dead = true;
		philo->time_last_action = ft_get_time();
		pthread_mutex_unlock(&table->death_mutex);
		ft_print_msg(philo, "is dead\n");
		return (-1);
	}
	pthread_mutex_unlock(&table->death_mutex);
	return (0);
}
