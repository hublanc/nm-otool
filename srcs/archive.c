/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:12:34 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/22 18:15:53 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void	display_name_arch(char *filename, char *object)
{
	ft_putstr("\n");
	ft_putstr(filename);
	ft_putstr("(");
	ft_putstr(object);
	ft_putstr("):\n");
}

void		handle_archive(char *ptr, off_t size, char *filename)
{
	struct ar_hdr	*ar;
	int32_t			size_ar;
	int32_t			size_name;

	ar = (struct ar_hdr*)((void*)ptr + SARMAG);
	if ((void*)ar < (void*)(ptr + size))
	{
		size_ar = ft_atoi(ar->ar_size);
		ar = (struct ar_hdr*)((char*)ar + ((sizeof(struct ar_hdr) + size_ar)));
	}
	while ((void*)ar < (void*)(ptr + size))
	{
		if (!ft_strncmp(ar->ar_fmag, ARFMAG, FT_SARFMAG))
		{
			size_name = ft_atoi(ar->ar_name + ft_strlen(AR_EFMT1));
			size_ar = ft_atoi(ar->ar_size);
			display_name_arch(filename, ar->ar_name + sizeof(struct ar_hdr));
			read_binary((char*)ar + sizeof(struct ar_hdr) + size_name, size,
						filename);
		}
		else
			break ;
		ar = (struct ar_hdr*)((char*)ar + ((sizeof(struct ar_hdr) + size_ar)));
	}
}
