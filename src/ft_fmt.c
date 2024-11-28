#include "ft_printf.h"
#include <stdio.h>

void initialize_fmt(t_fmt *fmt) {
  fmt->width_len = 0;
  fmt->flag_mask = 0;
  fmt->conversion = 0;
  fmt->original_len = 0;
  fmt->original = NULL;
  fmt->precision_len = 0;
}

int	fmt_percentage(t_fmt *fmt, va_list *v_arg)
{
  write(1, "%", 1);
  return (1);
}

int	is_nonzero_digit(char c)
{
  return (c > '0' && c <= '9');
}

int	print_fmt(t_fmt *fmt, va_list *v_arg) {
  if (fmt->flag_mask & FLAG_VALID_MASK) {
    write(1, fmt->original, fmt->original_len);
    return (fmt->original_len);
  }
  // if (fmt->conversion == 'c')
    // return(fmt_char(fmt, v_arg));
  if (fmt->conversion == 's')
    return(fmt_string(fmt, v_arg));
  // else if (fmt->conversion == 'p')
  //   return(fmt_ptr(fmt, v_arg));
  else if (fmt->conversion == 'd' || fmt->conversion == 'i')
    return (fmt_integer(fmt, v_arg));
  // else if (fmt->conversion == 'u')
  //   return (fmt_uinteger(fmt, v_arg));
  // else if (fmt->conversion == 'x' || fmt->conversion == 'X')
  //   return (fmt_hex(fmt, v_arg));
  else
    return (fmt_percentage(fmt, v_arg));
}
