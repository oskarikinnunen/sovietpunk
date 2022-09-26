/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/26 21:09:37 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP_OBJ.h"
#include "bresenham.h"
#define RAY_LENGTH	200
#define WALLSCALE	2
#define GAMESCALE	64
#define RENDERSCALE	10

#include "assert.h"
//TODO make this generic and move to helper file
static void	drawrect(SDL_Renderer *r, int *crd)
{
	int	i;

	i = 0;
	while (i < RENDERSCALE)
	{
		SDL_RenderDrawPoint(r, crd[X] + i, crd[Y]);
		SDL_RenderDrawPoint(r, crd[X], crd[Y] + i);
		SDL_RenderDrawPoint(r, crd[X] + i, crd[Y] + RENDERSCALE);
		SDL_RenderDrawPoint(r, crd[X] + RENDERSCALE, crd[Y] + i);
		i++;
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
				drawimagescaled(context, crd, map [(crd[X] / RENDERSCALE) + (crd[Y] / RENDERSCALE) * MAPSIZE] - 1,
				RENDERSCALE);
			}
			crd[X] += RENDERSCALE;
		}
		crd[X] = 0;
		crd[Y] += RENDERSCALE;
	}
}

static void DrawLineWithRenderScale(SDL_Renderer *r, int *origin, int *dest)
{
	float f = ((float)RENDERSCALE / (float)GAMESCALE);
	SDL_RenderDrawLine(r, origin[X] * f, origin[Y] * f,
							dest[X] * f, dest[Y] * f);
}

int	floorcast(int **floor, t_bresenham *b, t_gamecontext *gc)
{
	int		left;
	int		i;

	left = WINDOW_H / 2;
	
	i = 0;
	while (i <= left)
	{
		while (v2dist(b->local, b->target) > gc->sdlcontext->ft[i])
			if (step_bresenham(b)) break;
		floor[0][i] = ((b->local[Y] % GAMESCALE) & 0xFF) + ((b->local[X] % GAMESCALE) << 8);
		i++;
	}
}

int	raycast_len(int crd[2], int dst[2], t_gamecontext *gc, int *floor) //TODO use int[2] as params?
{
	static	t_bresenham	b;
	int		tex_sample;
	int		res;

	populate_bresenham(&b, crd, dst);
	while (b.local[X] > 0 && b.local[X] < MAPSIZE * GAMESCALE
			&& b.local[Y] > 0 && b.local[Y] < MAPSIZE * GAMESCALE
			&& !step_bresenham(&b))
	{
		if (gc->map[((b.local[Y] & 0xFFFFFFC0) >> 6) * MAPSIZE
			+ ((b.local[X] & 0xFFFFFFC0) >> 6)] != 0)
			break;
	}
	tex_sample = b.local[X] % GAMESCALE;
	if (b.local[X] % GAMESCALE == 0 ||
		b.local[X] % GAMESCALE == GAMESCALE - 1)
		tex_sample = b.local[Y] % GAMESCALE;
	res = v2dist(crd, b.local);
	b.local[Y] = ft_clamp(b.local[Y], 0, (MAPSIZE * GAMESCALE));
	b.local[X] = ft_clamp(b.local[X], 0, (MAPSIZE * GAMESCALE));
	int tx = (gc->map[((b.local[Y] & 0xFFFFFFC0) >> 6) * MAPSIZE
			+ ((b.local[X] & 0xFFFFFFC0) >> 6)] & 0xFF);
	tx = tx << 24;

	populate_bresenham(&b, b.local, crd);
	floorcast(&floor, &b, gc);
	tex_sample = tex_sample & 0xFF;
	
	//assert(tex_sample <= 64);
	res += (tex_sample << 16) + tx;
	return (res);
}

int		calc_wallheight(int wall)
{
	return (((RAY_LENGTH * GAMESCALE) - wall) * WALLSCALE);
}

void	rendergame(t_sdlcontext *sdl, int *walls, int max)
{
	int	i;
	int offset = 300;
	int wallheight;

	//SDL_SetRenderDrawColor(sdl.renderer, 255, 0, 0, 10);
	i = 0;
	while (i < max)
	{
		wallheight = 0;
		//if (walls[i] & 0xFFFF != 0)
		wallheight = (WALLTHING * WINDOW_W) / (walls[i] & 0xFFFF);
		float iy = 0.0f;
		t_simpleimg img = sdl->images[walls[i] >> 24 & 0xFF];
		int ix = ((float)(walls[i] >> 16 & 0xFF) / 64.0f) * img.size[X];
		float ystep = ((float)img.size[Y] / (float)wallheight);
		while (iy < wallheight - 1 && wallheight != 0)
		{
			Uint32 clr = samplecolor(img, ix + iy * ystep, iy * ystep);
			clr = shade(clr, wallheight);
			int ind = i + (int)(WINDOW_H / 2 + iy - (wallheight / 2)) * WINDOW_W;
			if (ind < 0) ind = 0;
			if (ind > WINDOW_H * WINDOW_W) ind = WINDOW_H * WINDOW_W;
			((int *)sdl->surface->pixels)[ind] = clr;
			iy++;
		}
		i++;
	}
}

