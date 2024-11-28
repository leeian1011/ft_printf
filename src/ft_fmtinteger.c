#include "ft_printf.h"
#include <stdio.h>

static void fmtint_intplaces(t_fmt *fmt, long integer, long *arg_len)
{
  if (integer < 0)
    integer = -integer;
  *arg_len = 0;
  if (integer == 0)
    (*arg_len)++;
  else {
    while (integer > 0) {
      (*arg_len)++;
      integer /= 10;
    }
  }
}

static size_t fmtint_establish_sign(t_fmt *fmt, char *buffer, size_t *buffer_offset)
{
  size_t	count;

  count = 0;
  if (fmt->flag_mask & FLAG_PLUS_MASK)
  {
    if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK)
      buffer[(*buffer_offset)++] = '-';
    else
      buffer[(*buffer_offset)++] = '+';
    count = 1;
  }
  else if (fmt->flag_mask & FLAG_SPACE_MASK)
  {
    if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK)
      buffer[(*buffer_offset)++] = '-';
    else
      buffer[(*buffer_offset)++] = ' ';
    count = 1;
  }
  else if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK)
  {
    buffer[(*buffer_offset)++] = '-';
    count = 1;
  }
  return (count);
}

static void	fmtint_establish_conversion(t_fmt *fmt, long integer, char *buffer, size_t buffer_size, size_t *buffer_offset)
{
  int	precision_len;
  long	arg_len;
  char	*int_str;
  int	sign;

  fmtint_intplaces(fmt, integer, &arg_len);
  int_str = ft_itoa(integer);
  if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK)
    int_str++;
  sign = fmtint_establish_sign(fmt, buffer, buffer_offset);
  if (fmt->precision_len > 0)
  {
    precision_len = fmt->precision_len;
    while (precision_len-- - arg_len > 0)
    {
      buffer[(*buffer_offset)++] = '0';
      fmt->width_len--;
    }
  }
  if (fmt->precision_len != 0 || integer != 0)
    *buffer_offset = ft_strlcat(buffer, int_str, buffer_size);
  fmt->width_len -= arg_len + sign;
  if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK)
    int_str--;
  free(int_str);
}

static int fmtint_print_specifier(t_fmt *fmt, char *buffer, long integer, long arg_len, size_t size)
{
  size_t index;

  index = 0;
  if (fmt->precision_len > arg_len)
    arg_len = fmt->precision_len;
  if (fmt->flag_mask & FLAG_DASH_MASK)
  {
    fmtint_establish_conversion(fmt, integer, buffer, size, &index);
    while (fmt->width_len-- > 0)
      buffer[index++] = ' ';
  }
  else if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK || fmt->width_len > 0)
  {
    while (fmt->width_len > 0 && fmt->width_len-- - arg_len > 0)
      buffer[index++] = ' ';
    fmtint_establish_conversion(fmt, integer, buffer, size, &index);
  }
  else if (fmt->flag_mask & FLAG_ZERO_MASK || fmt->flag_mask & FLAG_DOT_MASK)
  {
    while (fmt->width_len > 0 && fmt->width_len-- - arg_len > 0)
      buffer[index++] = '0';
    fmtint_establish_conversion(fmt, integer, buffer, size, &index);
  }
  write(1, buffer, index);
  return (index);
}

int fmt_integer(t_fmt *fmt, va_list *v_arg)
{
  char buffer[ARG_BUFFER_SIZE];
  char *mbuffer;
  long integer;
  long arg_len;

  integer = va_arg(*v_arg, int);
  fmtint_intplaces(fmt, integer, &arg_len);
  if (fmt->flag_mask == 0)
  {
    ft_putnbr_fd(integer, 1);
    return (arg_len);
  }
  if (integer < 0)
    fmt->flag_mask |= FLAG_NEGATIVE_INTEGER_MASK;
  if (fmt->width_len >= ARG_BUFFER_SIZE)
  {
    mbuffer = ft_calloc(fmt->width_len + 1, 1);
    if (!mbuffer)
      return (0);
    int x = fmtint_print_specifier(fmt, buffer, integer, arg_len, fmt->width_len + 1);
    free(mbuffer);
    return (x);
  }
  else
  {
    ft_bzero(buffer, ARG_BUFFER_SIZE);
    return (fmtint_print_specifier(fmt, buffer, integer, arg_len, ARG_BUFFER_SIZE));
  }
}
