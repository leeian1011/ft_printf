#include "ft_printf.h"
#include <stdio.h>
#include <stdlib.h>

static char	*fmthex_itoh(unsigned int hex)
{
  char	*ahex;
  int		index;
  unsigned int borrow_hex;

  index = 0;
  if (hex == 0)
    index = 1;
  borrow_hex = hex;
  while (borrow_hex > 0)
  {
    borrow_hex /= 16;
    index++;
  }
  ahex = ft_calloc(index + 1, 1);
  if (!ahex)
    return (NULL);
  ahex[index--] = '\0';
  while (hex >= 16)
  {
    ahex[index--] = "0123456789abcdef"[hex % 16];
    hex /= 16;
  }
  ahex[index--] = "0123456789abcdef"[hex % 16];
  return (ahex);
}

static int	fmthex_initialize(t_fmt* fmt, char **ahex, unsigned int hex, size_t *hex_len)
{
  size_t	index;

  index = 0;
  *ahex = fmthex_itoh(hex);
  *hex_len = ft_strlen(*ahex);
  if (fmt->width_len == 0 && fmt->flag_mask == 0)
  {
    if (fmt->conversion == 'X')
    {
      while ((*ahex)[index])
      {
        (*ahex)[index] = ft_toupper((*ahex)[index]);
        index++;
      }
    }
    write(1, *ahex, *hex_len);
    free(*ahex);
    return (1);
  }
  return (0);
}

static void	fmthex_establish_conversion(t_fmt *fmt, char *hex, char *buffer, size_t buffer_size, size_t *buffer_offset)
{
  long	hex_len;
  long	copy_hex_len;
  long	copy_precision_len;

  hex_len = ft_strlen(hex);
  copy_precision_len = fmt->precision_len;
  if (fmt->precision_len > 0)
  {
    while (copy_precision_len-- - hex_len > 0)
    {
      buffer[(*buffer_offset)++] = '0';
      fmt->width_len--;
    }
  }
  if ((fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len == 0) && ft_strncmp(hex, "0", hex_len) == 0)
    fmt->width_len++;
  else	
    *buffer_offset = ft_strlcat(buffer, hex, buffer_size);
  if (fmt->conversion == 'X')
  {
    copy_hex_len = ft_strlen(buffer);
    while (copy_hex_len--)
      buffer[copy_hex_len] = ft_toupper(buffer[copy_hex_len]);
  }
  fmt->width_len -= hex_len;
}

static int fmthex_print_specifier(t_fmt *fmt, char *buffer, char *integer, size_t hex_len, size_t size)
{
  size_t index;

  index = 0;
  if (fmt->precision_len > hex_len)
    hex_len = fmt->precision_len;
  if (fmt->flag_mask & FLAG_HASH_MASK && ft_strncmp(integer, "0", hex_len) != 0)
    index = ft_strlcat(buffer, HEX_PREFIX, size);
  if (fmt->flag_mask & FLAG_DASH_MASK)
  {
    fmthex_establish_conversion(fmt, integer, buffer, size, &index);
    if ((long)fmt->width_len < 0)
      fmt->width_len = 0;
    while (fmt->width_len-- > 0)
      buffer[index++] = ' ';
    return (write(1, buffer, index));
  }
  else if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK)
    while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
      buffer[index++] = ' ';
  else if (fmt->flag_mask & FLAG_ZERO_MASK)
    while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
      buffer[index++] = '0';
  fmthex_establish_conversion(fmt, integer, buffer, size, &index);
  return (write(1, buffer, index));
}


int fmt_hex(t_fmt *fmt, unsigned int hex)
{
  char buffer[ARG_BUFFER_SIZE];
  char *mbuffer;
  char	*ahex;
  size_t hex_len;

  if (fmthex_initialize(fmt, &ahex, hex, &hex_len))
    return (hex_len);
  if (fmt->width_len >= ARG_BUFFER_SIZE)
  {
    mbuffer = ft_calloc(fmt->width_len + 1, sizeof(char));
    if (!mbuffer)
      return (0);
    hex_len = fmthex_print_specifier(fmt, buffer, ahex, hex_len, fmt->width_len + 1);
    free(mbuffer);
  }
  else
  {
    ft_bzero(buffer, ARG_BUFFER_SIZE);
    hex_len = fmthex_print_specifier(fmt, buffer, ahex, hex_len, ARG_BUFFER_SIZE);
  }
  free(ahex);
  return (hex_len);
}
