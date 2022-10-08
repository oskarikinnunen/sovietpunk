/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   samplemap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 12:57:35 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/08 08:28:29 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

bool	is_in_map(int crd[2])
{
	return (crd[X] < MAPSIZE - 1 && crd[Y] < MAPSIZE - 1 && crd[X] > 0 && crd[Y] > 0);
}

uint32_t	samplemap(uint32_t *map, int crd[2])
{
	if (crd[X] > MAPSIZE - 1 || crd[Y] > MAPSIZE - 1|| crd[X] < 0 || crd[Y] < 0)
		return (0);
	return (map[crd[X] + (crd[Y] * MAPSIZE)]);
}