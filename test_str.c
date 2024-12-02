#include <stdio.h>
#include "./src/ft_printf.h"
#include <limits.h>

void set_static_str(char **str)
{
  static char *emp;
  static char *nuller;

  if (!emp)
    emp = "";
  if (!nuller)
    nuller = "(null)";
  if (!str)
    *str = nuller;
  else
    *str = emp;
}

int main()
{
  // printf("%-25.2c\n", 'c');
  // ft_printf(" %-s ", "-");
  printf("rt-> %1s\n", "");
  ft_printf("ft-> %1s\n", "");
  // char *x = "";
  // printf("%d\n", *x);
  // printf("%d\n", ft_printf("%c %c %c\n", '0', '2', '1'));
  // ft_printf("%d %d %d\n", 123, 'a', 'A');
  // printf("%d\n", ft_printf("%d %c %c\n", 123, '2', '1'));
  // ft_printf("%c %c %c\n", '0', '2', '3');
  // ft_printf("%c\n", 'h');
  // printf("%.9s\n", NULL);
  // ft_printf("%.9s\n", NULL);
  // printf("%.6s\n", NULL);
  // ft_printf("%.6s\n", NULL);
  // printf("%.s\n", NULL);
  // ft_printf("%.s\n", NULL);
  // printf("%.9s\n", "chill guy");
  // ft_printf("%.9s\n", "chill guy");
  // printf("%#0 8.9s\n", "chill guy");
  // ft_printf("%#0 8.9s\n", "chill guy");
  // printf("%##0 8.9s\n", "chill guy");
  // ft_printf("%##0 8.9s\n", "chill guy");
  // printf("%#- .8.9s\n", "chill guy");
  // ft_printf("%#- .8.9s\n", "chill guy");
  // printf("%-4096.9s\n", "chill guy");
  // ft_printf("%-4096.9s\n", "chill guy");
  // printf("hellothere t#$%-.3sfahfah-fahfahfah\n", "chill guy");
  // ft_printf("hellothere t#$%-.3sfahfah-fahfahfah\n", "chill guy");
  // printf("%4096.9s\n", "chill guy");
  // ft_printf("%4096.9s\n", "chill guy");
  // printf("%+-0.9s\n", "chill guy");
  // ft_printf("%+-0.9s\n", "chill guy");
  // printf("%-1004096.9s\n", "chill guy");
  // ft_printf("%-1004096.9s\n", "chill guy");
}
