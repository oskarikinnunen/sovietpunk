/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:18:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/25 20:41:24 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int	sp_fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags);
	if (fd == -1)
		error_exit("fileopen error");
	return (fd);
}