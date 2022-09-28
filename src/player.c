/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/28 20:43:19 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/28 20:43:34 by okinnune         ###   ########.fr       */
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
			if (gc->map[ix + (iy * MAPSIZE)] == 2)
			{
				gc->player.pos[X] = (ix * GAMESCALE) - 0.5f;
				gc->player.pos[Y] = (iy * GAMESCALE) - 0.5f;
			}
			ix++;
		}
		ix = 0;
		iy++;
	}
	gc->player.angle = 0;
}
