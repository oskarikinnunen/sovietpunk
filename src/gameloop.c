/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/07 16:25:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP_OBJ.h"
#include "bresenham.h"
#define RAY_LENGTH	20
#define WALLSCALE	2
#define MAPTILESIZE	10

#include "assert.h"

void	rendergame(t_sdlcontext *sdl, int *walls, t_gamecontext *gc)
{
	int			i;
	int			smpl[2];
	int			pxlcrd[2];
	int			wallheight;
	float		ystep;
	t_simpleimg	img;

	i = -1;
	int rend = 0;
	while (i++ < WINDOW_W)
	{
		wallheight = walls[i] & 0xFFFF;
		img = sdl->images[walls[i] >> 24 & 0xFF];
		smpl[X] = ((walls[i] >> 16 & 0xFF) / 64.0f) * img.size[X];
		smpl[Y] = ft_clamp((wallheight / 2) - WINDOW_H, 0, wallheight);
		ystep = ((float)img.size[Y] / wallheight);
		while (++smpl[Y] *ystep < img.size[Y] - 1 && wallheight != 0)
		{
			if ((WINDOW_H / 2) + (smpl[Y] - wallheight / 2) > WINDOW_H)
				break ;
			v2cpy(pxlcrd, (int [2]){i, ((WINDOW_H / 2) +  (smpl[Y] - wallheight / 2))} );
			int clr = shade
				(samplecolor(img, smpl[X] + smpl[Y] * ystep, smpl[Y] * ystep),
				wallheight);
			draw(sdl->surface->pixels, pxlcrd, clr);
		}
	}
}

static void drawplayer2D(t_gamecontext *gc)
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
	v2add(linecrd, (int [2]){sin(line_angle) * MAPTILESIZE * 2, cos(line_angle) * MAPTILESIZE * 2});
	drawline(gc->sdl.surface->pixels, screencrd, linecrd, CLR_PRPL);
	line_angle = gc->player.angle;
	v2cpy(linecrd, screencrd);
	v2add(linecrd, (int [2]){sin(line_angle) * MAPTILESIZE * 2, cos(line_angle) * MAPTILESIZE * 2});
	drawline(gc->sdl.surface->pixels, screencrd, linecrd, CLR_PRPL);
}

void	render2Dmap(t_gamecontext *gc)
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
			drawimagescaled(&gc->sdl, crd, tile, MAPTILESIZE);
			drawrect(gc->sdl.surface->pixels, crd, CLR_GRAY, MAPTILESIZE);
			crd[X] += MAPTILESIZE;
		}
		crd[X] = 0;
		crd[Y] += MAPTILESIZE;
	}
	drawplayer2D(gc);
}

void	floorcast(int **floor, t_bresenham *b, t_gamecontext *gc)
{
	int		left;
	int		i;

	left = WINDOW_H / 2;
	i = 0;
	while (i <= left)
	{
		while (v2dist(b->local, b->target) >= gc->sdl.ft[i])
		{
			if (step_bresenham(b))
				break;
		}
		floor[0][i] = ((b->local[Y] % GAMESCALE) & 0xFF)
			+ ((b->local[X] % GAMESCALE) << 8);
		i++;
	}
}

int	raycast_len(int crd[2], int dst[2], t_gamecontext *gc, int *floor)
{
	static t_bresenham	b;
	int					tex_sample;
	int					res;
	int					tx;
	int					mapcrd[2];

	populate_bresenham(&b, crd, dst);
	while (b.local[X] > 0 && b.local[X] < MAPSIZE * GAMESCALE
		&& b.local[Y] > 0 && b.local[Y] < MAPSIZE * GAMESCALE
		&& !step_bresenham(&b) && gc->map[((b.local[Y] & 0xFFFFFFC0) >> 6)
			* MAPSIZE + ((b.local[X] & 0xFFFFFFC0) >> 6)] == 0)
		;
	tex_sample = b.local[X] % GAMESCALE;
	if (b.local[X] % GAMESCALE == 0
		|| b.local[X] % GAMESCALE == GAMESCALE - 1)
		tex_sample = b.local[Y] % GAMESCALE;
	res = WALLTHING / v2dist(crd, b.local);
	tx = (gc->map[((b.local[Y] & 0xFFFFFFC0) >> 6) * MAPSIZE
			+ ((b.local[X] & 0xFFFFFFC0) >> 6)] & 0xFF);
	tx = ft_clamp(tx, 0, 3) << 24;
	populate_bresenham(&b, b.local, crd);
	floorcast(&floor, &b, gc);
	res += ((tex_sample & 0xFF) << 16) + tx;
	return (res);
}

