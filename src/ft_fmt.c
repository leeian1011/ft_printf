/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 19:48:12 by ialee             #+#    #+#             */
/*   Updated: 2024/12/02 21:34:45 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/// Instantiates all fmt struct values.
void	initialize_fmt(t_fmt *fmt)
{
	fmt->width_len = 0;
	fmt->flag_mask = 0;
	fmt->conversion = 0;
	fmt->original_len = 0;
	fmt->original = NULL;
	fmt->precision_len = 0;
	fmt->buf = NULL;
}

int	is_nonzero_digit(char c)
{
	return ((c > '0' && c <= '9'));
}

size_t	num_places(long num)
{
	size_t	result;

	result = 0;
	if (num <= 0)
	{
		result++;
		num = -num;
	}
	while (num > 0)
	{
		result++;
		num /= 10;
	}
	return (result);
}

/// Switch for printing conversions.
int	print_fmt(t_fmt *fmt, va_list *v_arg)
{
	if (fmt->flag_mask & FLAG_VALID_MASK)
		return (write(1, fmt->original, fmt->original_len));
	if (fmt->conversion == 'c')
		return (fmt_char(fmt, va_arg(*v_arg, int)));
	else if (fmt->conversion == 's' || fmt->conversion == 'c')
		return (fmt_string(fmt, va_arg(*v_arg, char *)));
	else if (fmt->conversion == 'p')
		return (fmt_ptr(fmt, va_arg(*v_arg, unsigned long)));
	else if (fmt->conversion == 'd' || fmt->conversion == 'i')
		return (fmt_integer(fmt, va_arg(*v_arg, int)));
	else if (fmt->conversion == 'u')
		return (fmt_integer(fmt, va_arg(*v_arg, unsigned int)));
	else if (fmt->conversion == 'x' || fmt->conversion == 'X')
		return (fmt_hex(fmt, va_arg(*v_arg, unsigned int)));
	else
		return (write(1, "%", 1));
}
