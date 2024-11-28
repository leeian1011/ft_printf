#include "ft_printf.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/// Set precision mask

static void handle_conversions(t_fmt *fmt, const char **str) {
  char *conversion;

  conversion = ft_strchr(CONVERSIONS, **str);
  if (!conversion)
    fmt->flag_mask |= FLAG_VALID_MASK;
  else
    fmt->conversion = *conversion;
  fmt->original_len += 1;
  (*str)++;
}

static void update_flag(t_fmt *fmt, unsigned char flag_mask, const char **str) {
  fmt->flag_mask |= flag_mask;
  (*str)++;
}

static void handle_width(t_fmt *fmt, const char **str)
{
  fmt->width_len = ft_atoi(*str);
  while (ft_isdigit(**str))
    (*str)++;
}

static void handle_precision(t_fmt *fmt, const char **str)
{
  fmt->flag_mask |= FLAG_DOT_MASK;
  fmt->precision_len = ft_atoi(++*str);
  while (ft_isdigit(**str))
    (*str)++;
}

void handle_flag(t_fmt *fmt, const char **str)
{
  char *flag;

  while (**str) {
    flag = ft_strchr(FLAGS, **str);
    if (!flag && is_nonzero_digit(**str))
    {
      handle_width(fmt, str);
      continue;
    }
    else if (!flag)
      break;
    if (*flag == '+')
      update_flag(fmt, FLAG_PLUS_MASK, str);
    else if (*flag == '#')
      update_flag(fmt, FLAG_HASH_MASK, str);
    else if (*flag == ' ')
      update_flag(fmt, FLAG_SPACE_MASK, str);
    else if (*flag == '-')
      update_flag(fmt, FLAG_DASH_MASK, str);
    else if (*flag == '0')
      update_flag(fmt, FLAG_ZERO_MASK, str);
    else if (*flag == '.')
      handle_precision(fmt, str);
  }
}

int handle_specifiers(const char **str, va_list *v_arg) {
  t_fmt fmt;
  char *flag;
  int count;

  count = 0;
  initialize_fmt(&fmt);
  fmt.original = (*str)++;
  handle_flag(&fmt, str);
  fmt.original_len += *str - fmt.original;
  handle_conversions(&fmt, str);
  print_fmt(&fmt, v_arg);
  return (count);
}

int ft_printf(const char *str, ...) {
  int count;
  size_t printable;
  va_list v_arg;
  const char *iterator;

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
      break;
    count += handle_specifiers(&iterator, &v_arg);
    str = iterator;
  }
  va_end(v_arg);
  return (count);
}
