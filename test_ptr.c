#include <stdio.h>
#include "./src/ft_printf.h"
#include <limits.h>

int main()
{
  // char *x;
  // char **y = &x;
  // printf("%d\n", ft_strncmp("10", "0", 2));
  // printf("%d\n", printf(" %p \n", 16));
  // printf("%d\n", ft_printf(" %p \n", 16));
  // printf("%d\n",printf(" %p \n", ULLONG_MAX));
  // printf("%d\n", ft_printf(" %p \n", ULLONG_MAX));
  // printf("%d\n", printf(" %p \n", LONG_MIN));
  // printf("%d\n", ft_printf(" %p \n", LONG_MIN));
  // printf("%d\n", printf(" %.2p \n", LONG_MIN));
  // printf("%d\n", ft_printf(" %.2p \n", LONG_MIN));
  // printf("%d\n", printf(" %-25p \n", NULL));
  // printf("%d\n", ft_printf(" %-25p \n", NULL));
  // printf("%d\n", printf("%p\n", NULL));
  // printf("%d\n", ft_printf("%p\n", NULL));
  // printf("%d\n", printf("%p\n", &y));
  // printf("%d\n", ft_printf("%p\n", &y));
	// printf(" %p \n", -1);
	// ft_printf(" %p \n", -1);
	printf(" %-2p \n", (void *)1);
	ft_printf(" %-2p \n", 1);
	// printf(" %p \n", 15);
	// ft_printf(" %p \n", 15);
	// printf(" %p \n", 16);
	// ft_printf(" %p \n", 16);
}

