/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 11:14:35 by yilin             #+#    #+#             */
/*   Updated: 2024/05/26 18:06:40 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

char	*ft_strnstr(const char *long_str, const char *short_str, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (short_str[j] == '\0')
		return ((char *)long_str);
	while (long_str[i])
	{
		j = 0;
		while (long_str[i + j] == short_str[j] && (i + j) < len)
		{
			if (short_str[j + 1] == '\0')
				return ((char *)&long_str[i]);
			j++;
		}
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <bsd/string.h>
int	main(void)
{
	char	lng_s[] = "hello,today!Saturday!";
	char	shrt_s[] = "Sa";
	char	lng_s_r[] = "hello,today!Saturday!";
	char	shrt_s_r[] = "Sa";
	char	*result;
	char	*result_real;

	result = ft_strnstr(lng_s, shrt_s, 14);
	printf("%s\n", result);	
	result_real = strnstr(lng_s_r, shrt_s_r, 14);
	printf("%s\n", result_real);	
}
*/
