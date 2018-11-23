/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 18:10:30 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/23 18:20:38 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

void	print_cpu_type(cpu_type_t type)
{
	static const cpu_type_t cpu_types[14] = {CPU_TYPE_VAX, CPU_TYPE_MC680x0,
		CPU_TYPE_I386, CPU_TYPE_X86_64, CPU_TYPE_MC98000, CPU_TYPE_HPPA,
		CPU_TYPE_ARM, CPU_TYPE_ARM64, CPU_TYPE_MC88000, CPU_TYPE_SPARC,
		CPU_TYPE_I860, CPU_TYPE_POWERPC, CPU_TYPE_POWERPC64, 0};
	static const char *cpu_str[14] = {"vax", "mc680x0", "i386", "x86_64",
		"mc98000", "hppa", "arm", "arm64", "mc88000", "sparc", "i860",
		"ppc", "ppc64", 0};
	int	i;

	i = 0;
	while (i < 14)
	{
		if (type == cpu_types[i])
		{
			ft_putstr(cpu_str[i]);
			break;
		}
		i++;
	}
}

void	print_name_fat32(char *filename, cpu_type_t type, int arch)
{
	if (arch == ALL_ARCH)
	{
		ft_putstr("\n");
		ft_putstr(filename);
		ft_putstr(" (for architecture ");
		print_cpu_type(type);
		ft_putstr("):\n");
	}
}

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
		if (OSSwapInt32(fa->cputype) == CPU_TYPE_X86_64 || arch == ALL_ARCH)
		{
			arch = arch == NO_ARCH ? ARCH_FOUND : arch;
			print_name_fat32(filename, OSSwapInt32(fa->cputype), arch);
			read_binary(ptr + OSSwapInt32(fa->offset), size, filename, O_ARG);
		}
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
