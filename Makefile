NAME=libftprintf.a
CC = cc
ARC = ar rc
SOURCE = src/ft_printf.c src/ft_fmthex.c src/ft_fmtstr.c src/ft_fmtptr.c src/ft_fmtint.c src/ft_fmt.c src/ft_ltoa.c
LIBFT_PATH = ./includes/libft/
OBJECT = $(SOURCE:.c=.o)
TEST_TYPE = str int ptr hex

all: $(NAME)

$(NAME): $(OBJECT) 
	make re -C $(LIBFT_PATH)
	ar rcs $(@) $(^) $(LIBFT_PATH)*.o

%.o: %.c
	$(CC) -Wall -Wextra -Werror -c $(<) -o $(@)

clean:
	rm -rf $(OBJECT) 

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_PATH)

re: fclean $(NAME)

bonus: all

$(TEST_TYPE): %: $(NAME) 
	$(CC) -Wall -Wextra -Werror test_$(@).c $(NAME)

.PHONY:
	libft.a all clean fclean re
