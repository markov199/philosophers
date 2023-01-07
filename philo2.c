#include"philo.h"

void	ft_print_msg(t_philo *philo, char *str);
void	ft_pick_forks(t_philo *philo);
void	ft_drop_forks(t_philo *philo);
int		ft_check_dead(t_philo *philo);
void	ft_usleep(t_philo *philo, t_ms time);
void	ft_exit(t_table *table);
t_ms	ft_get_time();

void ft_print_msg(t_philo *philo, char *str)
{
    t_table *table;

    table = philo->table_data;
	

    pthread_mutex_lock(&(table->mutex_print));

	printf("[%d]philo[%d]%s",philo->time_last_action - table->start_time, philo->id, str);
    usleep(100);
    pthread_mutex_unlock(&(table->mutex_print));
}

void ft_usleep(t_philo *philo, t_ms sleep_time)
{
	t_table *table;
	t_ms	time;

	table = philo->table_data;
	time = ft_get_time();
	while (1)
	{
		if (table->philo_dead == true)
			return;
		ft_check_dead(philo);
		if ((ft_get_time() - time) >= sleep_time)
			return;
	}
}

void ft_think(t_philo *philo)
{
    t_table *table;

	if (ft_check_dead(philo) == -1)
		return ;
    table = philo->table_data;
    if (philo->state != 1)
    {
        philo->state = 1;
        philo->time_last_action = ft_get_time();
        ft_print_msg(philo, "is thinking\n");
    }
    ft_usleep(philo, 1);
}

void ft_sleep(t_philo *philo)
{
    t_table *table;

	if (ft_check_dead(philo) == -1)
		return ;
    table = philo->table_data;
    philo->state = 4;
    philo->time_last_action = ft_get_time() ;
    ft_print_msg(philo, "is sleeping\n");
    ft_usleep(philo, table->time_to_sleep);
}


void ft_eat(t_philo *philo)
{
    t_table *table;

	if (ft_check_dead(philo) == -1)
		return ;
    table = philo->table_data;
    philo->state = 3;
    philo->meals_eaten++;
    philo->time_last_action = ft_get_time();
    ft_print_msg(philo, "is eating\n");
    ft_usleep(philo, table->time_to_eat );
    philo->last_meal = ft_get_time();
	ft_drop_forks(philo);
}

void ft_pick_forks(t_philo *philo)
{
    t_table *table;

    table = philo->table_data;
	if (ft_check_dead(philo) == -1)
		return ;      
	pthread_mutex_lock(&table->mutex_thread);
	if(table->forks[philo->id -1].used != 0 
		|| table->forks[philo->id % table->num_philo].used != 0)
		pthread_mutex_unlock(&table->mutex_thread);
	else
	{
		if(table->forks[philo->id - 1].wait != philo->id 
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

void ft_drop_forks(t_philo *philo)
{
	t_table *table;

    table = philo->table_data;
	if (ft_check_dead(philo) == -1)
		return ;
    if ( philo->has_fork == true)
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

int ft_check_dead(t_philo *philo)
{
	t_table *table;
	t_ms	time;

	table = philo->table_data;
	time = ft_get_time() ;
	pthread_mutex_lock(&table->mutex_check);
	if (table->philo_dead == true)
	{
		pthread_mutex_unlock(&table->mutex_check);
		return (-1);
	}
	if ((time - philo->last_meal) >= table->time_to_die)
	{
		table->philo_dead = true;
		philo->time_last_action = ft_get_time();
		pthread_mutex_unlock(&table->mutex_check);
		ft_print_msg(philo, "is dead\n");
		return(-1);
	}
	pthread_mutex_unlock(&table->mutex_check);
	return (0);
}

void *routine(void *args)
{
    t_philo *philo;
	t_table *table;

    philo = (t_philo *)args;
	table = philo->table_data;
	if (philo->id % 2 != 0)
		ft_sleep(philo);
	while((table->philo_dead == false) && (table->max_meals - philo->meals_eaten != 0))
	{
    	ft_pick_forks(philo);
		if(philo->has_fork)
		{
			ft_eat(philo);
			if (table->max_meals - philo->meals_eaten == 0)
            return (NULL);
		}   
		else
		{
			
		
			ft_think(philo);
			continue;
		}
		
		ft_sleep(philo);		
	}
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
        table->max_meals = -1;
    return;

}
void ft_init_forks(t_table *table, t_error *error)
{
    int i;
    
    i = 0;
    table->forks = calloc(sizeof(t_fork), table->num_philo);
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

void ft_init_philo(t_table *table, t_error *error)
{
    t_philo *philo;
    int i;

    i = 0;
	(void)error;
    philo = calloc(sizeof(t_philo), table->num_philo);
    while (i < table->num_philo)
    {
        philo[i].id = i + 1;
        philo[i].meals_eaten = 0;
        philo[i].state = 0;
        philo[i].has_fork = false;
        philo[i].table_data = table;
        philo[i].last_meal = 0;
		philo[i].time_start = 0;
		philo[i].time_last_action = 0;
		philo[i].threads = NULL;
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


t_table *ft_init_table(t_table *table, int ac, char *av[], t_error *error)
{
    
    ft_get_args(table, ac, av, error);
    if (*error)
	{
		printf("%s", *error);
	return (table);
	}
    table->philo_dead = false;
    table->start_time = ft_get_time();

    if(pthread_mutex_init(&table->mutex_print, NULL))
        *error = INIT_PRINT;
    if(pthread_mutex_init(&table->mutex_thread, NULL))
        *error = INIT_THREAD;
	if(pthread_mutex_init(&table->mutex_check, NULL))
        *error = INIT_THREAD;
    ft_init_forks(table, error);
    ft_init_philo(table, error);
    return (table);
}

void ft_exit(t_table *table)
{
	if (table->forks)
		free(table->forks);
    if (table->philo)
		free(table->philo);
    if (table)
        free (table);
}

int main(int ac, char *av[])
{
   t_table *table;
   t_error errormsg;

   table = calloc(sizeof(t_table), 1);
   if(ac == 5 || ac == 6)
   { 
        errormsg = NULL;
        ft_init_table(table, ac, av, &errormsg);
        if (errormsg) 
            return (0);
        ft_init_threads(table);
		pthread_mutex_destroy(&table->mutex_print);
		pthread_mutex_destroy(&table->mutex_thread);
		pthread_mutex_destroy(&table->mutex_check);

    }
	else
		write (2, "Invalid number of arguments\n", 28);
    ft_exit(table);
    return (0);
}

 /*   
int	ft_wait_time(t_philo *philo, long unsigned int time)
{
	long unsigned int	t;

	t = ft_get_time();
	while (1)
	{
		// pthread_mutex_lock(&philo->table_data->mutex_check);
		// if (philo->table_data->any_dead > 0)
		// {
		// 	pthread_mutex_unlock(&philo->table_data->mutex_check);
		// 	return (-1);
		// }
		pthread_mutex_unlock(&philo->table_data->mutex_check);
		if (ft_check_dead(philo) == -1)
			return (-1);
		if ((ft_get_time() - t) >= time)
			return (0);
		usleep(100);
	}
	return (0);
}*/