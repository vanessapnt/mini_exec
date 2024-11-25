/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:23:09 by yilin             #+#    #+#             */
/*   Updated: 2024/05/22 17:09:27 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
#include <stdlib.h>
int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
char test_toupper(unsigned int i, char c)
{
    // Convert to uppercase only if the character is at an even index
    if (i % 2 == 0 && c >= 'a' && c <= 'z')
    {
        return c - 32; // Convert lowercase to uppercase
    }
    return c; // Return the character unchanged otherwise
}
*/
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*new_str;

	i = 0;
	new_str = (char *)malloc((ft_strlen(s) + 1) * sizeof(*s));
	if (!new_str)
		return (NULL);
	while (s[i])
	{
		new_str[i] = f(i, s[i]);
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
/*
#include <stdio.h>
int main()
{
	char const *str = "hello_world";
	char *result;

	result = ft_strmapi(str, test_toupper);
	printf("%s\n", result);
	free(result);
	return (0);
}
*/