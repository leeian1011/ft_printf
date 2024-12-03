/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:23:18 by ialee             #+#    #+#             */
/*   Updated: 2024/12/02 21:24:40 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/// Setup logic
static void	update_flag(t_fmt *fmt, unsigned char flag_mask, const char **str)
{
	if (fmt->width_len != 0 || fmt->flag_mask & FLAG_DOT_MASK)
		fmt->flag_mask |= FLAG_VALID_MASK;
	fmt->flag_mask |= flag_mask;
	(*str)++;
}

/// Width
static void	handle_width(t_fmt *fmt, const char **str)
{
	fmt->width_len = ft_atoi(*str);
	while (ft_isdigit(**str))
		(*str)++;
}

/// Precision flag (.)
static void	handle_precision(t_fmt *fmt, const char **str)
{
	if (fmt->flag_mask & FLAG_DOT_MASK)
		fmt->flag_mask |= FLAG_VALID_MASK;
	fmt->flag_mask |= FLAG_DOT_MASK;
	fmt->precision_len = ft_atoi(++*str);
	while (ft_isdigit(**str))
		(*str)++;
}

/// Flag logic.
/// This is essentially a switch that performs 
/// setup logic when a flag is identified.
static void	handle_flag(t_fmt *fmt, const char **str)
{
	char	*flag;

	while (**str)
	{
		flag = ft_strchr(FLAGS, **str);
		if (!flag && is_nonzero_digit(**str))
		{
			handle_width(fmt, str);
			continue ;
		}
		else if (!flag)
			break ;
		if (*flag == '+')
			update_flag(fmt, FLAG_PLUS_MASK, str);
		else if (*flag == '#')
			update_flag(fmt, FLAG_HASH_MASK, str);
		else if (*flag == ' ')
			update_flag(fmt, FLAG_SPACE_MASK, str);
		else if (*flag == '-')
			update_flag(fmt, FLAG_DASH_MASK, str);
		else if (*flag == '0')
			update_flag(fmt, FLAG_ZERO_MASK, str);
		else if (*flag == '.')
			handle_precision(fmt, str);
	}
}

/// Handle Flags for ft_printf.
/// This initiates the `fmt` struct and assigns the
/// appropriae flags and conversion bitmasks/
int	handle_specifiers(const char **str, va_list *v_arg)
{
	t_fmt	fmt;
	char	*conversion;

	initialize_fmt(&fmt);
	fmt.original = (*str)++;
	handle_flag(&fmt, str);
	fmt.original_len += *str - fmt.original;
	conversion = ft_strchr(CONVERSIONS, **str);
	if (!conversion)
		fmt.flag_mask |= FLAG_VALID_MASK;
	else
		fmt.conversion = *conversion;
	fmt.original_len += 1;
	(*str)++;
	return (print_fmt(&fmt, v_arg));
}
