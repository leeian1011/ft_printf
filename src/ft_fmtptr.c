#include "ft_printf.h"
#include <stdio.h>
#include <stdlib.h>

static char	*fmthex_itoh(unsigned long hex)
{
  char	*ahex;
  int		index;
  unsigned long borrow_hex;

  index = 0;
  if (hex == 0)
    index = 1;
  borrow_hex = hex;
  while (borrow_hex > 0)
  {
    borrow_hex /= 16;
    index++;
  }
  ahex = malloc(index + 1);
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

static int	fmtptr_initialize(t_fmt* fmt, char **ahex, unsigned long hex, size_t *hex_len)
{
  *ahex = fmthex_itoh(hex);
  *hex_len = ft_strlen(*ahex);
  if (fmt->width_len == 0 && fmt->flag_mask == 0)
  {
    if (ft_strncmp(*ahex, "0", *hex_len) == 0)
      *hex_len = write(1, "(nil)", 5);
    else
    {
      *hex_len += write(1, "0x", 2);
      write(1, *ahex, *hex_len - 2);
    }
    free(*ahex);
    return (*hex_len);
  }
  return (0);
}

static void	fmtptr_establish_conversion(t_fmt *fmt, char *hex, char *buffer, size_t buffer_size, size_t *buffer_offset)
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
    copy_hex_len = hex_len;
    while (copy_hex_len--)
      ft_toupper(buffer[copy_hex_len]);
  }
  fmt->width_len -= hex_len;
}

static int fmtptr_print_specifier(t_fmt *fmt, char *buffer, char *integer, size_t ptr_len, size_t size)
{
  size_t index;

  index = 0;
  if (fmt->precision_len > ptr_len)
    ptr_len = fmt->precision_len;
  index += ft_strlcpy(buffer, HEX_PREFIX, size);
  fmt->width_len -= 2;
  if (fmt->flag_mask & FLAG_DASH_MASK)
  {
    fmtptr_establish_conversion(fmt, integer, buffer, size, &index);
    while ((long)fmt->width_len-- > 0)
      buffer[index++] = ' ';
  }
  else if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK)
  {
    while (fmt->width_len > 0 && (long)(fmt->width_len-- - ptr_len) > 0)
      buffer[index++] = ' ';
    fmtptr_establish_conversion(fmt, integer, buffer, size, &index);
  }
  else
    fmtptr_establish_conversion(fmt, integer, buffer, size, &index);
  if (ft_strncmp(buffer, "0x0", ft_strlen(buffer)) == 0)
    return(write(1, "(nil)", 5));
  else
    return(write(1, buffer, index));
}


int fmt_ptr(t_fmt *fmt, unsigned long ptr)
{
  char buffer[ARG_BUFFER_SIZE];
  char *mbuffer;
  char	*aptr;
  size_t ptr_len;

  if (fmtptr_initialize(fmt, &aptr, ptr, &ptr_len))
    return (ptr_len);
  if (fmt->width_len >= ARG_BUFFER_SIZE)
  {
    mbuffer = ft_calloc(fmt->width_len + 1, sizeof(char));
    if (!mbuffer)
      return (0);
    ptr_len = fmtptr_print_specifier(fmt, mbuffer, aptr, ptr_len, fmt->width_len + 1);
    free(mbuffer);
  }
  else
  {
    ft_bzero(buffer, ARG_BUFFER_SIZE);
    ptr_len = fmtptr_print_specifier(fmt, buffer, aptr, ptr_len, ARG_BUFFER_SIZE);
  }
  free(aptr);
  return (ptr_len);
}