static void	updatemovementvector(float move_f2[2], int32_t keystate, float angle)
{
	angle = angle + (FOV / 2.0f);
	if ((keystate >> KEYS_UPMASK) & 1) 
	{
		move_f2[X] += sin(angle);
		move_f2[Y] += cos(angle);
	}
	if ((keystate >> KEYS_DOWNMASK) & 1)
	{
		move_f2[X] -= sin(angle);
		move_f2[Y] -= cos(angle);
	}
	if ((keystate >> KEYS_LEFTMASK) & 1)
	{
		move_f2[X] += sin(angle + RAD90);
		move_f2[Y] += cos(angle + RAD90);
	}
	if ((keystate >> KEYS_RGHTMASK) & 1)
	{
		move_f2[X] += -sin(angle + RAD90);
		move_f2[Y] += -cos(angle + RAD90);
	}
}

void moveplayer(t_gamecontext *gc)
{
	float	move_f2[2];
	float	angle;
	float	potential_plrpos[2];
	int		mapcrd[2];

	ft_bzero(move_f2, sizeof(float [2]));
	angle = 0;
	angle -= gc->mouse_delta[X] * MOUSESPEED;
	angle *= gc->clock.delta;
	gc->player.angle += angle;
	updatemovementvector(move_f2, gc->keystate, gc->player.angle);
	f2mul(move_f2, gc->clock.delta * MOVESPEED);
	f2cpy(potential_plrpos, gc->player.pos);
	f2add(potential_plrpos, move_f2);
	f2mul(potential_plrpos, 1.0f / GAMESCALE);
	f2tov2(potential_plrpos, mapcrd);
	if (is_in_map(mapcrd) && samplemap(gc->map, mapcrd) == 0)
		f2add(gc->player.pos, move_f2);
}

void	render_floor(t_sdlcontext sdl, int *floor, int ix, int h)
{
	int		iy;
	int		x;
	int		y;
	int		pxlcrd[2];
	Uint32	clr;

	iy = 0;
	while (iy <= (WINDOW_H / 2))
	{
		x = (floor[iy] & 0xFF) * (sdl.images[0].size[X] / GAMESCALE);
		y = (floor[iy] >> 8) * (sdl.images[0].size[Y] / GAMESCALE);
		clr = samplecolor(sdl.images[0], x + y, x);
		clr = shade(clr, iy * 2);
		int oc = ((int *)sdl.surface->pixels)[ix + (iy + WINDOW_H / 2) * WINDOW_W];
		v2cpy(pxlcrd, (int [2]) {ix, iy + WINDOW_H / 2});
		draw(sdl.surface->pixels, pxlcrd, clr);
		v2cpy(pxlcrd, (int [2]) {ix, (WINDOW_H / 2)- iy});
		draw(sdl.surface->pixels, pxlcrd, clr);
		iy++;
	}
}

int *raycast(float playerpos[2], float angle, t_sdlcontext *sdl, t_gamecontext gc) //TODO: remove sdl context, only used for debug?
{
	static int			wallheights[WINDOW_W];
	static int			floor_tex[WINDOW_W];
	int					scan_h;
	int32_t				ray_d[2];
	float				scan_angle;

	scan_h = 0;
	scan_angle = angle + FOV;
	while (scan_h < WINDOW_W)
	{
		ray_d[X] = sin(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[Y] = cos(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[X] += (int)playerpos[X];
		ray_d[Y] += (int)playerpos[Y];
		wallheights[scan_h] = raycast_len((int[2]){playerpos[X], playerpos[Y]}, ray_d, &gc, floor_tex);
		render_floor(*sdl, floor_tex,
			scan_h, wallheights[scan_h] & 0xFFFF);
		scan_angle -= RAYSLICE;
		scan_h++;
	}
	return (wallheights);
}

void	openmap(t_gamecontext *gc)
{
	int	fd;

	fd = open("map", O_RDONLY);
	(void)!read(fd, gc->map, MAPSIZE * MAPSIZE * sizeof(u_int32_t)); //TODO: check that we read the correct amount
	close(fd);
}

void	gameloop(t_gamecontext gc)
{
	int	*walls;
	int	mapcrd[2];

	openmap(&gc);
	spawnplayer(&gc);
	printf("SPAWNPLAYER");
	while (1)
	{
		update_deltatime(&gc.clock);
		if (eventloop(&gc))
			break ; //TODO: is good mby?
		moveplayer(&gc);
		walls = raycast(gc.player.pos, gc.player.angle, &gc.sdl, gc);
		rendergame(&gc.sdl, walls, &gc);
		renderobj(&gc);
		drawfdf(&gc.sdl, gc.sdl.fdfs[0], walls);
		render2Dmap(&gc);
		//drawcircle(gc.sdl.surface->pixels, (int [2]) {100, 100}, 60, INT_MAX);
		SDL_UpdateWindowSurface(gc.sdl.window);
	}
	exit(0);
}
