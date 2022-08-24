/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/24 23:44:39 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"
#define MAP_W 8
#define MAP_H 8
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

void	render2Dmap(t_sdlcontext context)
{
	int	crd[2];

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < MAP_H * RENDERSCALE)
	{
		while (crd[X] < MAP_W * RENDERSCALE)
		{
			SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 255);
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

int	raycast_len(int *crd, int *dst, t_sdlcontext sdl) //TODO use int[2] as params?
{
	int			len;
	t_bresenham	b;
	static Uint32 map[8][8] =
	{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 0}
	};

	ft_bzero(&b, sizeof(t_bresenham));
	populate_bresenham(&b, crd, dst);
	while (b.local[X] > 0 && b.local[X] < MAP_W * GAMESCALE
			&& b.local[Y] > 0 && b.local[Y] < MAP_H * GAMESCALE)
	{
		step_bresenham_x(&b);
		if (map[(b.local[Y] / GAMESCALE)][b.local[X] / GAMESCALE] == 1)
			break;
		step_bresenham_y(&b);
		if (map[(b.local[Y] / GAMESCALE)][b.local[X] / GAMESCALE] == 1)
			break;
	}
	DrawLineWithRenderScale(sdl.renderer, crd, b.local);
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

	SDL_SetRenderDrawColor(sdl.renderer, 255, 0, 0, 255);
	i = 0;
	while (i < max)
	{
		wallheight = 0;
		if (walls[i] < MAP_H * GAMESCALE)
			wallheight = (MAP_H * GAMESCALE) - (walls[i]);
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
	plr->pos[X] += plr->dest[X];
	plr->pos[Y] += plr->dest[Y];
	plr->angle += plr->rot * deltatime;
}

int *raycast(float playerpos[2], float angle, t_sdlcontext *sdl) //TODO: remove sdl context, only used for debug?
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
		wallheights[scan_h] = raycast_len((int[2]){playerpos[X], playerpos[Y]}, ray_d, *sdl);
		scan_h++;
	}
	return (wallheights);
}

void	gameloop(t_gamecontext *gc)
{
	ft_bzero(gc->player.sp_pos, sizeof(int32_t[2]));
	gc->player.pos[X] = 3;
	gc->player.pos[Y] = 6;
	gc->player.angle = 0;
	while (1)
	{
		if (eventloop(gc))
			return ;
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 0, 255);
		SDL_RenderClear(gc->sdlcontext->renderer);
		moveplayer(&gc->player, gc->clock.delta);
		update_deltatime(&gc->clock);
		render2Dmap(*gc->sdlcontext);
		rendergame(*gc->sdlcontext,
			raycast(gc->player.pos, gc->player.angle, gc->sdlcontext),
			512);
		SDL_RenderPresent(gc->sdlcontext->renderer);
	}
}
