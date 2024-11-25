/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:35:13 by yilin             #+#    #+#             */
/*   Updated: 2024/06/03 18:52:59 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
// #include <unistd.h>

int	ft_put_str(char *str)
{
	int	len;

	len = 0;
	if (!str)
		return (ft_put_str("(null)"));
	while (*str)
	{
		len += write(1, str, 1);
		str++;
	}
	return (len);
}
