/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 17:16:38 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 21:23:41 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

int			main(int argc, char *argv[])
{
	int i;
	int	ret;
	int arg;

	i = 1;
	ret = 0;
	arg = OTOOL_FILE;
	if (argc > 1)
	{
		while (i < argc)
		{
			ret = core(argv[i], arg);
			i++;
		}
	}
	return (ret);
}
