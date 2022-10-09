/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 20:43:19 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 15:25:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	spawnplayer(t_gamecontext *gc)
{
	int			ix;
	int			iy;
	uint32_t	smpl;

	ix = 0;
	iy = 0;
	while (iy < MAPSIZE)
	{
		while (ix < MAPSIZE)
		{
			smpl = samplemap(gc->map, (int[2]) {ix, iy});
			if (samplemap(gc->map, (int [2]){ix, iy}) == SPAWN)
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
