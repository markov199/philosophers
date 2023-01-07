/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 11:55:15 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/07 13:09:29 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"philo.h"

void	ft_print_msg(t_philo *philo, char *str);
void	ft_pick_forks(t_philo *philo);
void	ft_drop_forks(t_philo *philo);
int		ft_check_dead(t_philo *philo);
void	ft_usleep(t_philo *philo, t_ms time);
void	ft_exit(t_table *table);
t_ms	ft_get_time(void);

void	ft_pick_forks(t_philo *philo)
{
	t_table	*table;

	table = philo->table_data;
	if (ft_check_dead(philo) == -1 || table->num_philo == 1)
		return ;
	pthread_mutex_lock(&table->mutex_thread);
	if (table->forks[philo->id -1].used != 0
		|| table->forks[philo->id % table->num_philo].used != 0)
		pthread_mutex_unlock(&table->mutex_thread);
	else
	{
		if (table->forks[philo->id - 1].wait != philo->id
			&& table->forks[philo->id % table->num_philo].wait != philo->id)
		{
			table->forks[philo->id -1].used = 1;
			table->forks[philo->id % table->num_philo].used = 1;
			philo->has_fork = 1;
			philo->state = 2;
			philo->time_last_action = ft_get_time();
			pthread_mutex_unlock(&table->mutex_thread);
			ft_print_msg(philo, "has taken forks\n");
		}
		else
			pthread_mutex_unlock(&table->mutex_thread);
	}
}

void	ft_drop_forks(t_philo *philo)
{
	t_table	*table;

	table = philo->table_data;
	if (ft_check_dead(philo) == -1)
		return ;
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
}

void	*routine(void *args)
{
	t_philo	*philo;

	philo = (t_philo *)args;
	while (philo->table_data->max_meals - philo->meals_eaten != 0)
	{
		pthread_mutex_lock(&philo->table_data->death_mutex);
		if (philo->table_data->philo_dead == true)
		{
			pthread_mutex_unlock(&philo->table_data->death_mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->table_data->death_mutex);
		ft_pick_forks(philo);
		if (philo->has_fork)
		{
			ft_eat(philo);
			if (philo->table_data->max_meals - philo->meals_eaten == 0)
				return (NULL);
		}
		else
			ft_think(philo);
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

	table = calloc(sizeof(t_table), 1);
	if (ac == 5 || ac == 6)
	{
		errormsg = NULL;
		ft_init_table(table, ac, av, &errormsg);
		if (errormsg)
			return (0);
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
