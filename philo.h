/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 11:54:45 by mkovoor           #+#    #+#             */
/*   Updated: 2023/01/09 14:57:12 by mkovoor          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include<pthread.h>
# include<stdio.h>
# include<stdbool.h>
# include<sys/time.h>
# include<unistd.h>
# include<stdlib.h>
# include"message.h"

typedef unsigned int	t_ms;

typedef char			*t_error;
typedef struct s_fork{
	pthread_mutex_t	fork_mutex;
	int				wait;
	bool			used;
}	t_fork;

typedef struct s_table{
	int				num_philo;
	int				max_meals;
	t_ms			start_time;
	t_ms			time_to_die;
	t_ms			time_to_eat;
	t_ms			time_to_sleep;
	bool			philo_dead;
	t_fork			*forks;
	struct s_philo	*philo;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_thread;
	pthread_mutex_t	mutex_check;
	pthread_mutex_t	death_mutex;
}	t_table;

typedef struct s_philo{
	int			id;
	int			meals_eaten;
	bool		has_fork;
	int			state;
	t_ms		last_meal;
	t_table		*table_data;
	pthread_t	threads;
	t_ms		time_start;
	t_ms		time_last_action;
}	t_philo;

/*utilities*/

void	ft_print_msg(t_philo *philo, char *str, t_ms time);
int		ft_atoi(int *number, char *str, t_error *error);
int		ft_usleep(t_philo *philo, t_ms time);
void	*ft_calloc(size_t nmemb, size_t size);
void	*ft_memset(void *s, int c, size_t n);
t_ms	ft_get_time(void);

/*action functions*/
int		ft_pick_forks(t_philo *philo);
int		ft_drop_forks(t_philo *philo);
int		ft_think(t_philo *philo);
int		ft_sleep(t_philo *philo);
int		ft_eat(t_philo *philo);

/*init and thread functions*/

void	*routine(void *args);
void	ft_exit(t_table *table);
int		ft_check_dead(t_philo *philo);
void	ft_get_args(t_table *table, int ac, char *av[], t_error *error);

void	ft_init_forks(t_table *table, t_error *error);
void	ft_init_threads(t_table *table);
void	ft_init_philo(t_table *table, t_error *error);
t_table	*ft_init_table(t_table *table, int ac, char *av[], t_error *error);

#endif
/* states *thinking = 1, taken fork = 2, eating = 3, sleeping = 4, died = 5 */
