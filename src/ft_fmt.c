#include "ft_printf.h"
#include <stdio.h>

void initialize_fmt(t_fmt *fmt) {
  fmt->flag_len = 0;
  fmt->flag_mask = 0;
  fmt->conversion = 0;
  fmt->original_len = 0;
  fmt->original = NULL;
  fmt->precision_len = 0;
}

void fmt_bstring(t_fmt *fmt, char *buffer, char *arg, long long arg_len)
{
  int	index;

  if (fmt->flag_mask & FLAG_DASH_MASK) {
    index = ft_strlcat(buffer, arg, STRING_ARG_BUFFER_SIZE);
    // printf("%lli\n", fmt->flag_len - arg_len);
    while (fmt->flag_len - arg_len++ > 0)
      buffer[index++] = ' ';
    write(1, buffer, index);
  } else if (fmt->flag_mask & FLAG_ZERO_MASK) {
    while (fmt->flag_len - arg_len > 0)
      buffer[index++] = ' ';
    index = ft_strlcat(buffer, arg, STRING_ARG_BUFFER_SIZE);
    write(1, buffer, index);
  }
}

void fmt_mstring(t_fmt *fmt, char *buffer, char *arg, size_t arg_len)
{
}

void fmt_string(t_fmt *fmt, va_list *v_arg)
{
  char		buffer[STRING_ARG_BUFFER_SIZE];
  char		*mbuffer;
  char		*str_arg;
  size_t	arg_len;

  str_arg = va_arg(*v_arg, char *);
  arg_len = ft_strlen(str_arg);
  if (arg_len + fmt->flag_len > STRING_ARG_BUFFER_SIZE)
  {
    mbuffer = malloc(arg_len + 1);
    fmt_mstring(fmt, mbuffer, str_arg, arg_len);
  }
  else
  {
    ft_bzero(buffer, STRING_ARG_BUFFER_SIZE);
    fmt_bstring(fmt, buffer, str_arg, arg_len);
  }
}

void print_fmt(t_fmt *fmt, va_list *v_arg) {
  if (fmt->flag_mask & FLAG_VALID_MASK) {
    write(1, fmt->original, fmt->original_len);
    return;
  }
  if (fmt->conversion == 's')
    fmt_string(fmt, v_arg);
  // printf("fmt len = %lu\n", fmt->flag_len);
  // printf("%b", fmt->flag_mask);
}
