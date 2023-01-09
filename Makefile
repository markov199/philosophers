# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mkovoor <mkovoor@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/06 16:53:13 by mkovoor           #+#    #+#              #
#    Updated: 2023/01/09 07:38:14 by mkovoor          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		= philo

CC			= gcc
CFLAGS		= -g3 -Wall -Wextra -Werror -pthread -fsanitize=address
#CFLAGS		= -g3 -pthread
#CFLAGS		= -g -Wall -Wextra -Werror -pthread
RM			= rm -rf

SRCS		= philo.c utils.c actions.c init.c lib_func.c
				

OBJS		= $(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
			@printf "\x1B[32m-> Executable Ready! <-\x1B[0m\n"
			
clean:
			@$(RM) $(OBJS)
			@printf "\x1B[35m-> *.o cleaned! <-\x1B[0m\n"

fclean:		clean
			@$(RM) $(NAME) $(NAME_BONUS)
			@printf "\x1B[36m-> Executable Deleted! <-\x1B[0m\n"

re:			fclean all

.PHONY:		all clean fclean re
