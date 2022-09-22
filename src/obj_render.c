/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 17:35:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/22 19:54:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int		screenspace_y(t_gamecontext *gc, int dist)
{
	int	iy;

	/*iy = (WINDOW_H / 2) - 2;
	while (iy >= 0)
	{
		if (gc->sdlcontext->ft[iy] > dist)
			break;
		iy--;
	}*/
	iy = 0;
	while (iy < WINDOW_H / 2)
	{
		if (gc->sdlcontext->ft[iy] > dist)
			break;
		iy++;
	}
	return (WINDOW_H / 2 + iy);
	//return ((WINDOW_H / 2) + iy);
}

void	renderobj(t_gamecontext *gc)
{
	int		scan_h;
	float	angle;
	float	o_angle;
	int		dist;
	t_fdf	*fdf;

	fdf = gc->sdlcontext->fdfs; //TODO do as param or iterate through the objects;

	angle = gc->player.angle + 1.57f;
	scan_h = 0;
	
	dist = v2dist(fdf->crd, (int [2]){gc->player.pos[X], gc->player.pos[Y]});
	fdf->scale = (WALLTHING * WINDOW_W) / dist;
	fdf->screenspace[X] = +300000;
	fdf->screenspace[Y] = screenspace_y(gc, dist) - (fdf->scale / 2);
	o_angle = atan2((float)fdf->crd[X] - gc->player.pos[X],
		(float)fdf->crd[Y] - gc->player.pos[Y]);
	fdf->view[Y] = o_angle;// * (180 / PI);
	printf("%f angle\n", o_angle);
	fdf->clock = &gc->clock;
	fdf_update(fdf);


	while (scan_h < WINDOW_W)
	{
		angle -= RAYSLICE;
		if (atan2(sin(angle), cos(angle)) <= o_angle + RAYSLICE &&
			atan2(sin(angle), cos(angle)) >= o_angle - RAYSLICE)
		{
			fdf->screenspace[X] = scan_h - (fdf->scale / 2);
			break ;
		}
		scan_h++;
	}
}