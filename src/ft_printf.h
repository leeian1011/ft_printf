/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:28:06 by ialee             #+#    #+#             */
/*   Updated: 2024/12/02 21:36:57 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define FLAG_NEGATIVE_INTEGER_MASK 2
# define FLAG_VALID_MASK 1
# define FLAG_ZERO_DOT_MASK 12
# define ARG_BUFFER_SIZE 4096 
# define HEX_MAX_LENGTH 8
# define HEX_PREFIX "0x"

# include "../includes/libft/libft.h"
# include <stdarg.h>
# include <sys/types.h> 

typedef struct s_fmt {
	char			conversion;
	const char		*original;
	unsigned char	flag_mask;
	size_t			width_len;
	size_t			original_len;
	size_t			precision_len;
	char			*buf;
}	t_fmt;

/// fmt
void	initialize_fmt(t_fmt *fmt);
size_t	num_places(long num);
int		is_nonzero_digit(char c);
int		fmt_string(t_fmt *fmt, char *str);
int		fmt_char(t_fmt *fmt, char c);
int		fmt_ptr(t_fmt *fmt, ulong ptr);
int		fmt_integer(t_fmt *fmt, long integer);
int		fmt_hex(t_fmt *fmt, uint hex);
int		print_fmt(t_fmt *fmt, va_list *v_arg);

/// Long to Ascii
char	*ft_ltoa(long num);

/// Specifier handling for ft_printf (util). 
int		handle_specifiers(const char **str, va_list *v_arg);

int		ft_printf(const char *str, ...);
#endif
