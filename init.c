
#include"philo.h"

// int ft_get_forks(t_table *table, int x)
// {
//     pthread_mutex_lock(&table->forks[x-1].fork_mutex);
//     if (table->forks[x-1]->used == 0 && table->forks[x]->used == 0)
//     {
//         table->forks[x-1]->used = 1;
//         table->forks[x]->used = 1;
//         return (1);
//     }

//     else
//         return (0);
//     pthread_mutex_unlock(&table->forks[x-1]->fork_mutex);

// }

int ft_pop_queue(t_queue *queue)
{
    int i;
    int x;

    i = queue->priority_queue[0];
    x = 0;
    while (x < (queue->queue_count -1))
    {
        queue->priority_queue[x] = queue->priority_queue[x + 1];
        printf("%d ",queue->priority_queue[x]);
        x++;
    }
    printf("\n popped queue\n");
    queue->queue_count--;
    return (i);
}
void ft_update_queue(t_queue *queue, int x)
{
    while (x < (queue->queue_count -1))
    {
        queue->priority_queue[x] = queue->priority_queue[x + 1];
        printf("%d ",queue->priority_queue[x]);
        x++;
    }
    printf("\n updated queue\n");
    queue->queue_count--;
    return ;
}

void ft_add_to_queue( t_queue *queue, int x)
{
    int i;

    i = queue->queue_count;
    queue->priority_queue[i] = x;
    queue->queue_count++;
    i = 0;
    while(i < queue->queue_count)
    {
        printf("%d ", queue->priority_queue[i++]);
    }
    printf("\n added to queue\n");
}

void *routine(void *args)
{
    // int i;
    t_philo *philo;

    philo = (t_philo *)args;
    
    pthread_mutex_lock(&philo->table_data->mutex_print);
    printf("%dthread started\n", philo->id);
    usleep(1000);
    pthread_mutex_unlock(&philo->table_data->mutex_print);
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

 void ft_init_forks(int x, t_error *error)
{
    int i;
    t_fork *forks;
    
    i = 0;
    forks = (t_fork *)malloc(sizeof(t_fork) * x);
    while (i < x)
    {
        forks[i].used = false;
        if(pthread_mutex_init(&forks[i++].fork_mutex, NULL))
        {
            while(i)
                pthread_mutex_destroy(&forks[--i].fork_mutex);
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
    printf("156 here\n");
        printf("philo%d data\n", table->philo[i].id);
        printf("meals eaten %d \n", table->philo[i].meals_eaten);
        printf("state%d \n", table->philo[i].state);
        printf("last meal%d \n", table->philo[i++].last_meal);
        // pthread_create(&philo[i].threads, NULL, &routine, &philo[i++]);
        pthread_create(&philo[i].threads, NULL, &routine, &philo[i]);
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
        philo[i].has_right_fork = false;
        philo[i].has_left_fork = false;
        philo[i].table_data = table;
        philo[i].last_meal = 0;
        pthread_mutex_lock(&table->mutex_thread);
        pthread_create(&philo[i].threads, NULL, &routine, &philo[i++]);
        pthread_mutex_unlock(&table->mutex_thread);
        usleep(1000);
        i++;
    }
    return;
}

t_ms ft_get_time()
{
    struct timeval time;

    gettimeofday(&time, NULL);
    return ((t_ms)time.tv_sec / 1000 + time.tv_usec * 1000);

}

void ft_init_queue(t_table *table, t_error *error)
{
    int i;

    i = 0;
    table->philo_queue = (t_queue *)malloc (sizeof(t_queue));
    table->philo_queue->queue_count = table->num_philo;
    table->philo_queue->priority_queue = (int *)malloc(sizeof(int) * table->num_philo);
    while (i < table->philo_queue->queue_count)
    {
        table->philo_queue->priority_queue[i] = i + 1;
        printf("%d ", table->philo_queue->priority_queue[i++] );
    }
    printf("\n priority queue\n");
    pthread_mutex_init(&table->philo_queue->queue_mutex, NULL);

}

t_table *ft_init_table(int ac, char *av[], t_error *error)
{
    int i;
    t_table *table;

    i = 0;
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
    ft_init_forks(table->num_philo, error);
    ft_init_philo(table, error);
    ft_init_queue(table, error);
    return (table);
}
void ft_exit(t_table *table)
{
    // int i;

    // i = 0;
    // pthread_mutex_destroy(&table->mutex_print);
    // free(table->forks);
    // free(table->philo);
    printf("136 exiting\n ");
}

int main(int ac, char *av[])
{
   int i;
   t_table *table;
   t_error errormsg;
   int next;

   i = 0;
   if(ac == 5 || ac == 6)
   { 
    errormsg = NULL;
    table = ft_init_table(ac, av, &errormsg);
    if (errormsg)
        {
            ft_exit(table);
            printf("170%s", errormsg);
            return (0);
        }
    printf("num of philos = %d\n", table->num_philo);
    printf("time to die = %d\n", table->time_to_die);
    printf("time to eat = %d\n", table->time_to_eat);
    printf("time to sleep = %d\n", table->time_to_sleep);
    printf("start time = %u\n", table->start_time);
    // ft_init_threads(table);
    // while (!table->philo_dead)
    // {
    //     pthread_mutex_lock(&table->philo_queue->queue_mutex);
    //     next = table->philo_queue->priority_queue[0];
    //     if(ft_get_forks(table, next))
    //         ft_pop_queue(table->philo_queue);
    //     pthread_mutex_unlock(&table->philo_queue->queue_mutex);
    //     usleep(1000);
    // }
    // ft_pop_queue(table->philo_queue);
    // ft_add_to_queue(table->philo_queue, ('9' - '0'));
    while (i < table->num_philo)
    {
        printf("philo%d data\n", table->philo[i].id);
        printf("meals eaten %d \n", table->philo[i].meals_eaten);
        printf("state%d \n", table->philo[i].state);
        printf("last meal%d \n", table->philo[i++].last_meal);
    }

   }
}
    