/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:18:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 14:28:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int	sp_fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags);
	if (fd < 0)
		error_exit("fileopen error");
	return (fd);
}

void	openmap(t_gamecontext *gc, uint32_t *to)
{
	int	fd;

	if (ft_strstr(gc->mapname, ".map") == NULL)
		error_exit("invalid mapname, must end with '.map'");
	fd = sp_fileopen(gc->mapname, O_RDONLY);
	(void)!read(fd, to, MAPSIZE * MAPSIZE * sizeof(u_int32_t));
	close(fd);
}
