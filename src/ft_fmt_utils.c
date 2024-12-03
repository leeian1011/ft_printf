/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 22:22:48 by ialee             #+#    #+#             */
/*   Updated: 2024/12/03 23:10:15 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/// fmthex utility function that handles scenarios for the '-', '0' and '.'
/// flags.
void	fmthex_flag_util(t_fmt *fmt, size_t *index, char *num, size_t hex_len)
{
	if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK)
	{
		if ((fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len == 0)
			&& ft_strncmp(num, "0", hex_len) == 0)
			fmt->width_len++;
		while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
			fmt->buf[(*index)++] = ' ';
	}
	else if (fmt->flag_mask & FLAG_ZERO_MASK)
	{
		while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
			fmt->buf[(*index)++] = '0';
	}
	else if (fmt->width_len > 0)
		while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
			fmt->buf[(*index)++] = ' ';
}

/// fmtptr precision util
/// fmtptr utility function that handles scenarios for the '.'
/// flags.
void	fmtptr_putil(t_fmt *fmt, long precision_len, long hex_len, size_t *idx)
{
	if (fmt->precision_len > 0)
	{
		while (precision_len-- - hex_len > 0)
		{
			fmt->buf[(*idx)++] = '0';
			fmt->width_len--;
		}
	}
}

/// fmtstr_zero_util ('0' flag)
/// fmtptr utility function that handles scenarios for the '0'
/// flags.
int	fmtstr_zutil(t_fmt *fmt, size_t index, long len, char *arg)
{
	while ((long)(fmt->width_len-- - len) > 0)
		fmt->buf[index++] = ' ';
	if ((fmt->flag_mask & FLAG_DOT_MASK && (long)fmt->precision_len < len)
		|| len == 0)
	{
		while ((long)fmt->precision_len-- > 0)
			fmt->buf[index++] = *arg++;
	}
	else
	{
		while (len-- > 0)
			fmt->buf[index++] = *arg++;
	}
	return (write(1, fmt->buf, index));
}


/// fmtint_width_util
/// fmtptr utility function that handles scenarios for varying width with flags
/// '0' and '.'.
size_t	fmtint_width_util(t_fmt *fmt, size_t sz, long num, char *int_str)
{
	if (fmt->flag_mask & FLAG_DOT_MASK
		&& fmt->precision_len == 0 && num == 0)
	{
		fmt->width_len++;
		return (0);
	}
	else
	{
		if (!(fmt->flag_mask & FLAG_ZERO_MASK)
			&& fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK
			&& !ft_strchr(fmt->buf, '-'))
			ft_strlcat(fmt->buf, "-", sz);
		return (ft_strlcat(fmt->buf, int_str, sz));
	}
}

/// fmtint_width_util
/// fmtptr utility function that handles scenarios for the '0' and '.' flags.
void	fmtint_flag_util(t_fmt *fmt, size_t *idx, long width_len, long arg_len)
{
	if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK)
		while (fmt->width_len > 0 && (long)(width_len-- - arg_len) > 0)
			fmt->buf[(*idx)++] = ' ';
	else if (fmt->flag_mask & FLAG_ZERO_MASK)
		while (fmt->width_len > 0 && (long)(width_len-- - arg_len) > 0)
			fmt->buf[(*idx)++] = '0';
	else if (fmt->width_len > 0)
		while (fmt->width_len > 0 && (long)(width_len-- - arg_len) > 0)
			fmt->buf[(*idx)++] = ' ';
}
