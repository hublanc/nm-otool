/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   qsort.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:17:06 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/22 18:24:07 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void		swap_symbol(t_symbol *ptr1, t_symbol *ptr2)
{
	t_symbol	tmp;

	tmp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = tmp;
}

static int32_t	inside_sort(t_symbol *symbols, int32_t x, int32_t y)
{
	t_symbol	pivot;
	int32_t		i;
	int32_t		j;
	int			ret;

	pivot = symbols[y];
	i = x - 1;
	j = x;
	ret = 0;
	while (j < y)
	{
		ret = ft_strcmp(symbols[j].name, pivot.name);
		if ((ret < 0) || (ret == 0
			&& symbols[j].info.n_value < pivot.info.n_value))
		{
			i++;
			swap_symbol(&(symbols[i]), &(symbols[j]));
		}
		j++;
	}
	swap_symbol(&(symbols[i + 1]), &(symbols[j]));
	return (i + 1);
}

void			sort_symbols(t_symbol *symbols, int32_t x, int32_t y)
{
	int32_t	p;

	p = 0;
	if (x < y)
	{
		p = inside_sort(symbols, x, y);
		sort_symbols(symbols, x, p - 1);
		sort_symbols(symbols, p + 1, y);
	}
}
