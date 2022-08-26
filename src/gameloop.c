/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/26 03:07:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"
#define RAY_LENGTH	200
#define WALLSCALE	2
#define GAMESCALE	40
#define RENDERSCALE	40

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

void	render2Dmap(t_sdlcontext context, uint32_t *map)
{
	int	crd[2];

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < MAPSIZE * RENDERSCALE)
	{
		while (crd[X] < MAPSIZE * RENDERSCALE)
		{
			SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 255);
			if (map [(crd[X] / RENDERSCALE) + (crd[Y] / RENDERSCALE) * MAPSIZE] != 0) {
				SDL_SetRenderDrawColor(context.renderer, 255, 255, 0, 255);
				drawimagescaled(context, crd, map [(crd[X] / RENDERSCALE) + (crd[Y] / RENDERSCALE) * MAPSIZE] - 1,
				RENDERSCALE);
			}
			drawrect(context.renderer, crd);
			crd[X] += RENDERSCALE;
		}
		crd[X] = 0;
		crd[Y] += RENDERSCALE;
	}
}

static void DrawLineWithRenderScale(SDL_Renderer *r, int *origin, int *dest)
{
	int f = (RENDERSCALE / GAMESCALE);
	SDL_RenderDrawLine(r, origin[X] * f, origin[Y] * f,
							dest[X] * f, dest[Y] * f);
}

int	raycast_len(int *crd, int *dst, t_gamecontext gc) //TODO use int[2] as params?
{
	int			len;
	t_bresenham	b;

	ft_bzero(&b, sizeof(t_bresenham));
	populate_bresenham(&b, crd, dst);
	while (b.local[X] > 0 && b.local[X] < MAPSIZE * GAMESCALE
			&& b.local[Y] > 0 && b.local[Y] < MAPSIZE * GAMESCALE)
	{
		step_bresenham_x(&b);
		step_bresenham_y(&b);
		if (gc.map[(b.local[Y] / GAMESCALE) * MAPSIZE + (b.local[X] / GAMESCALE)] != 0)
			break;
	}
	DrawLineWithRenderScale(gc.sdlcontext->renderer, crd, b.local);
	len = sqrt((crd[X] - b.local[X]) * (crd[X] - b.local[X]) + //TODO: make v2dist function
				(crd[Y] - b.local[Y]) * (crd[Y] - b.local[Y]));
	return len;
}

int		calc_wallheight(int wall)
{
	return (((RAY_LENGTH * GAMESCALE) - wall) * WALLSCALE);
}

void	rendergame(t_sdlcontext sdl, int *walls, int max)
{
	int	i;
	int offset = 300;
	int wallheight;

	SDL_SetRenderDrawColor(sdl.renderer, 255, 0, 0, 10);
	i = 0;
	while (i < max)
	{
		wallheight = 0;
		if (walls[i] < MAPSIZE * GAMESCALE)
			wallheight = (MAPSIZE * GAMESCALE) - (walls[i]);
		wallheight /= 4;
		SDL_RenderDrawLine(sdl.renderer,
			i + offset, offset - wallheight,
			i + offset, offset + wallheight);
		i++;
	}
}

//int	deltatime

int v2dist(int *v, int *ov)
{
	return (
			sqrt(
			(v[X] - ov[X]) * (v[X] - ov[X]) +
			(v[Y] - ov[Y]) * (v[Y] - ov[Y])
			)
			);
}

void moveplayer(t_player *plr, int deltatime)
{
	plr->pos[X] += plr->dest[X] * deltatime;
	plr->pos[Y] += plr->dest[Y] * deltatime;
	plr->angle += plr->rot * deltatime;
}

int *raycast(float playerpos[2], float angle, t_sdlcontext *sdl, t_gamecontext gc) //TODO: remove sdl context, only used for debug?
{
	static int	wallheights[512];
	int			scan_h;
	int32_t		ray_d[2];
	float		scan_angle;

	scan_h = 0;
	scan_angle = angle;
	SDL_SetRenderDrawColor(sdl->renderer, 20, 20, 255, 255);
	while (scan_h < 512)
	{
		scan_angle += 0.0025; //512 * 0.005 is under 1 rad
		ray_d[X] = sin(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[Y] = cos(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[X] += (int)playerpos[X];
		ray_d[Y] += (int)playerpos[Y];
		wallheights[scan_h] = raycast_len((int[2]){playerpos[X], playerpos[Y]}, ray_d, gc);
		scan_h++;
	}
	return (wallheights);
}

void	openmap(t_gamecontext *gc)
{
	int	fd;

	fd = open("map", O_RDONLY);
	read(fd, gc->map, MAPSIZE * MAPSIZE * sizeof(u_int32_t));
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
					(float [2]){ix * GAMESCALE, iy * GAMESCALE}, sizeof(float [2]));
			ix++;
		}
		ix = 0;
		iy++;
	}
	gc->player.angle = 0;
}

void	gameloop(t_gamecontext *gc)
{
	int i;

	openmap(gc);
	spawnplayer(gc);
	
	while (1)
	{
		if (eventloop(gc))
			return ;
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 0, 255);
		SDL_RenderClear(gc->sdlcontext->renderer);
		moveplayer(&gc->player, gc->clock.delta);
		update_deltatime(&gc->clock);
		render2Dmap(*gc->sdlcontext, gc->map);
		rendergame(*gc->sdlcontext,
			raycast(gc->player.pos, gc->player.angle, gc->sdlcontext, *gc),
			512);
		SDL_RenderPresent(gc->sdlcontext->renderer);
	}
}