void moveplayer(t_player *plr, int deltatime, int *map)
{
	int	dest_index;

	dest_index = (int)(plr->pos[X] + (plr->dest[X] * deltatime)) / 64
				+ ((int)(plr->pos[Y] + (plr->dest[Y] * deltatime)) / 64) * MAPSIZE;
	if (map[dest_index] == 0 || map[dest_index] == 3)
	{
		plr->pos[X] += plr->dest[X] * deltatime;
		plr->pos[Y] += plr->dest[Y] * deltatime;
	}
	
	plr->angle += plr->rot * deltatime;
}

void	render_floor(t_sdlcontext *sdl, int *floor, int ix, int h)
{
	int	iy = 0;

	//assert ((int)WALLTHING == 54);

	//int wh = ((int)WALLTHING * WINDOW_W) / h;
	while (iy <= (WINDOW_H / 2))
	{
		int x = (floor[iy] & 0xFF) * (sdl->images[0].size[X] / GAMESCALE); //TODO replace with real texture size;
		int y = (floor[iy] >> 8) * (sdl->images[0].size[Y] / GAMESCALE);
		
		int clr = samplecolor(sdl->images[0], x + y, x);
		if (x != 0 && y != 0)
			clr = shade(clr, iy * 2);
		((int *)sdl->surface->pixels)[ix + (iy + WINDOW_H / 2) * WINDOW_W] = clr;
		((int *)sdl->surface->pixels)[ix + ft_clamp((WINDOW_H / 2 - iy), 0, WINDOW_H) * WINDOW_W] = clr;
		iy++;
	}
	//printf("ix %i \n", ix);
}

int *raycast(float playerpos[2], float angle, t_sdlcontext *sdl, t_gamecontext gc) //TODO: remove sdl context, only used for debug?
{
	static int			wallheights[WINDOW_W];
	static int			floor_tex[WINDOW_W];
	int					scan_h;
	int32_t				ray_d[2];
	float				scan_angle;

	scan_h = 0;
	scan_angle = angle + 1.57;
	//floorcast(*sdl, gc.player.pos, (float [2]){scan_angle, scan_angle - (512 * 0.0022)}); //TODO make fov define
	gc.sdlcontext->fdfs->screenspace[X] = -200;
	while (scan_h < WINDOW_W)
	{
		scan_angle -= RAYSLICE; //512 * 0.005 is under 1 rad
		ray_d[X] = sin(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[Y] = cos(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[X] += (int)playerpos[X];
		ray_d[Y] += (int)playerpos[Y];
		gc.v[X] = scan_h;
		//Set camera plane vector and use in raycastlen. TODO: fix global state issue
		//SDL_RenderDrawLine(sdl->renderer)
		wallheights[scan_h] = raycast_len((int[2]){playerpos[X], playerpos[Y]}, ray_d, &gc, floor_tex);
		//wallheights[scan_h] += (gc.tex_x << 16);
		render_floor(sdl, floor_tex, scan_h, wallheights[scan_h] & 0xFFFF);
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
				ft_memcpy(gc->player.pos,
					(float [2]){(ix * GAMESCALE) - 0.5f, (iy * GAMESCALE) - 0.5f}, sizeof(float [2]));
			ix++;
		}
		ix = 0;
		iy++;
	}
	gc->player.angle = 0;
}

void	gameloop(t_gamecontext gc)
{
	int i;

	openmap(&gc);
	spawnplayer(&gc);
	while (1)
	{
		if (eventloop(&gc))
			return ;
		//SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 0, 255);
		//SDL_RenderClear(gc->sdlcontext->renderer);
		//SDL_RenderPresent(gc->sdlcontext->renderer);
		update_deltatime(&gc.clock);
		moveplayer(&gc.player, gc.clock.delta, gc.map);
		
		//SDL_LockSurface(gc->sdlcontext->surface);
		
		rendergame(gc.sdlcontext,
			raycast(gc.player.pos, gc.player.angle, gc.sdlcontext, gc),
			WINDOW_W);
			
		
		renderobj(&gc);
		render2Dmap(gc.sdlcontext, gc.map);
		drawimagescaled(gc.sdlcontext, gc.sdlcontext->fdfs->screenspace, 3, gc.sdlcontext->fdfs->scale);
		//drawimagescaled(gc.sdlcontext, gc.sdlcontext->objs->screenspace, 3, 200);
		//SDL_RenderCopy(gc->sdlcontext->renderer, gc->sdlcontext->tex, NULL, NULL);
		//SDL_UnlockSurface(gc->sdlcontext->surface);
		SDL_UpdateWindowSurface(gc.sdlcontext->window);
		
	}
}
