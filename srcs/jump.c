/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 16:06:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 16:25:41 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

uint32_t	jump_header(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct mach_header);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct mach_header_64);
	return (size);
}

uint32_t	jump_segment(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct segment_command);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct segment_command_64);
	return (size);
}

uint32_t	jump_section(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct section);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct section_64);
	return (size);
}

uint32_t	jump_nlist(uint32_t magic)
{
	uint32_t	size;

	size = 0;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
		size = sizeof(struct nlist);
	else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		size = sizeof(struct nlist_64);
	return (size);
}
