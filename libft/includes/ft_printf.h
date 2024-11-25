/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yilin <yilin@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 18:42:18 by yilin             #+#    #+#             */
/*   Updated: 2024/06/03 19:49:40 by yilin            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

int	ft_put_char(int a);
int	ft_put_str(char *str);
int	ft_put_int(long int nb);
int	ft_put_unsign(unsigned int nb);
int	ft_put_hex(unsigned int nb, char xX);
int	ft_put_ptr(void *addr);
int	ft_printf(const char *format, ...);

#endif
