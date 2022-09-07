/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/07 21:46:36 by okinnune         ###   ########.fr       */
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
			//SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 255);
			if (map [(crd[X] / RENDERSCALE) + (crd[Y] / RENDERSCALE) * MAPSIZE] != 0) {
				//SDL_SetRenderDrawColor(context->renderer, 255, 255, 0, 255);
				drawimagescaled(context, crd, map [(crd[X] / RENDERSCALE) + (crd[Y] / RENDERSCALE) * MAPSIZE] - 1,
				RENDERSCALE);
			}
			//drawrect(context.renderer, crd);
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

int	floorcast(int **floor, t_bresenham *b, int h, t_gamecontext *gc)
{
	float	step;
	int		left;
	int		i;

	left = WINDOW_H / 2;
	step = (float)(h) / (float)(left);
	
	i = 0;
	while (i++ < left)
	{
		while (v2dist(b->local, b->target) > gc->sdlcontext->ft[i])
			if (step_bresenham(b)) break;
		floor[0][i] = ((b->local[Y] % GAMESCALE) & 0xFF) + ((b->local[X] % GAMESCALE) << 8);
		if (gc->sdlcontext->objs->crd[X] == b->local[X] && gc->sdlcontext->objs->crd[Y] == b->local[Y])
		{
			//gc->sdlcontext->objs->screenspace[X] = gc->v[X];
			gc->sdlcontext->objs->screenspace[Y] = (WINDOW_H / 2) + i;
			gc->sdlcontext->objs->scale = (WALLTHING * WINDOW_W) / v2dist((int [2]){gc->player.pos[X],gc->player.pos[Y]}, b->local);
		}
		//if 
		//if current b.local is same as objects, calculate screen space position of object somehow??
		//screenspace x comes further up in the callstack, in raycast, i == y screen space coordinate
	}
}

int	raycast_len(int crd[2], int dst[2], t_gamecontext *gc, int *floor) //TODO use int[2] as params?
{
	static t_bresenham	b;

	populate_bresenham(&b, crd, dst);
	while (b.local[X] > 0 && b.local[X] < MAPSIZE * GAMESCALE
			&& b.local[Y] > 0 && b.local[Y] < MAPSIZE * GAMESCALE)
	{
		if (step_bresenham(&b)) break; //Move until masked value changes?
		if (gc->map[((b.local[Y] & 0xFFFFFFC0) >> 6) * MAPSIZE + ((b.local[X] & 0xFFFFFFC0) >> 6)] != 0)
		{
			gc->tex_x = (b.local[Y] / GAMESCALE) - b.local[Y];
			break;
		}
	}
	gc->tex_x = b.local[X] % GAMESCALE;
	if (b.local[X] % GAMESCALE == 0 || b.local[X] % GAMESCALE == GAMESCALE - 1) //removed debug
		gc->tex_x = b.local[Y] % GAMESCALE;
	int h = v2dist(crd, b.local);
	populate_bresenham(&b, b.local, crd);
	floorcast(&floor, &b, h, gc);
	return (h);
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
		if ((walls[i] & 0xFFFF) != 0)
			wallheight = (WALLTHING * WINDOW_W) / (walls[i] & 0xFFFF);
		float iy = 0.0f;
		float ty = 0.0f;
		int ix = ((float)(walls[i] >> 16) / 64.0f) * sdl->images[1].size[X];
		float ystep = ((float)sdl->images[1].size[Y] / (float)wallheight);
		while (iy < wallheight - 1 && wallheight != 0)
		{
			//Uint32 clr = samplecolor(sdl->images[1], ix + (iy * ystep), (int)(iy * ystep));
			Uint32 clr = sdl->images[1].data[(int)(ix + iy * ystep) + (int)(iy * ystep) * sdl->images[1].size[X]];
			//float mul = ft_clampf((float)((float)wallheight / (float)128), 0, 1.0f);
			float mul  = (float)((float)wallheight / 128.0f) < 1.0f ? (float)((float)wallheight / 128.0f) : 1.0f;
			int r = (clr & 0xFF) * mul * mul;
			int g = (clr >> 8 & 0xFF) * mul;
			int b = (clr >> 16 & 0xFF) * mul;
			int ind = i + (int)(300 + iy - (wallheight / 2)) * WINDOW_W;
			if (ind < 0) ind = 0;
			if (ind > WINDOW_H * WINDOW_W) ind = WINDOW_H * WINDOW_W;
			//int ind = i + (int)(300 + iy - (wallheight / 2)) * WINDOW_W;
			//clr = b + (g << 8) + (r << 16);
			((int *)sdl->surface->pixels)[ind] = clr;
			iy++;
		}
		/*SDL_RenderDrawLine(sdl.renderer,
			i + offset, offset - wallheight,
			i + offset, offset + wallheight);*/
		i++;
	}
}

//int	deltatime

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
	int wh = ((int)WALLTHING * WINDOW_W) / h;
	while (iy < (WINDOW_H / 2) - 1)
	{
		int x = (floor[iy] & 0xFF) * (128 / GAMESCALE); //TODO replace with real texture size;
		int y = (floor[iy] >> 8) * (128 / GAMESCALE);
		int clr = samplecolor(sdl->images[1], x + y, x);
		float mul = 0.25f + (float)iy / (float)(WINDOW_H / 2);
		int r = (clr & 0xFF) * mul * mul;
		int g = (clr >> 8 & 0xFF) * mul;
		int b = (clr >> 16 & 0xFF) * mul;
		clr = (b & 0xFF) + (g << 8) + (r << 16);
		
		((int *)sdl->surface->pixels)[ix + (iy + 300) * WINDOW_W] = clr;
		((int *)sdl->surface->pixels)[ix + ft_clamp((300 - iy), 0, WINDOW_H) * WINDOW_W] = clr;
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
	gc.sdlcontext->objs->screenspace[X] = -200;
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
		wallheights[scan_h] += (gc.tex_x << 16);
		render_floor(sdl, floor_tex, scan_h, wallheights[scan_h] & 0xFFFF);
		scan_h++;
	}
	
	return (wallheights);
}

void	openmap(t_gamecontext *gc)
{
	int	fd;

	fd = open("map", O_RDONLY);
	(void)!read(fd, gc->map, MAPSIZE * MAPSIZE * sizeof(u_int32_t));
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
			if (gc->map[ix + (iy * MAPSIZE)] == 3)
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
	gc.sdlcontext->objs = ft_memalloc(sizeof(t_fdf)); //TODO: nullcheck and move to another function
	t_obj	o;
	parse_obj(&o);
	fdf_init(gc.sdlcontext->objs, &gc.sdlcontext->images[3], &o);
	gc.sdlcontext->objs->crd[X] = (int)gc.player.pos[X] + 120;
	gc.sdlcontext->objs->crd[Y] = (int)gc.player.pos[Y] - 20;
	//gc.sdlcontext->objs->view[X] = -90.0f;

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
			
		//render2Dmap(gc.sdlcontext, gc.map);
		renderobj(&gc);
		drawimagescaled(gc.sdlcontext, gc.sdlcontext->objs->screenspace, 3, gc.sdlcontext->objs->scale);
		//drawimagescaled(gc.sdlcontext, gc.sdlcontext->objs->screenspace, 3, 200);
		//SDL_RenderCopy(gc->sdlcontext->renderer, gc->sdlcontext->tex, NULL, NULL);
		//SDL_UnlockSurface(gc->sdlcontext->surface);
		SDL_UpdateWindowSurface(gc.sdlcontext->window);
		
	}
}
