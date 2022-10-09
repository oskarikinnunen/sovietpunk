/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 17:35:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 23:20:46 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int		screenspace_y(t_gamecontext *gc, int dist) //only needs sdl
{
	int	iy;

	iy = 0;
	while (iy < WINDOW_H / 2)
	{
		if (gc->sdl.ft[iy] > dist)
			break;
		iy++;
	}
	return (WINDOW_H / 2 + iy);
}

static void drawstripe(uint32_t *pixels, t_fdf fdf, int ix, float scalar)
{
	int	iy;
	int	py;

	iy = 0;
	py = fdf.screenspace[Y];
	//printf("img size %i %i len %i \n", fdf.img.size[X], fdf.img.size[Y], fdf.img.length);
	while (iy++ < fdf.scale - 1)
	{
		if (iy + py < 0 || iy + py >= WINDOW_H)
			continue;
		
		uint32_t clr =
			samplecolor(fdf.img, ix * scalar, iy * scalar);
		
		if (clr != 0)
		{
			draw(pixels, (int[2]) {ix + fdf.screenspace[X], iy + py}, clr);
			//pixels[(iy + fdf.screenspace[Y]) * WINDOW_W] = clr;
		}
			
	}
}

void	drawfdf(t_sdlcontext *sdl, t_fdf fdf, int *walls)
{
	int			ix;
	int			ssx;
	float		scalar;

	scalar = ((float)fdf.img.size[X] / (float)fdf.scale);
	ssx = fdf.screenspace[X];
	ix = 0;
	while (ix++ < fdf.scale)
	{
		//p_x = ft_clamp(ix + ssx, 0, WINDOW_W - 1);
		if (ix + ssx < 0 || ix + ssx >= WINDOW_W
			||(walls[ix + ssx] & 0xFFFF) > fdf.scale)
			continue;
		drawstripe(sdl->surface->pixels, fdf, ix, scalar);
	}
}

void	renderobj(t_gamecontext *gc)
{
	int		scan_h;
	float	angle;
	float	o_angle;
	int		dist;
	t_fdf	*fdf;

	fdf = gc->sdl.fdfs;
	angle = gc->player.angle + FOV;
	dist = v2dist(fdf->crd, (int [2]){gc->player.pos[X], gc->player.pos[Y]});
	fdf->scale = WALLTHING / dist;
	fdf->screenspace[Y] = screenspace_y(gc, dist) - (fdf->scale / 2);
	fdf->screenspace[X] = 300000;
	o_angle = atan2((float)fdf->crd[X] - gc->player.pos[X],
		(float)fdf->crd[Y] - gc->player.pos[Y]);
	fdf->view[Y] = o_angle;
	fdf->clock = &gc->clock;
	scan_h = 0;
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
	if (fdf->screenspace[X] != 300000)
		fdf_update(fdf);
}