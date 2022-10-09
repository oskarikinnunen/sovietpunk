/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:18:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 01:13:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int	sp_fileopen(char *filename, int flags)
{
	int	fd;

	fd = open(filename, flags);
	if (fd < 0)
		exit(0);
	return (fd);
}