#include "ft_printf.h"
#include <stdio.h>

static int fmtint_intplaces(t_fmt *fmt, long integer, size_t *arg_len, long *precision_len, char **int_str)
{
  char	*result;

  result = NULL;
  *arg_len = num_places(integer);
  if (fmt->flag_mask == 0 && fmt->width_len == 0)
  {
    result = ft_ltoa(integer);
    write(1, result, *arg_len);
    free(result);
    return (0);
  }
  if (integer < 0)
  {
    fmt->flag_mask |= FLAG_NEGATIVE_INTEGER_MASK;
    integer = -integer;
  }
  if (!!int_str)
    *int_str = ft_ltoa(integer);
  if (!!precision_len)
    *precision_len = fmt->precision_len;
  return (1);
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
  size_t	arg_len;
  long	precision_len;
  char	*int_str;

  int_str = "";
  fmtint_intplaces(fmt, integer, &arg_len, &precision_len, &int_str);
  if (fmt->precision_len > 0 && fmt->flag_mask & FLAG_DOT_MASK)
  {
    if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK)
      arg_len--;
    while ((long)(precision_len-- - arg_len) > 0)
    {
      buffer[(*buffer_offset)++] = '0';
      fmt->width_len--;
    }
  }
  if (fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len == 0 && integer == 0)
    fmt->width_len++;
  else
    *buffer_offset = ft_strlcat(buffer, int_str, buffer_size);
  fmt->width_len -= arg_len;
  free(int_str);
}

static int fmtint_print_specifier(t_fmt *fmt, char *buffer, long integer, size_t arg_len, size_t size)
{
  size_t index;

  index = 0;
  fmtint_establish_sign(fmt, buffer, &index);
  if (fmt->precision_len > arg_len)
    arg_len = fmt->precision_len;
  if (fmt->flag_mask & FLAG_DASH_MASK)
  {
    fmtint_establish_conversion(fmt, integer, buffer, size, &index);
    if ((long)fmt->width_len < 0)
      fmt->width_len = 0;
    while (fmt->width_len-- > 0)
      buffer[index++] = ' ';
    return (write(1, buffer, index));
  }
  else if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK)
    while (fmt->width_len > 0 && (long)(fmt->width_len-- - arg_len) > 0)
      buffer[index++] = ' ';
  else if (fmt->flag_mask & FLAG_ZERO_MASK)
    while (fmt->width_len > 0 && (long)(fmt->width_len-- - arg_len) > 0)
      buffer[index++] = '0';
  fmtint_establish_conversion(fmt, integer, buffer, size, &index);
  return(write(1, buffer, index));
}

int fmt_integer(t_fmt *fmt, long integer)
{
  char buffer[ARG_BUFFER_SIZE];
  char *mbuffer;
  size_t arg_len;

  arg_len = 0;
  mbuffer = NULL;
  if (!fmtint_intplaces(fmt, integer, &arg_len, NULL, NULL))
    return (arg_len);
  if (fmt->width_len >= ARG_BUFFER_SIZE)
  {
    mbuffer = ft_calloc(fmt->width_len + 1, sizeof(char));
    if (!mbuffer)
      return (0);
    arg_len = fmtint_print_specifier(fmt, buffer, integer, arg_len, fmt->width_len + 1);
    free(mbuffer);
  }
  else
  {
    ft_bzero(buffer, ARG_BUFFER_SIZE);
    arg_len = fmtint_print_specifier(fmt, buffer, integer, arg_len, ARG_BUFFER_SIZE);
  }
  return (arg_len);
}
