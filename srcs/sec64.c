/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sec64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 13:46:52 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 16:24:20 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

t_sec64_list	*new_sec64(struct section_64 *section)
{
	t_sec64_list	*new;

	if (!(new = (t_sec64_list*)ft_memalloc(sizeof(struct s_sec64_list))))
		return (NULL);
	new->section = section;
	return (new);
}

void			add_sec64(t_sec64_list **list, t_sec64_list *new)
{
	if (*list)
	{
		while (*list)
			list = &(*list)->next;
		*list = new;
	}
	else
		*list = new;
}

void			sec64_delone(t_sec64_list **list)
{
	if (!list)
		return ;
	free(*list);
	*list = NULL;
}

void			sec64_del(t_sec64_list **list)
{
	if (*list)
	{
		sec64_del(&(*list)->next);
		sec64_delone(&(*list));
	}
}
