/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:25:11 by ialee             #+#    #+#             */
/*   Updated: 2024/12/02 21:35:22 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/// printf macro clone.
/// Only supports the conversions: cspdiuxX%
/// Supports the flags '0-.+ #'
int	ft_printf(const char *str, ...)
{
	int			count;
	size_t		printable;
	va_list		v_arg;
	const char	*iterator;

	count = 0;
	iterator = str;
	va_start(v_arg, str);
	while (*str)
	{
		while (*iterator != '%' && *iterator)
			iterator++;
		printable = iterator - str;
		if (printable)
			count += write(1, str, printable);
		if (!*iterator)
			break ;
		count += handle_specifiers(&iterator, &v_arg);
		str = iterator;
	}
	va_end(v_arg);
	return (count);
}
