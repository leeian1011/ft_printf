/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 20:12:47 by ialee             #+#    #+#             */
/*   Updated: 2024/12/03 20:44:42 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <sys/types.h>
#include <stdio.h>

static char	*fmthex_itoh(unsigned int hex)
{
	char			*ahex;
	int				index;
	unsigned int	borrow_hex;

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

static int	fmth_init(t_fmt *fmt, char **ahex, uint hex, size_t *hex_len)
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

static void	fmth_ec(t_fmt *fmt, char *hex, size_t sz, size_t *idx)
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
			fmt->buf[(*idx)++] = '0';
			fmt->width_len--;
		}
	}
	if ((fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len == 0)
		&& ft_strncmp(hex, "0", hex_len) == 0)
		fmt->width_len++;
	else
		*idx = ft_strlcat(fmt->buf, hex, sz);
	copy_hex_len = ft_strlen(fmt->buf);
	if (fmt->conversion == 'X')
		while (copy_hex_len--)
			fmt->buf[copy_hex_len] = ft_toupper(fmt->buf[copy_hex_len]);
	fmt->width_len -= hex_len;
}

static int	fmthex_prints(t_fmt *fmt, char *num, size_t hex_len, size_t size)
{
	size_t	index;

	index = 0;
	if (fmt->precision_len > hex_len)
		hex_len = fmt->precision_len;
	if (fmt->flag_mask & FLAG_HASH_MASK && ft_strncmp(num, "0", hex_len) != 0)
		index = ft_strlcat(fmt->buf, HEX_PREFIX, size);
	if (fmt->flag_mask & FLAG_DASH_MASK)
	{
		fmth_ec(fmt, num, size, &index);
		if ((long)fmt->width_len < 0)
			fmt->width_len = 0;
		while (fmt->width_len-- > 0)
			fmt->buf[index++] = ' ';
		return (write(1, fmt->buf, index));
	}
	else if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK)
	{
		if ((fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len == 0) && ft_strncmp(num, "0", hex_len) == 0)
			fmt->width_len++;
		while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
			fmt->buf[index++] = ' ';
	}
	else if (fmt->flag_mask & FLAG_ZERO_MASK)
	{
		while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
			fmt->buf[index++] = '0';
	}
	else if (fmt->width_len > 0)
		while (fmt->width_len > 0 && (long)(fmt->width_len-- - hex_len) > 0)
			fmt->buf[index++] = ' ';
	fmth_ec(fmt, num, size, &index);
	return (write(1, fmt->buf, index));
}

int	fmt_hex(t_fmt *fmt, unsigned int hex)
{
	char		buffer[ARG_BUFFER_SIZE];
	char		*ahex;
	size_t		hex_len;

	if (fmth_init(fmt, &ahex, hex, &hex_len))
		return (hex_len);
	if (fmt->width_len >= ARG_BUFFER_SIZE)
	{
		fmt->buf = ft_calloc(fmt->width_len + 1, sizeof(char));
		if (!fmt->buf)
			return (0);
		hex_len = fmthex_prints(fmt, ahex, hex_len, fmt->width_len + 1);
		free(fmt->buf);
	}
	else
	{
		ft_bzero(buffer, ARG_BUFFER_SIZE);
		fmt->buf = buffer;
		hex_len = fmthex_prints(fmt, ahex, hex_len, ARG_BUFFER_SIZE);
	}
	free(ahex);
	return (hex_len);
}
