/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fmtint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 21:06:45 by ialee             #+#    #+#             */
/*   Updated: 2024/12/02 21:42:42 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdio.h>

static int	fmti_places(t_fmt *fmt, long num, size_t *arg_len, char **int_str)
{
	char	*result;

	result = NULL;
	*arg_len = num_places(num);
	if (fmt->flag_mask == 0 && fmt->width_len == 0)
	{
		result = ft_ltoa(num);
		write(1, result, *arg_len);
		free(result);
		return (0);
	}
  if (!(fmt->flag_mask & FLAG_DASH_MASK) && num == 0 && fmt->flag_mask & FLAG_DOT_MASK && fmt->precision_len == 0)
    *arg_len = 0;
	if (num < 0)
	{
		fmt->flag_mask |= FLAG_NEGATIVE_INTEGER_MASK;
    fmt->precision_len++;
		num = -num;
	}
	if (!!int_str)
		*int_str = ft_ltoa(num);
	return (1);
}

static size_t	fmtint_establish_sign(t_fmt *fmt, size_t *buffer_offset)
{
	size_t	count;

	count = 1;
	if (fmt->flag_mask & FLAG_PLUS_MASK)
  {
    if (!(fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK))
			fmt->buf[(*buffer_offset)++] = '+';
  }
	else if (fmt->flag_mask & FLAG_SPACE_MASK)
  {
		if (!(fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK))
			fmt->buf[(*buffer_offset)++] = ' ';
  }
  else if ((fmt->flag_mask & FLAG_ZERO_MASK))
  {
    if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK && !(fmt->flag_mask & FLAG_DOT_MASK))
    {
      fmt->buf[(*buffer_offset)++] = '-';
      fmt->precision_len -= 1;
    }
  }
	return (count);
}

static void	fmti_ec(t_fmt *fmt, long num, size_t sz, size_t *idx)
{
	size_t		arg_len;
	long		precision_len;
	char		*int_str;
  int is_monke;
  // int first_index;

	int_str = "";
	precision_len = fmt->precision_len;
  is_monke = 0;
	fmti_places(fmt, num, &arg_len, &int_str);
	if (fmt->precision_len > 0 && fmt->flag_mask & FLAG_DOT_MASK)
	{
		if (fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK && !ft_strchr(fmt->buf, '-'))
    {
		  *idx = ft_strlcat(fmt->buf, "-", sz);
			// arg_len--;
    }
    // first_index = *idx;
		while ((long)(precision_len-- - arg_len) > 0)
		{
			fmt->buf[(*idx)++] = '0';
			fmt->width_len--;
		}
	}
  // if (!(fmt->flag_mask & FLAG_ZERO_MASK) && (fmt->flag_mask & FLAG_PLUS_MASK || fmt->flag_mask & FLAG_SPACE_MASK || fmt->flag_mask & FLAG_DOT_MASK) && fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK)
  // {
  //   fmt->buf[first_index] = '-';
  //   is_monke = 1;
  // }
	if (fmt->flag_mask & FLAG_DOT_MASK
		&& fmt->precision_len == 0 && num == 0)
		fmt->width_len++;
	else
  {
    if (!(fmt->flag_mask & FLAG_ZERO_MASK) && fmt->flag_mask & FLAG_NEGATIVE_INTEGER_MASK && is_monke < 1 && !ft_strchr(fmt->buf, '-'))
			ft_strlcat(fmt->buf, "-", sz);
		*idx = ft_strlcat(fmt->buf, int_str, sz);
  }
	fmt->width_len -= arg_len;
	free(int_str);
}

static int	fmti_prints(t_fmt *fmt, long num, size_t arg_len, size_t size)
{
	size_t	index;
  long width_len;

	index = 0;
  width_len = fmt->width_len;
	fmtint_establish_sign(fmt, &index);
	if (fmt->precision_len > arg_len)
		arg_len = fmt->precision_len;
	if (fmt->flag_mask & FLAG_DASH_MASK)
	{
		fmti_ec(fmt, num, size, &index);
		if ((long)fmt->width_len < 0)
			fmt->width_len = 0;
		while ((long)fmt->width_len-- > 0)
			fmt->buf[index++] = ' ';
		return (write(1, fmt->buf, index));
	}
	else if ((fmt->flag_mask & FLAG_ZERO_DOT_MASK) == FLAG_ZERO_DOT_MASK)
  {
		while (fmt->width_len > 0 && (long)(width_len-- - arg_len) > 0)
			fmt->buf[index++] = ' ';
  }
	else if (fmt->flag_mask & FLAG_ZERO_MASK)
		while (fmt->width_len > 0 && (long)(width_len-- - arg_len) > 0)
			fmt->buf[index++] = '0';
  else if (fmt->width_len > 0)
		while (fmt->width_len > 0 && (long)(width_len-- - arg_len) > 0)
			fmt->buf[index++] = ' ';
	fmti_ec(fmt, num, size, &index);
	return (write(1, fmt->buf, index));
}

int	fmt_integer(t_fmt *fmt, long integer)
{
	char		buffer[ARG_BUFFER_SIZE];
	size_t		arg_len;

	arg_len = 0;
	if (!fmti_places(fmt, integer, &arg_len, NULL))
		return (arg_len);
	if (fmt->width_len >= ARG_BUFFER_SIZE)
	{
		fmt->buf = ft_calloc(fmt->width_len + 1, sizeof(char));
		if (!fmt->buf)
			return (0);
		arg_len = fmti_prints(fmt, integer, arg_len, fmt->width_len + 1);
		free(fmt->buf);
	}
	else
	{
		ft_bzero(buffer, ARG_BUFFER_SIZE);
		fmt->buf = buffer;
		arg_len = fmti_prints(fmt, integer, arg_len, ARG_BUFFER_SIZE);
	}
	return (arg_len);
}
