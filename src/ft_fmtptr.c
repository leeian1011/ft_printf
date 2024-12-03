/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmtptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:09:28 by ialee             #+#    #+#             */
/*   Updated: 2024/12/03 22:39:37 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

/// Integer to Hex ascii.
static char	*fmthex_itoh(unsigned long hex)
{
	char			*ahex;
	int				index;
	unsigned long	borrow_hex;

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

/// Initializes and determines the ptr value to be printed.
static int	fmtptr_init(t_fmt *fmt, char **ahex, ulong hex, size_t *hex_len)
{
	*ahex = fmthex_itoh(hex);
	*hex_len = ft_strlen(*ahex);
	if (fmt->width_len == 0 && fmt->flag_mask == 0)
	{
		if (ft_strncmp(*ahex, "0", *hex_len) == 0)
			*hex_len = write(1, "(nil)", 5);
		else
		{
			*hex_len += write(1, HEX_PREFIX, 2);
			write(1, *ahex, *hex_len - 2);
		}
		free(*ahex);
		return (*hex_len);
	}
	return (0);
}

/// (fmtptr_establish_conversion)
/// Determines the width_len as well as the value representable..
static void	fmtptr_ec(t_fmt *fmt, char *hex, size_t sz, size_t *idx)
{
	long	hex_len;

	hex_len = ft_strlen(hex);
	fmtptr_putil(fmt, fmt->precision_len, hex_len, idx);
	if ((fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len == 0)
		&& ft_strncmp(hex, "0", hex_len) == 0)
		fmt->width_len++;
	else
	{
		if (ft_strncmp(hex, "0", hex_len) == 0)
			*idx = ft_strlcat(fmt->buf, "(nil)", sz);
		else
		{
			ft_strlcat(fmt->buf, HEX_PREFIX, sz);
			*idx = ft_strlcat(fmt->buf, hex, sz);
		}
	}
	fmt->width_len -= hex_len;
}

/// Prints the passed in variadic argument (void */long)
/// along with it's appropriate flags.
static int	fmtptr_prints(t_fmt *fmt, char *num, size_t ptr_len, size_t size)
{
	size_t	index;

	index = 0;
	if (ft_strncmp(num, "0", ft_strlen(num)) == 0)
		fmt->width_len -= 4;
	else
		fmt->width_len -= 2;
	if (fmt->precision_len > ptr_len)
		ptr_len = fmt->precision_len;
	if (fmt->flag_mask & FLAG_DASH_MASK)
	{
		fmtptr_ec(fmt, num, size, &index);
		while ((long)fmt->width_len-- > 0)
			fmt->buf[index++] = ' ';
	}
	else if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK
		|| fmt->width_len > 0)
	{
		while (fmt->width_len > 0 && (long)(fmt->width_len-- - ptr_len) > 0)
			fmt->buf[index++] = ' ';
		fmtptr_ec(fmt, num, size, &index);
	}
	else
		fmtptr_ec(fmt, num, size, &index);
	return (write(1, fmt->buf, index));
}

/// Entry point for void * conversion handling.
int	fmt_ptr(t_fmt *fmt, unsigned long ptr)
{
	char		buffer[ARG_BUFFER_SIZE];
	char		*aptr;
	size_t		ptr_len;

	if (fmtptr_init(fmt, &aptr, ptr, &ptr_len))
		return (ptr_len);
	if (fmt->width_len >= ARG_BUFFER_SIZE)
	{
		fmt->buf = ft_calloc(fmt->width_len + 1, sizeof(char));
		if (!fmt->buf)
			return (0);
		ptr_len = fmtptr_prints(fmt, aptr, ptr_len, fmt->width_len + 1);
		free(fmt->buf);
	}
	else
	{
		ft_bzero(buffer, ARG_BUFFER_SIZE);
		fmt->buf = buffer;
		ptr_len = fmtptr_prints(fmt, aptr, ptr_len, ARG_BUFFER_SIZE);
	}
	free(aptr);
	return (ptr_len);
}
