NAME = minishell
S_DIR = srcs srcs/ft_exec_ast srcs/parser_lexer
SRCS = $(foreach dir, ${S_DIR}, ${wildcard $(dir)/*.c})
OBJS = ${SRCS:.c=.o}

H_DIR = includes
LFT = lib/libft
LIBFLAGS = -Llib/libft -lft
FLAGS = -Wall -Wextra -Werror -I ${H_DIR} #-g -fsanitize=address

.c.o:
	gcc ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	make -C ${LFT}
	gcc ${FLAGS} ${LIBFLAGS} ${OBJS} -o ${NAME} -lreadline

all: ${NAME}

clean:
	make clean -C ${LFT}
	rm -f ${OBJS}

fclean: clean
	make fclean -C ${LFT}
	rm -f ${NAME}

re: fclean ${NAME}

.PHONY: all, clean, fclean, re, bonus