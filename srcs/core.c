/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hublanc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/26 20:57:33 by hublanc           #+#    #+#             */
/*   Updated: 2018/11/26 21:22:39 by hublanc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm.h"

static int	put_err_msg(char *filename, char *msg)
{
	ft_putstr_fd("ft_nm: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (EXIT_FAILURE);
}

void		read_binary(t_info info, int arg)
{
	uint32_t	magic;
	char		*str;

	magic = *(int*)(info.ptr);
	str = (char*)(info.ptr);
	info.magic = magic;
	if (magic == MH_MAGIC || magic == MH_CIGAM
		|| magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		handle_macho(info, arg);
	}
	else if (magic == FAT_MAGIC || magic == FAT_CIGAM)
	{
		handle_fat(info, arg);
	}
	else if (!ft_strncmp(str, ARMAG, SARMAG))
	{
		handle_archive(info, arg);
	}
	else
	{
		put_err_msg(info.filename,
				"The file was not recognized as an object file");
	}
}

int			core(char *filename, int arg)
{
	int			fd;
	struct stat info;
	char		*ptr;
	t_info		map_info;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (put_err_msg(filename, "Can't open file"));
	if (fstat(fd, &info) == -1)
	{
		close(fd);
		return (put_err_msg(filename, "Can't stat file"));
	}
	ptr = (char*)mmap(0, info.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (ptr == MAP_FAILED)
	{
		close(fd);
		return (put_err_msg(filename, "No memory left"));
	}
	map_info.ptr = ptr;
	map_info.size = info.st_size;
	map_info.filename = filename;
	read_binary(map_info, arg);
	munmap(ptr, info.st_size);
	close(fd);
	return (EXIT_SUCCESS);
}
