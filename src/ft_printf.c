#include "ft_printf.h"
#include <stdio.h>

/// Set precision mask
void handle_precision(t_fmt *fmt, const char **str)
{
  fmt->flag_mask |= FLAG_DOT_MASK;
  fmt->precision_len = ft_atoi(++*str);
  while (ft_isdigit(**str))
    (*str)++;
}

void handle_conversions(t_fmt *fmt, const char **str)
{
  char	*conversion;

  conversion = ft_strchr(CONVERSIONS, **str);
  if (!conversion)
    fmt->flag_mask |= FLAG_VALID_MASK;
  else
    fmt->conversion = *conversion;
  fmt->original_len += 1;
  (*str)++;
}

void handle_variable_flag(t_fmt *fmt, const char **str)
{
  if (**str == '-')
  {
    if (fmt->flag_mask & FLAG_DASH_MASK)
      fmt->flag_mask |= FLAG_VALID_MASK;
    fmt->flag_mask |= FLAG_DASH_MASK;
    fmt->flag_len = ft_atoi(++(*str));
  }
  else
  {
    if (fmt->flag_mask & FLAG_ZERO_MASK)
      fmt->flag_mask |= FLAG_VALID_MASK;
    fmt->flag_mask |= FLAG_ZERO_MASK;
    if (!(fmt->flag_mask & FLAG_DASH_MASK))
      fmt->flag_len = ft_atoi(++(*str));
  }
  while (ft_isdigit(**str))
    (*str)++;
}

void handle_flag(t_fmt *fmt, const char **str) {
  char *flag;

  fmt->original = (*str)++;
  while (**str) {
    if (fmt->flag_mask & FLAG_DOT_MASK)
      fmt->flag_mask |= FLAG_VALID_MASK;
    flag = ft_strchr(FLAGS, **str);
    if (!flag)
      break ;
    if (*flag == '+')
      fmt->flag_mask |= FLAG_PLUS_MASK;
    else if (*flag == '#')
      fmt->flag_mask |= FLAG_HASH_MASK;
    else if (*flag == ' ')
      fmt->flag_mask |= FLAG_SPACE_MASK;
    else if (*flag == '-' || *flag == '0')
      handle_variable_flag(fmt, str);
    else if (*flag == '.')
      handle_precision(fmt, str);
    else
      break;
    if (*flag != '-' && *flag != '0')
      (*str)++;
  }
  fmt->original_len += *str - fmt->original;
}

int handle_specifiers(const char **str, va_list *v_arg) {
  t_fmt fmt;
  char *flag;
  int count;

  count = 0;
  initialize_fmt(&fmt);
  // printf("Init fmt\n");
  handle_flag(&fmt, str);
  // printf("Handle flags\n");
  handle_conversions(&fmt, str);
  // printf("Handle conversions\n");
  print_fmt(&fmt, v_arg);
  return (count);
}

int ft_printf(const char *str, ...) {
  int 				count;
  size_t 			printable;
  va_list			v_arg;
  const char	*iterator;

  count = 0;
  iterator = str;
  va_start(v_arg, str);
  while (*str) {
    while (*iterator != '%' && *iterator)
      iterator++;
    // printf("\n%p vs %p\n", &iterator, str);
    printable = iterator - str;
    if (printable)
      write(1, str, printable);
    count += printable;
    if (!*iterator)
      break ;
    count += handle_specifiers(&iterator, &v_arg);
    str = iterator;
  }
  va_end(v_arg);
  return (count);
}
