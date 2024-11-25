/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:43:30 by yilin             #+#    #+#             */
/*   Updated: 2024/05/24 17:39:12 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*#include <unistd.h>*/

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	char	*s1_ptr;
	char	*s2_ptr;
	size_t	i;

	s1_ptr = (char *)s1;
	s2_ptr = (char *)s2;
	i = 0;
	while (i < n)
	{
		if (s1_ptr[i] != s2_ptr[i])
			return ((unsigned char)s1_ptr[i] - (unsigned char)s2_ptr[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int main()
{
	char	s1[6] = "hello";
	char	s2[5] = "hall";
	int	result;
	int	result_r;

	result = ft_memcmp(s1, s2, 3);
	printf("%i\n", result);
	result_r = memcmp(s1, s2, 3);
	printf("%i\n", result_r);
	return (0);
}
*/