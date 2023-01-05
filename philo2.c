#include"philo.h"

void ft_forks(t_philo *philo);
t_ms ft_get_time();
void ft_print_msg(t_philo *philo, char *str);
<<<<<<< HEAD
void ft_exit(t_table *table);
=======
void ft_is_dead(t_philo *philo);
>>>>>>> d5dfb059a98df534b14a19c6a71b8f45cd25ad11

void ft_print_msg(t_philo *philo, char *str)
{
    t_table *table;

    table = philo->table_data;
    pthread_mutex_lock(&(table->mutex_print));
    printf("[%d]philo[%d]%s",philo->time_last_action, philo->id, str);
    usleep(10000);
    pthread_mutex_unlock(&(table->mutex_print));
}
void ft_think(t_philo *philo)
{
    t_table *table;

    table = philo->table_data;
    if (philo->state != 1)
    {
        philo->state = 1;
        philo->time_last_action = ft_get_time() - table->start_time;
        ft_print_msg(philo, "is thinking\n");
    }
    usleep (100);
    ft_forks(philo);
}

void ft_sleep(t_philo *philo)
{
    t_table *table;

    table = philo->table_data;
    philo->state = 4;
    philo->time_last_action = ft_get_time() - table->start_time;
    ft_print_msg(philo, "is sleeping\n");
    usleep(table->time_to_sleep * 1000);
    ft_forks(philo);
}


void ft_eat(t_philo *philo)
{
    t_table *table;

    table = philo->table_data;
    philo->state = 3;
    philo->meals_eaten++;
    philo->time_last_action = ft_get_time() - table->start_time;
    philo->last_meal = philo->time_last_action;
    ft_print_msg(philo, "is eating\n");
    usleep(table->time_to_eat * 1000);
    ft_forks(philo);
}

void ft_forks(t_philo *philo)
{
    t_table *table;

    table = philo->table_data;
    // if (ft_get_time() - philo->last_meal >= table->time_to_die)
    // {
    //     philo->time_last_action = ft_get_time() - table->start_time ;
    //     printf("time %d -  > %d\n",ft_get_time() - philo->last_meal,table->time_to_die );
    //     table->philo_dead = 1;
    //     ft_print_msg(philo, PHILO_DIED);
    //     return;
    // }
    if ( philo->has_fork == false)
    {
        pthread_mutex_lock(&table->mutex_thread);
        if(table->forks[philo->id -1].used == 0 && table->forks[philo->id % table->num_philo].used == 0 )
        {
            table->forks[philo->id -1].used = 1;
            table->forks[philo->id % table->num_philo].used = 1;
            philo->has_fork = 1;
            philo->state = 2;
            philo->time_last_action = ft_get_time() - table->start_time;
            ft_print_msg(philo, "has taken forks\n");
            pthread_mutex_unlock(&table->mutex_thread);
            ft_eat(philo);
        }
        else
        {
            pthread_mutex_unlock(&table->mutex_thread);
            ft_think(philo);
        }
    }
    else if ( philo->has_fork == true)
    { 
        pthread_mutex_lock(&table->mutex_thread);
        table->forks[philo->id -1].used = 0;
        table->forks[philo->id % table->num_philo].used = 0;
        philo->has_fork = 0;
        pthread_mutex_unlock(&table->mutex_thread);
        if (table->max_meals <= philo->meals_eaten )
            return;
        ft_sleep(philo);
    }
}

void *routine(void *args)
{
    t_philo *philo;

    philo = (t_philo *)args;
    ft_forks(philo);    
    return(NULL);
}

int ft_atoi(int *number, char *str, t_error *error)
{
    int i;
    int sign;

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

void    ft_get_args (t_table *table, int ac, char *av[], t_error *error)
{
    int i;

    i = 1;
    if (!ft_atoi(&table->num_philo, av[i++], error))
        return;
    if (!ft_atoi((int *)&table->time_to_die, av[i++], error))
        return;
    if (!ft_atoi((int *)&table->time_to_eat, av[i++], error))
        return;
    if (!ft_atoi((int *)&table->time_to_sleep, av[i++], error))
        return;
    if(i != ac)
    {
        if(!ft_atoi(&table->max_meals, av[i],error))
            return;
    }
    else
        table->max_meals = 0;
    return;

}
void ft_init_forks(t_table *table, t_error *error)
{
    int i;
    
    i = 0;
    table->forks = (t_fork *)malloc(sizeof(t_fork) * table->num_philo);
    while (i < table->num_philo)
    {
        table->forks[i].used = false;
        if(pthread_mutex_init(&table->forks[i++].fork_mutex, NULL))
        {
            while(i)
                pthread_mutex_destroy(&table->forks[--i].fork_mutex);
            *error = INIT_FORK;
        }
    }
    return ;
}
void ft_init_threads(t_table *table)
{
    int i;
    t_philo *philo;

    i = 0;
    philo = table->philo;
    while (i < table->num_philo)
    {
        philo[i].time_start = ft_get_time();
        philo[i].last_meal = philo[i].time_start;
        pthread_create(&philo[i].threads, NULL, &routine, &philo[i]);
        // usleep(1000);
        i++;
    }
    i = 0;
    while (i < table->num_philo)
    {
        pthread_join(philo[i].threads, NULL);
        i++;
    }
}


void ft_init_philo(t_table *table, t_error *error)
{
    t_philo *philo;
    int i;

    i = 0;
    philo = malloc(sizeof(t_philo) * table->num_philo);
    while (i < table->num_philo)
    {
        philo[i].id = i + 1;
        philo[i].meals_eaten = 0;
        philo[i].state = 0;
        philo[i].has_fork = false;
        philo[i].table_data = table;
        philo[i].last_meal = 0;
        i++;
    }
    table->philo = philo;
    return;
}

t_ms ft_get_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((t_ms)time.tv_sec * 1000 + time.tv_usec / 1000);
}


t_table *ft_init_table(int ac, char *av[], t_error *error)
{
    // int i;
    t_table *table;
    // t_fork *forks;
    // t_philo *philo;

    // i = 0;
    table = (t_table *)malloc(sizeof(t_table));
    ft_get_args(table, ac, av, error);
    if (*error)
       {
        return (table);
       }
    table->philo_dead = false;
    table->start_time = ft_get_time();
    if(pthread_mutex_init(&table->mutex_print, NULL))
        *error = INIT_PRINT;
    if(pthread_mutex_init(&table->mutex_thread, NULL))
        *error = INIT_THREAD;
    ft_init_forks(table, error);
    ft_init_philo(table, error);
    return (table);
}

void ft_exit(t_table *table)
{
    // int i;

    // i = 0;
    // pthread_mutex_destroy(&table->mutex_print);
    // free(table->forks);
    // free(table->philo);
    if (table)
        free (table);
    printf("136 exiting\n ");
}

int main(int ac, char *av[])
{
   t_table *table;
   t_error errormsg;
   if(ac == 5 || ac == 6)
   { 
        errormsg = NULL;
        table = ft_init_table(ac, av, &errormsg);
        ft_init_threads(table);
        if (errormsg || table->philo_dead)
        {
            ft_exit(table);
            printf("170%s", errormsg);
            return (0);
        }
    }
    return (0);
}
    
