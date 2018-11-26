/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archive.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:12:34 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 21:36:02 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static void	put_name_field(char *str)
{
	int	i;

	i = 0;
	while (i < 16 && str[i] != ' ')
		i++;
	write(1, str, i);
}

static void	put_name_arch(char *filename, char *object, int arg)
{
	if (arg == O_ARG || arg == M_ARG)
		ft_putstr("\n");
	ft_putstr(filename);
	ft_putstr("(");
	if (!ft_strncmp(object, AR_EFMT1, FT_SEFMT1))
		ft_putstr(object + sizeof(struct ar_hdr));
	else
		put_name_field(object);
	ft_putstr("):\n");
}

static void	print_header_otool(t_info info, int arg)
{
	if (arg == OTOOL_FILE)
	{
		ft_putstr("Archive : ");
		ft_putendl(info.filename);
	}
}

void		handle_archive(t_info info, int arg)
{
	struct ar_hdr	*ar;
	int32_t			size_ar;
	int32_t			size_name;

	print_header_otool(info, arg);
	ar = (struct ar_hdr*)((void*)(info.ptr) + SARMAG);
	if (cp(info, ar))
	{
		size_ar = ft_atoi(ar->ar_size);
		ar = (struct ar_hdr*)((char*)ar + ((sizeof(struct ar_hdr) + size_ar)));
	}
	while (cp(info, ar))
	{
		if (!ft_strncmp(ar->ar_fmag, ARFMAG, FT_SARFMAG))
		{
			size_name = ft_atoi(ar->ar_name + ft_strlen(AR_EFMT1));
			size_ar = ft_atoi(ar->ar_size);
			put_name_arch(info.filename, ar->ar_name, arg);
			binary_tmp(info, (char*)ar + sizeof(struct ar_hdr) + size_name,
						arg);
		}
		else
			break ;
		ar = (struct ar_hdr*)((char*)ar + ((sizeof(struct ar_hdr) + size_ar)));
	}
}
