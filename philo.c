/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 11:55:15 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/09 16:34:58 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

int	ft_pick_forks(t_philo *philo)
{
	t_table	*table;

	table = philo->table_data;
	pthread_mutex_lock(&table->mutex_thread);
	if (table->num_philo > 1 && table->forks[philo->id -1].used == 0
		&& table->forks[philo->id % table->num_philo].used == 0)
	{
		if (table->forks[philo->id - 1].wait != philo->id
			&& table->forks[philo->id % table->num_philo].wait != philo->id)
		{
			table->forks[philo->id -1].used = 1;
			table->forks[philo->id % table->num_philo].used = 1;
			pthread_mutex_unlock(&table->mutex_thread);
			philo->has_fork = 1;
			philo->state = 2;
			philo->time_last_action = ft_get_time();
			if (ft_check_dead(philo) == -1)
				return (-1);
			ft_print_msg(philo, "has taken fork\n", ft_get_time() - \
			philo->table_data->start_time);
			return (1);
		}
	}
	else
		pthread_mutex_unlock(&table->mutex_thread);
	return (0);
}

int	ft_drop_forks(t_philo *philo)
{
	t_table	*table;

	table = philo->table_data;
	if (philo->has_fork == true)
	{
		pthread_mutex_lock(&table->mutex_thread);
		table->forks[philo->id -1].used = 0;
		table->forks[philo->id - 1].wait = philo->id;
		table->forks[philo->id % table->num_philo].used = 0;
		table->forks[philo->id % table->num_philo].wait = philo->id;
		philo->has_fork = 0;
		pthread_mutex_unlock(&table->mutex_thread);
	}
	return (0);
}

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	philo->last_meal = ft_get_time();
	while (philo->table_data->max_meals - philo->meals_eaten != 0)
	{
		if (ft_pick_forks(philo) == 1)
		{
			if (ft_eat(philo) == -1)
				return (NULL);
		}
		else
		{
			if (ft_think(philo) == -1)
				return (NULL);
			continue ;
		}
		if (ft_sleep(philo) == -1)
			return (NULL);
	}
	return (NULL);
}

void	ft_exit(t_table *table)
{
	if (table->forks)
		free(table->forks);
	if (table->philo)
		free(table->philo);
	if (table)
		free(table);
}

int	main(int ac, char *av[])
{
	t_table	*table;
	t_error	errormsg;

	table = ft_calloc(sizeof(t_table), 1);
	if (ac == 5 || ac == 6)
	{
		errormsg = NULL;
		ft_init_table(table, ac, av, &errormsg);
		if (errormsg)
		{
			ft_exit(table);
			return (0);
		}
		ft_init_threads(table);
		pthread_mutex_destroy(&table->mutex_print);
		pthread_mutex_destroy(&table->mutex_thread);
		pthread_mutex_destroy(&table->mutex_check);
		pthread_mutex_destroy(&table->death_mutex);
	}
	else
		write (2, "Invalid number of arguments\n", 28);
	ft_exit(table);
	return (0);
}
