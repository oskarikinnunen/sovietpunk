/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/07 17:35:29 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/07 11:54:15 by okinnune         ###   ########.fr       */
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

static void drawstripe(int *pixels, t_fdf fdf, int ix, float scalar)
{
	int	iy;
	int	py;

	iy = 0;
	py = fdf.screenspace[Y];
	while (iy++ < fdf.scale - 1)
	{
		if (iy + py < 0 || iy + py >= WINDOW_H)
			continue;
		//int clr = 0;
		int clr = samplecolor(fdf.img, ix * scalar, iy * scalar);
		if (clr != 0)
			pixels[(iy + fdf.screenspace[Y]) * WINDOW_W] = clr;
	}
}

void	drawfdf(t_sdlcontext *sdl, t_fdf fdf, int *walls)
{
	int			ix;
	int			ssx;
	int			p_x;
	float		scalar;

	scalar = ((float)fdf.img.size[X] / (float)fdf.scale);
	ssx = fdf.screenspace[X];
	ix = 0;
	printf("drawfdf \n");
	while (ix++ < fdf.scale)
	{
		p_x = ft_clamp(ix + ssx, 0, WINDOW_W - 1);
		if (ix + ssx < 0 || ix + ssx >= WINDOW_W
			/*||(walls[p_x] & 0xFFFF) > fdf.scale*/)
			continue;
		//drawstripe(((int *)sdl->surface->pixels) + p_x, fdf, ix, scalar);
	}
	printf("drawfdf complete \n");
}

void	renderobj(t_gamecontext *gc)
{
	int		scan_h;
	float	angle;
	float	o_angle;
	int		dist;
	t_fdf	*fdf;

	
	fdf = gc->sdl.fdfs; //TODO do as param or iterate through the objects;
	angle = gc->player.angle + FOV;
	scan_h = 0;
	
	dist = v2dist(fdf->crd, (int [2]){gc->player.pos[X], gc->player.pos[Y]});
	
	fdf->scale = WALLTHING / dist;
	
	fdf->screenspace[X] = +300000;
	
	printf("b4 screen space y \n");
	fdf->screenspace[Y] = screenspace_y(gc, dist) - (fdf->scale / 2);
	
	o_angle = atan2((float)fdf->crd[X] - gc->player.pos[X],
		(float)fdf->crd[Y] - gc->player.pos[Y]);
	fdf->view[Y] = o_angle;// * (180 / PI);
	fdf->clock = &gc->clock;
	
	printf("b4 scanh \n");
	while (scan_h < WINDOW_W)
	{
		angle -= RAYSLICE;
		if (atan2(sin(angle), cos(angle)) <= o_angle + RAYSLICE &&
			atan2(sin(angle), cos(angle)) >= o_angle - RAYSLICE)
		{
			
			fdf->screenspace[X] = scan_h - (fdf->scale / 2);
			printf("set screenspace x \n");
			break ;
		}
		scan_h++;
	}
	if (fdf->screenspace[X] != 300000)
		fdf_update(fdf);
}