#include<philo.h>

int ft_atoi(int *number, char *str, t_error *error)
{
    int i;
    int sign;

    i = 0;
    sign = 0;
    if (str[sign] == '-')
            *error = INVALID_ARGS;
    else if (str[sign] == '+')
        sign = 1;
    i = sign;
    while (str[i] && !*error)
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
