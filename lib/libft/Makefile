# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmercadi <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/24 10:58:50 by dmercadi          #+#    #+#              #
#    Updated: 2021/03/24 16:07:34 by dmercadi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_isalnum.c \
		ft_isalpha.c \
		ft_isascii.c \
		ft_isdigit.c \
		ft_isprint.c \
		ft_memccpy.c \
		ft_memchr.c \
		ft_memcmp.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_bzero.c \
		ft_memset.c \
		ft_calloc.c \
		ft_strdup.c \
		ft_putchar_fd.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_putnbr_fd.c \
		ft_split.c \
		ft_strlen.c \
		ft_strchr.c \
		ft_strrchr.c \
		ft_strncmp.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_substr.c \
		ft_strjoin.c \
		ft_itoa.c \
		ft_strmapi.c \
		ft_strtrim.c \
		ft_isupper.c \
		ft_islower.c \
		ft_toupper.c \
		ft_tolower.c \
		ft_lst1.c \
		ft_lst2.c \
		ft_strisdigit.c \
		get_next_line.c

OBJS = ${SRCS:.c=.o}

FLAGS = -Wall -Werror -Wextra

CC = gcc

.c.o:
	${CC} ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	ar rc ${NAME} ${OBJS}

all: ${NAME}

clean:
	rm -f ${OBJS} ${BONUS_OBJS}

fclean:	clean
	rm -f ${NAME}

re:	fclean ${NAME}

.PHONY: all clean fclean re
