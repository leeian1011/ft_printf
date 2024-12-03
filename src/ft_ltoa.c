/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ltoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ialee <ialee@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:22:14 by ialee             #+#    #+#             */
/*   Updated: 2024/12/03 20:53:25 by ialee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft/libft.h"

/// unsigned long to ascii.
static char	*ft_ultoa(unsigned long num, size_t num_places, char *buffer)
{
	if (num == 0)
		buffer[num_places] = '0';
	while (num > 0)
	{
		buffer[num_places--] = '0' + (num % 10);
		num /= 10;
	}
	return (buffer);
}

/// Converts `long` to `char *`.
/// RETURN(s):
/// `NULL` - on failure to allocate memory. 
/// `char *` - (number translated to `char *` ) on success.
char	*ft_ltoa(long num)
{
	char			*buffer;
	unsigned long	copy_num;
	size_t			count;

	count = num <= 0;
	copy_num = num;
	if (count)
		copy_num = -copy_num;
	while (copy_num > 0)
	{
		count++;
		copy_num /= 10;
	}
	buffer = ft_calloc(count + 1, 1);
	if (!buffer)
		return (NULL);
	buffer[--count] = '\0';
	if (num < 0)
	{
		buffer[0] = '-';
		return (ft_ultoa(-num, count, buffer));
	}
	else
		return (ft_ultoa(num, count, buffer));
}
