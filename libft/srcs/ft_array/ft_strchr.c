/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 17:50:26 by yilin             #+#    #+#             */
/*   Updated: 2024/05/26 18:00:15 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	while (*str)
	{
		if (*str == (unsigned char)c)
			return ((char *)str);
		str++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)str);
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>
int	main()
{
	char *str = "helloooo!";
	char	*result;

	result = ft_strchr(((void*)0), '\0');
	char *test = strchr(((void*)0), '\0');
	printf("%s\n", result);
	printf("%s\n", test);
}
*/