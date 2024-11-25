/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 18:00:15 by yilin             #+#    #+#             */
/*   Updated: 2024/05/24 18:04:47 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*#include <unistd.h>*/

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const char	*s_ptr;
	size_t		i;
	char		*d_ptr;

	i = 0;
	d_ptr = (char *)dest;
	s_ptr = (const char *)src;
	if (!dest && !src)
		return (dest);
	while (i < n)
	{
		d_ptr[i] = s_ptr[i];
		i++;
	}
	return (dest);
}
/*
#include <stdio.h>
#include <string.h>
int main()
{
	char	src[8] = "hey!you";
	char	dest[50];
	char	dest_r[50];

	ft_memcpy(dest, src, 8);
	printf("%s\n", dest);
	memcpy(dest_r, src, 8);
	printf("%s\n", dest);
}
*/