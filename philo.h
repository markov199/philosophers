
#ifndef PHILO_H
#define PHILO_H

#include<pthread.h>
#include<stdio.h>
#include<stdbool.h>
#include<sys/time.h>
#include<unistd.h>
#include<stdlib.h>
#include"message.h"

typedef unsigned int    t_ms;

typedef char            *t_error;

typedef struct s_fork{
    pthread_mutex_t fork_mutex;
	int wait;
    bool used;
}   t_fork;

typedef struct s_table{
    int num_philo;
    int max_meals;
    t_ms start_time;
    t_ms time_to_die;
    t_ms time_to_eat;
    t_ms time_to_sleep;
    bool philo_dead;
    t_fork *forks;
    struct s_philo *philo;
    pthread_mutex_t mutex_print;
    pthread_mutex_t mutex_thread;
	pthread_mutex_t	mutex_check;
}   t_table;

typedef struct s_philo{
    int id;
    int meals_eaten;
    bool has_fork;
    int state; /*thinking = 1, taken fork = 2, eating = 3, sleeping = 4, died = 5 */
    t_ms last_meal;
    t_table *table_data;
    pthread_t threads;
    t_ms time_start;
    t_ms time_last_action;
}   t_philo;

# endif
