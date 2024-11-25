/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:06:53 by yilin             #+#    #+#             */
/*   Updated: 2024/05/27 14:05:36 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}
/*
void ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

int	main(void)
{
    // Test writing to standard output (stdout)
    ft_putstr_fd("AAA\n", 1);

    // Test writing to standard error (stderr)
    ft_putstr_fd("ERROR\n", 2);

    return (0);
}
*/