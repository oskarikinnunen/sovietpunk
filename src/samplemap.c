/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   samplemap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 12:57:35 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/07 13:01:18 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

uint32_t	samplemap(uint32_t *map, int crd[2])
{
	if (crd[X] + (crd[Y] * MAPSIZE) >= MAPSIZE * MAPSIZE)
		return (0);
	return (map[crd[X] + (crd[Y] * MAPSIZE)]);
}