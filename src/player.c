/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 20:43:19 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/08 09:40:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	spawnplayer(t_gamecontext *gc)
{
	int ix;
	int	iy;

	ix = 0;
	iy = 0;
	while (iy < MAPSIZE)
	{
		while (ix < MAPSIZE)
		{
			if (samplemap(gc->map, (int [2]){ix, iy}) == TILE_SPAWN)
			{
				gc->player.pos[X] = (ix * GAMESCALE) + GAMESCALE / 2;
				gc->player.pos[Y] = (iy * GAMESCALE) + GAMESCALE / 2;
				gc->map[ix + (iy) * MAPSIZE] = 0;
			}
			ix++;
		}
		ix = 0;
		iy++;
	}
	gc->player.angle = 0;
}
