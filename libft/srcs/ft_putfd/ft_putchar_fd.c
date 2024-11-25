/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:57:15 by yilin             #+#    #+#             */
/*   Updated: 2024/05/22 14:44:07 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/*
int	main(void)
{
    // Test writing to standard output (stdout)
    ft_putchar_fd('A', 1);
    ft_putchar_fd('\n', 1);

    // Test writing to standard error (stderr)
    ft_putchar_fd('B', 2);
    ft_putchar_fd('\n', 2);

    return (0);
}
*/