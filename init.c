/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 12:13:39 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/09 16:28:13 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	ft_get_args(t_table *table, int ac, char *av[], t_error *error)
{
	int	i;

	i = 1;
	if (!ft_atoi(&table->num_philo, av[i++], error))
		return ;
	if (!ft_atoi((int *)&table->time_to_die, av[i++], error))
		return ;
	if (!ft_atoi((int *)&table->time_to_eat, av[i++], error))
		return ;
	if (!ft_atoi((int *)&table->time_to_sleep, av[i++], error))
		return ;
	if (i != ac)
	{
		if (!ft_atoi(&table->max_meals, av[i], error))
			return ;
	}
	else
		table->max_meals = -1;
	return ;
}

void	ft_init_forks(t_table *table, t_error *error)
{
	int	i;

	i = 0;
	table->forks = ft_calloc(sizeof(t_fork), table->num_philo);
	while (i < table->num_philo)
	{
		table->forks[i].used = false;
		table->forks[i].wait = -1;
		if (pthread_mutex_init(&table->forks[i++].fork_mutex, NULL))
		{
			while (i)
				pthread_mutex_destroy(&table->forks[--i].fork_mutex);
			*error = INIT_FORK;
		}
	}
	return ;
}

void	ft_init_threads(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = table->philo;
	table->start_time = ft_get_time();
	while (i < table->num_philo)
	{
		pthread_create(&philo[i].threads, NULL, &routine, (void *)&philo[i]);
		usleep(100);
		i++;
	}
	i = 0;
	while (i < table->num_philo)
	{
		pthread_join(philo[i].threads, NULL);
		i++;
	}
}

void	ft_init_philo(t_table *table, t_error *error)
{
	t_philo	*philo;
	int		i;

	i = 0;
	(void)error;
	philo = calloc(sizeof(t_philo), table->num_philo);
	while (i < table->num_philo)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].has_fork = false;
		philo[i].state = 0;
		philo[i].last_meal = 0;
		philo[i].table_data = table;
		philo[i].time_start = 0;
		philo[i].time_last_action = 0;
		i++;
	}
	table->philo = philo;
	return ;
}

t_table	*ft_init_table(t_table *table, int ac, char *av[], t_error *error)
{
	ft_get_args(table, ac, av, error);
	if (*error)
	{
		printf("%s", *error);
		return (table);
	}
	table->philo_dead = false;
	if (pthread_mutex_init(&table->mutex_print, NULL))
		*error = INIT_PRINT;
	if (pthread_mutex_init(&table->mutex_thread, NULL))
		*error = INIT_THREAD;
	if (pthread_mutex_init(&table->mutex_check, NULL))
		*error = INIT_THREAD;
	if (pthread_mutex_init(&table->death_mutex, NULL))
		*error = INIT_THREAD;
	ft_init_forks(table, error);
	ft_init_philo(table, error);
	return (table);
}
