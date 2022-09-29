/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 17:35:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/29 21:53:45 by okinnune         ###   ########.fr       */
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

void	drawfdf(t_sdlcontext *sdl, t_fdf *fdf, int *walls)
{
	int			crd[2];
	int			color;
	float		scalar;

	scalar = ((float)fdf->img->size[X] / (float)fdf->scale);
	crd[Y] = fdf->screenspace[Y] - (fdf->scale / 2);
	while (crd[Y]++ < fdf->screenspace[Y] + (fdf->scale / 2) - 1)
	{
		crd[X] = fdf->screenspace[X] - (fdf->scale / 2);
		if (crd[Y] < 0 || crd[Y] > WINDOW_H)
			continue;
		while (crd[X]++ < fdf->screenspace[X] + (fdf->scale / 2) - 1)
		{
			if (crd[X] < 0 || crd[X] >= WINDOW_W)
				continue;
			color = samplecolor(*fdf->img, (float)(crd[X] - fdf->screenspace[X] / 2) * scalar
								, (float)(crd[Y] - fdf->screenspace[Y] / 2) * scalar); //cast to float?
			//printf("drawing \n");
			if (color != 0/* && (walls[crd[X]] & 0xFFFF) < fdf.scale*/)
			{
				int index = crd[X] + (crd[Y] * WINDOW_W);
				((int *)sdl->surface->pixels)[index] = color;
			}
		}
	}
}

void	renderobj(t_gamecontext *gc)
{
	int		scan_h;
	float	angle;
	float	o_angle;
	int		dist;
	t_fdf	*fdf;

	fdf = gc->sdlcontext->fdfs; //TODO do as param or iterate through the objects;
	angle = gc->player.angle + FOV;
	scan_h = 0;
	
	dist = v2dist(fdf->crd, (int [2]){gc->player.pos[X], gc->player.pos[Y]});
	fdf->scale = WALLTHING / dist;
	fdf->screenspace[X] = +300000;
	fdf->screenspace[Y] = screenspace_y(gc, dist) - (fdf->scale / 2);
	o_angle = atan2((float)fdf->crd[X] - gc->player.pos[X],
		(float)fdf->crd[Y] - gc->player.pos[Y]);
	fdf->view[Y] = o_angle;// * (180 / PI);
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