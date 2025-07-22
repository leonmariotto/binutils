# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsimanau <lsimanau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/13 20:17:20 by llenotre          #+#    #+#              #
#    Updated: 2022/06/28 11:40:48 by lmariott         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = liblst.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC =	\
	ft_lstdelone.c\
	default_del.c\
	ft_lstrev.c\
	ft_lstpush.c\
	ft_lstnew_nodup.c\
	ft_lstback.c\
	ft_lstnew.c\
	ft_lstget.c\
	ft_lstmap.c\
	ft_lstsize.c\
	ft_lstadd.c\
	ft_lstdel.c\
	ft_lstiter.c\
	ft_lstswap.c

HDR = liblst.h
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rc $(NAME) $(OBJ)
	rm -rf $(OBJ)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
