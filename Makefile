NAME = minishell
S_DIR = srcs srcs/ft_exec_ast srcs/parser_lexer srcs/builtins srcs/params srcs/signals
SRCS = $(foreach dir, ${S_DIR}, ${wildcard $(dir)/*.c})
OBJS = ${SRCS:.c=.o}

H_DIR = includes
LFT = lib/libft
#ifdef MACOS_CATALINA
#	LIBFLAGS = -Llib/libft -lft -L/usr/local/opt/readline/lib -lreadline
#else
	LIBFLAGS = -Llib/libft -lft -L${HOME}/.brew/opt/readline/lib -lreadline
#endif

#ifdef MACOS_CATALINA
#	INCLUDEFLAGS = -I/usr/local/opt/readline/include -I${H_DIR}
#else
	INCLUDEFLAGS = -I${HOME}/.brew/opt/readline/include -I${H_DIR}
#endif
FLAGS = -Wall -Wextra -Werror #-g -fsanitize=address

.c.o:
	gcc ${FLAGS} ${INCLUDEFLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	make -C ${LFT}
	gcc ${FLAGS} ${LIBFLAGS} ${OBJS} -o ${NAME}

all: ${NAME}

clean:
	make clean -C ${LFT}
	rm -f ${OBJS}

fclean: clean
	make fclean -C ${LFT}
	rm -f ${NAME}

re: fclean ${NAME}

install:
	brew install readline

.PHONY: all, clean, fclean, re, bonus