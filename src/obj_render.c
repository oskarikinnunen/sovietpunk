/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 17:35:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 12:17:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int	screenspace_y(t_gamecontext *gc, int dist)
{
	int	iy;

	iy = 0;
	while (iy < WINDOW_H / 2)
	{
		if (gc->sdl.ft[iy] > dist)
			break ;
		iy++;
	}
	return (WINDOW_H / 2 + iy);
}

static void	drawstripe(uint32_t *pixels, t_fdf fdf, int ix, float scalar)
{
	int			iy;
	int			py;
	uint32_t	clr;

	iy = 0;
	py = fdf.screenspace[Y];
	while (iy++ < fdf.scale - 1)
	{
		if (iy + py < 0 || iy + py >= WINDOW_H)
			continue ;
		clr = samplecolor(fdf.img, ix * scalar, iy * scalar);
		if (clr != 0)
			draw(pixels, (int [2]){ix + fdf.screenspace[X], iy + py}, clr);
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
		if (ix + ssx < 0 || ix + ssx >= WINDOW_W - 1
			|| (walls[ix + ssx] & 0xFFFF) > fdf.scale)
			continue ;
		drawstripe(sdl->surface->pixels, fdf, ix, scalar);
	}
}

void	renderobj(t_gamecontext *gc, t_fdf *fdf)
{
	int		scan_h;
	float	angle;
	int		dist;

	angle = gc->player.angle + FOV;
	dist = v2dist(fdf->crd, (int [2]){gc->player.pos[X], gc->player.pos[Y]});
	fdf->scale = WALLTHING / dist;
	fdf->screenspace[Y] = screenspace_y(gc, dist) - (fdf->scale / 2);
	fdf->screenspace[X] = 300000;
	fdf->view[Y] = atan2((float)fdf->crd[X] - gc->player.pos[X],
			(float)fdf->crd[Y] - gc->player.pos[Y]);
	fdf->clock = &gc->clock;
	scan_h = -1;
	while (scan_h++ < WINDOW_W)
	{
		angle -= RAYSLICE;
		if (atan2(sin(angle), cos(angle)) <= fdf->view[Y] + RAYSLICE
			&& atan2(sin(angle), cos(angle)) >= fdf->view[Y] - RAYSLICE)
		{
			fdf->screenspace[X] = scan_h - (fdf->scale / 2);
			break ;
		}
	}
	if (fdf->screenspace[X] != 300000)
		fdf_update(fdf);
}
