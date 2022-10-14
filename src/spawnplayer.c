/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawnplayer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 20:43:19 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 11:54:42 by okinnune         ###   ########.fr       */
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
	f2cpy(gc->player.pos, (float [2]){127.0f, 127.0f});
	while (iy < MAPSIZE)
	{
		while (ix < MAPSIZE)
		{
			smpl = samplemap(gc->map, (int [2]){ix, iy});
			if (smpl == SPAWN)
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
}
