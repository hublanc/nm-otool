/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:10:30 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/22 22:29:31 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

void	handle_fat32(char *ptr, off_t size, char *filename, uint32_t nfat_arch)
{
	uint32_t		i;
	int				arch;
	struct fat_arch *fa;

	i = 0;
	arch = NO_ARCH;
	fa = (struct fat_arch*)(ptr + sizeof(struct fat_header));
	while ((void*)fa < (void*)(ptr + size) && i < OSSwapInt32(nfat_arch))
	{
		if (OSSwapInt32(fa->cputype) == CPU_TYPE_X86_64)
		{
			arch = ARCH_FOUND;
			read_binary(ptr + OSSwapInt32(fa->offset), size, filename);
		}
		else if (arch == ALL_ARCH)
			read_binary(ptr + OSSwapInt32(fa->offset), size, filename);
		fa = (struct fat_arch*)((char*)fa + sizeof(struct fat_arch));
		i++;
		if (i == OSSwapInt32(nfat_arch) && arch == NO_ARCH)
		{
			i = 0;
			arch = ALL_ARCH;
			fa = (struct fat_arch*)(ptr + sizeof(struct fat_header));
		}
	}
}

void	handle_fat(char *ptr, off_t size, char *filename)
{
	struct fat_header	*fh;

	fh = (struct fat_header*)ptr;
	if (fh->magic == FAT_CIGAM)
	{
		handle_fat32(ptr, size, filename, fh->nfat_arch);
	}
}
