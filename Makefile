NAME=libftprintf.a
CC = cc
ARC = ar rc
SOURCE = ft_printf.c

all: $(NAME)

$(NAME): libft
	$(CC) src/ft_printf.c ./libft.a

libft:
	cd includes/libft; make; mv libft.a ../..

.PHONY:
	libft
