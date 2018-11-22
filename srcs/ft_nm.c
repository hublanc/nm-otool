/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 12:44:32 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/22 22:22:29 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm.h"
#include <stdio.h>

void	read_binary(char *ptr, off_t size, char *filename)
{
	uint32_t	magic_number;
	char		*str;
	
	magic_number = *(int*)ptr;
	str = (char*)ptr;
	if (magic_number == MH_MAGIC || 
		magic_number == MH_MAGIC_64)
	{
		handle_macho(ptr);
	}
	else if (magic_number == FAT_MAGIC || magic_number == FAT_CIGAM)
	{
		handle_fat(ptr, size, filename);
	}
	else if (!ft_strncmp(str, ARMAG, SARMAG))
	{
		handle_archive(ptr, size, filename);
	}
}

int		ft_nm(char *filename)
{
	int			fd;
	struct stat info;
	char		*ptr;

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
	read_binary(ptr, info.st_size, filename);
	munmap(ptr, info.st_size);
	close(fd);
	return (EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	int i;
	int	ret;

	i = 1;
	ret = 0;
	if (argc > 1)
	{
		while (i < argc)
		{
			ret = ft_nm(argv[i]);
			i++;
		}
	}
	return (ret);
}
