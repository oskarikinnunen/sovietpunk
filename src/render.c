/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:56:26 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 14:28:57 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

// 'i' here is just a norminette workaround, I know it's pretty disgusting
void	rendergame(t_sdlcontext *sdl, int *walls, t_gamecontext *gc, int i)
{
	int			smpl[2];
	int			pxlcrd[2];
	int			wallheight;
	float		ystep;
	t_simpleimg	img;

	while (i++ < WINDOW_W)
	{
		wallheight = walls[i] & 0xFFFF;
		img = sdl->images[ft_clamp(walls[i] >> 24 & 0xFF, 0, PNG_COUNT - 1)];
		smpl[X] = ((walls[i] >> 16 & 0xFF) / 64.0f) * img.size[X];
		smpl[Y] = ft_clamp((wallheight / 2) - WINDOW_H, 0, wallheight);
		ystep = ((float)img.size[Y] / wallheight);
		while (++smpl[Y] * ystep < img.size[Y] - 1 && wallheight != 0)
		{
			if (CENTER_Y + (smpl[Y] - wallheight / 2) >= WINDOW_H)
				break ;
			v2cpy(pxlcrd, (int [2])
			{i, (CENTER_Y + (smpl[Y] - wallheight / 2))});
			draw(sdl->surface->pixels, pxlcrd, shade
				(samplecolor(img, smpl[X] + smpl[Y] * ystep, smpl[Y] * ystep),
					wallheight));
		}
	}
}

static void	drawplayer_topdown(t_gamecontext *gc)
{
	float		plrcrd[2];
	int			linecrd[2];
	float		line_angle;
	int			screencrd[2];

	f2cpy(plrcrd, gc->player.pos);
	f2mul(plrcrd, 1.0f / GAMESCALE);
	f2mul(plrcrd, MAPTILESIZE);
	f2tov2(plrcrd, screencrd);
	drawcircle(gc->sdl.surface->pixels, screencrd, MAPTILESIZE / 4, CLR_PRPL);
	line_angle = gc->player.angle + (FOV);
	v2cpy(linecrd, screencrd);
	v2add(linecrd, (int [2])
	{sin(line_angle) * MAPTILESIZE * 2, cos(line_angle) * MAPTILESIZE * 2});
	drawline(gc->sdl.surface->pixels, screencrd, linecrd, CLR_PRPL);
	line_angle = gc->player.angle;
	v2cpy(linecrd, screencrd);
	v2add(linecrd, (int [2])
	{sin(line_angle) * MAPTILESIZE * 2, cos(line_angle) * MAPTILESIZE * 2});
	drawline(gc->sdl.surface->pixels, screencrd, linecrd, CLR_PRPL);
}

void	rendertopdownmap(t_gamecontext *gc)
{
	int			crd[2];
	int			mapcrd[2];
	uint32_t	tile;

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < MAPSIZE * MAPTILESIZE)
	{
		while (crd[X] < MAPSIZE * MAPTILESIZE)
		{
			v2cpy(mapcrd, crd);
			v2div(mapcrd, MAPTILESIZE);
			tile = samplemap(gc->map, mapcrd);
			drawquadtile(&gc->sdl, crd, tile, MAPTILESIZE + 2);
			crd[X] += MAPTILESIZE;
		}
		crd[X] = 0;
		crd[Y] += MAPTILESIZE;
	}
	drawplayer_topdown(gc);
}

void	render_floor(t_sdlcontext sdl, int16_t *floor, int ix, int h)
{
	int		iy;
	int		x;
	int		y;
	int		pxlcrd[2];
	Uint32	clr;

	iy = 0;
	while (iy <= CENTER_Y)
	{
		x = (floor[iy] & 0xFF) * (sdl.images[0].size[X] / GAMESCALE);
		y = (floor[iy] >> 8) * (sdl.images[0].size[Y] / GAMESCALE);
		clr = samplecolor(sdl.images[0], x + y, x);
		clr = shade(clr, iy * 2);
		v2cpy(pxlcrd, (int [2]){ix, iy + CENTER_Y});
		draw(sdl.surface->pixels, pxlcrd, clr);
		v2cpy(pxlcrd, (int [2]){ix, CENTER_Y - iy});
		draw(sdl.surface->pixels, pxlcrd, clr);
		iy++;
	}
}
