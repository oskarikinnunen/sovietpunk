/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/29 21:54:01 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP_OBJ.h"
#include "bresenham.h"
#define RAY_LENGTH	20
#define WALLSCALE	2
#define RENDERSCALE	10

#include "assert.h"

void	rendergame(t_sdlcontext *sdl, int *walls, t_gamecontext *gc)
{
	int			i;
	int			smpl[2];
	int			wallheight;
	float		ystep;
	t_simpleimg	img;

	i = -1;
	int rend = 0;
	while (i++ < WINDOW_W)
	{
		//wallheight = WALLTHING / (walls[i] & 0xFFFF);
		wallheight = walls[i] & 0xFFFF;
		img = sdl->images[walls[i] >> 24 & 0xFF];
		smpl[X] = ((walls[i] >> 16 & 0xFF) / 64.0f) * img.size[X];
		smpl[Y] = ft_clamp((wallheight / 2) - WINDOW_H, 0, wallheight);
		ystep = ((float)img.size[Y] / wallheight);
		while (++smpl[Y] *ystep < img.size[Y] - 1 && wallheight != 0)
		{
			if ((WINDOW_H / 2) + (smpl[Y] - wallheight / 2) > WINDOW_H)
				break ;
			((int *)sdl->surface->pixels)[i + ((WINDOW_H / 2)
					+ (smpl[Y] - wallheight / 2)) * WINDOW_W] = shade
				(samplecolor(img, smpl[X] + smpl[Y] * ystep, smpl[Y] * ystep),
					wallheight);
		}
	}
}

void	render2Dmap(t_sdlcontext *context, uint32_t *map)
{
	int	crd[2];

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < MAPSIZE * RENDERSCALE)
	{
		while (crd[X] < MAPSIZE * RENDERSCALE)
		{
			if (map [(crd[X] / RENDERSCALE) + (crd[Y] / RENDERSCALE) * MAPSIZE] != 0) {
				/*drawimagescaled(context, crd, map [(crd[X] / RENDERSCALE) + (crd[Y] / RENDERSCALE) * MAPSIZE] - 1,
				RENDERSCALE);*/
			}
			crd[X] += RENDERSCALE;
		}
		crd[X] = 0;
		crd[Y] += RENDERSCALE;
	}
}

void	floorcast(int **floor, t_bresenham *b, t_gamecontext *gc)
{
	int		left;
	int		i;

	left = WINDOW_H / 2;
	i = 0;
	while (i <= left)
	{
		while (v2dist(b->local, b->target) >= gc->sdlcontext->ft[i])
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

void moveplayer(t_player *plr, int deltatime, int *map)
{
	int	dest_index;

	dest_index = (int)(plr->pos[X] + (plr->dest[X] * deltatime)) / 64
				+ ((int)(plr->pos[Y] + (plr->dest[Y] * deltatime)) / 64) * MAPSIZE;
	if (map[dest_index] == 0 || map[dest_index] == 2)
	{
		plr->pos[X] += plr->dest[X] * deltatime;
		plr->pos[Y] += plr->dest[Y] * deltatime;
	}
	plr->angle += plr->rot * deltatime;
}

void	render_floor(t_sdlcontext sdl, int *floor, int ix, int h)
{
	int		iy;
	int		x;
	int		y;
	Uint32	clr;

	iy = 0;
	while (iy <= (WINDOW_H / 2))
	{
		x = (floor[iy] & 0xFF) * (sdl.images[0].size[X] / GAMESCALE);
		y = (floor[iy] >> 8) * (sdl.images[0].size[Y] / GAMESCALE);
		clr = samplecolor(sdl.images[0], x + y, x);
		clr = shade(clr, iy * 2);
		int oc = ((int *)sdl.surface->pixels)[ix + (iy + WINDOW_H / 2) * WINDOW_W];
		((int *)sdl.surface->pixels)[ix + (iy + WINDOW_H / 2) * WINDOW_W] = clr;
		((int *)sdl.surface->pixels)[ix + (WINDOW_H / 2 - iy) * WINDOW_W] = clr;
		iy++;
	}
}

static void printwalls(int walls[WINDOW_W]){
	for (int i = 0; i < WINDOW_W; i++)
	{
		printf("w %i \n", walls[i] & 0xFFFF);
		if (i > 0 && (walls[i - 1] & 0xFFFF) < (walls[i] & 0xFFFF)) {
			printf("ASSERT FAIL\n");
		}
			
		
	}
	exit(0);
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
	int i;
	int	*walls;

	openmap(&gc);
	spawnplayer(&gc);
	while (1)
	{
		if (eventloop(&gc))
			return ;
		update_deltatime(&gc.clock);
		moveplayer(&gc.player, gc.clock.delta, gc.map);
		//SDL_LockSurface(gc.sdlcontext->surface);
		walls = raycast(gc.player.pos, gc.player.angle, gc.sdlcontext, gc);
		rendergame(gc.sdlcontext,
			walls, &gc);
		//render2Dmap(gc.sdlcontext, gc.map);
		renderobj(&gc);
		//drawfdf(gc.sdlcontext, gc.sdlcontext->fdfs, walls);
		drawimagescaled(gc.sdlcontext, gc.sdlcontext->fdfs->screenspace, 3, gc.sdlcontext->fdfs->scale, walls);
		//SDL_UnlockSurface(gc.sdlcontext->surface);
		SDL_UpdateWindowSurface(gc.sdlcontext->window);
	}
}
