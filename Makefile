NAME = test
S_DIR = .
SRCS = $(foreach dir, ${S_DIR}, ${wildcard $(dir)/*.c})
OBJS = ${SRCS:.c=.o}

FLAGS = -Wall -Wextra -Werror


.c.o:
	gcc ${FLAGS} -c $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	gcc ${FLAGS} ${OBJS} -o ${NAME} -lreadline

all: ${NAME}

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ${NAME}

re: fclean ${NAME}

.PHONY: all, clean, fclean, re, bonus