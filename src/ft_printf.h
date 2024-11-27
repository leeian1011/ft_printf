#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# define CONVERSIONS "cspdiuxX%"
# define FLAGS "+# -0."
# define FLAG_PLUS_MASK 128
# define FLAG_HASH_MASK 64
# define FLAG_SPACE_MASK 32
# define FLAG_DASH_MASK 16
# define FLAG_ZERO_MASK 8
# define FLAG_DOT_MASK 4
# define FLAG_VALID_MASK 1
# define STRING_ARG_BUFFER_SIZE 4096 

# include "../includes/libft/libft.h"
# include <stdarg.h>

typedef struct s_fmt {
  char 					conversion;
  const char		*original;
  unsigned char	flag_mask;
  long long			flag_len;
  size_t				original_len;
  size_t				precision_len;
}	t_fmt;

void	initialize_fmt(t_fmt *fmt);
void	print_fmt(t_fmt *fmt, va_list *v_arg);

int		ft_printf(const char *str, ...);
#endif
