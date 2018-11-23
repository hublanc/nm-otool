/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 12:44:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/23 18:42:26 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>

void	read_binary(t_info info, int arg)
{
	uint32_t	magic;
	char		*str;
	
	magic = *(int*)(info.ptr);
	str = (char*)(info.ptr);
	if (magic == MH_MAGIC || magic == MH_CIGAM
		|| magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		handle_macho(info, arg);
	}
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
	{
		handle_fat(info);
	}
	else if (!ft_strncmp(str, ARMAG, SARMAG))
	{
		handle_archive(info);
	}
}

int		ft_nm(char *filename, int arg)
{
	int			fd;
	struct stat info;
	char		*ptr;
	t_info		map_info;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return(EXIT_FAILURE);
	if (fstat(fd, &info) == -1)
	{
		close(fd);
		return(EXIT_FAILURE);
	}
	ptr = (char*)mmap(0, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
	{
		close(fd);
		return(EXIT_FAILURE);
	}
	map_info.ptr = ptr;
	map_info.size = info.st_size;
	map_info.filename = filename;
	read_binary(map_info, arg);
	munmap(ptr, info.st_size);
	close(fd);
	return (EXIT_SUCCESS);
}

int main(int argc, char *argv[])
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
			ret = ft_nm(argv[i], arg);
			i++;
		}
	}
	return (ret);
}
