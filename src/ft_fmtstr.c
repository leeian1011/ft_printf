/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmtstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:11:23 by ialee             #+#    #+#             */
/*   Updated: 2024/12/02 21:39:36 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h> 

static int	fmtc_print_specifier(t_fmt *fmt, char c)
{
	int	index;

	index = 0;
	if (fmt->flag_mask & FLAG_DASH_MASK)
	{
		fmt->buf[index++] = c;
		while (fmt->width_len-- - 1 > 0)
			fmt->buf[index++] = ' ';
		return (write(1, fmt->buf, index));
	}
	else if (fmt->flag_mask & FLAG_ZERO_MASK || fmt->width_len > 0)
	{
		while (fmt->width_len-- - 1 > 0)
			fmt->buf[index++] = ' ';
		fmt->buf[index++] = c;
		index += ft_strlcpy(&fmt->buf[index], &c, 2);
		return (write(1, fmt->buf, index));
	}
	else
		return (write(1, &c, 1));
}

static int	fmtstr_prints(t_fmt *fmt, char *arg, long len, size_t sz)
{
	int	index;

	index = 0;
	if (fmt->flag_mask & FLAG_DASH_MASK)
	{
		index = ft_strlcpy(fmt->buf, arg, sz);
		while ((long)(fmt->width_len-- - len) > 0)
			fmt->buf[index++] = ' ';
		return (write(1, fmt->buf, index));
	}
	else if (fmt->flag_mask & FLAG_ZERO_MASK || fmt->width_len > 0)
	{
		while (fmt->width_len-- - len > 0)
			fmt->buf[index++] = ' ';
		index = ft_strlcat(fmt->buf, arg, sz);
		return (write(1, fmt->buf, index));
	}
	else
		return (write(1, arg, len));
}

static int	fmtstr_establish_str(t_fmt *fmt, char **str, long *full_len)
{
	size_t		result;
	static char	*null_str;
	static char	*empty_str;

	if (!null_str)
		null_str = "(null)";
	if (!empty_str)
		empty_str = "";
	result = 0;
	if (!*str)
	{
		if (fmt->flag_mask == 0
			|| (fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len >= 6))
			*str = null_str;
		else
			*str = empty_str;
	}
	result = ft_strlen(*str);
	if (fmt->flag_mask & FLAG_DOT_MASK && result > fmt->precision_len)
		result = fmt->precision_len;
	*full_len = result;
	if (fmt->width_len > result)
		*full_len = fmt->width_len;
	return (result);
}

int	fmt_string(t_fmt *fmt, char *str)
{
	char	buffer[ARG_BUFFER_SIZE];
	long	arg_len;
	long	full_len;

	arg_len = fmtstr_establish_str(fmt, &str, &full_len);
	if (fmt->flag_mask & FLAG_VALID_MASK)
		return (write(1, fmt->original, fmt->original_len));
	if (fmt->flag_mask == 0 && fmt->width_len == 0)
		return (write(1, str, arg_len));
	if (full_len >= ARG_BUFFER_SIZE)
	{
		fmt->buf = ft_calloc(full_len + 1, sizeof(char));
		if (!fmt->buf)
			return (0);
		full_len = fmtstr_prints(fmt, str, arg_len, full_len + 1);
		free(fmt->buf);
	}
	else
	{
		ft_bzero(buffer, ARG_BUFFER_SIZE);
		fmt->buf = buffer;
		full_len = fmtstr_prints(fmt, str, arg_len, ARG_BUFFER_SIZE);
	}
	return (full_len);
}

int	fmt_char(t_fmt *fmt, char c)
{
	char	buffer[ARG_BUFFER_SIZE];
	size_t	full_len;

	full_len = 1;
	if (fmt->width_len > full_len)
		full_len = fmt->width_len;
	if (fmt->flag_mask & FLAG_VALID_MASK)
		return (write(1, fmt->original, fmt->original_len));
	if (fmt->flag_mask == 0 && fmt->width_len == 0)
		return (write(1, &c, 1));
	if (full_len >= ARG_BUFFER_SIZE)
	{
		fmt->buf = ft_calloc(full_len + 1, sizeof(char));
		if (!fmt->buf)
			return (0);
		full_len = fmtc_print_specifier(fmt, c);
		free(fmt->buf);
	}
	else
	{
		ft_bzero(buffer, ARG_BUFFER_SIZE);
		fmt->buf = buffer;
		full_len = fmtc_print_specifier(fmt, c);
	}
	return (full_len);
}
