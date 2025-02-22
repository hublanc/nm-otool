/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 12:44:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 20:59:18 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>

int			main(int argc, char *argv[])
{
	int i;
	int	ret;
	int arg;

	i = 1;
	ret = 0;
	arg = argc > 2 ? M_ARG : O_ARG;
	if (argc > 1)
	{
		while (i < argc)
		{
			ret = core(argv[i], arg);
			i++;
		}
	}
	else
		ret = core("a.out", arg);
	return (ret);
}
