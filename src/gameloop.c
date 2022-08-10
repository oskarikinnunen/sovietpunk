/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/11 01:48:02 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"
#define MAP_W 8
#define MAP_H 8
#define RAY_LENGTH	200
#define WALLSCALE	2
#define GAMESCALE	10
#define RENDERSCALE	20
#define TURNSPEED	0.002f
#define MOVESPEED	50


static void	drawrect(SDL_Renderer *r, int *crd) //TODO make this generic and add to helper file
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
		step_bresenham(&b);
		if (b.local[Y] % 10 != 0 || b.local[X] % 10 != 0)
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

	i = 0;
	while (i < max)
	{
		wallheight = (MAP_H * GAMESCALE) - (walls[i]);
		//printf("%i \n", wallheight);
		//wallheight *= 0.05f;
		SDL_RenderDrawLine(sdl.renderer,
			i + offset, offset - wallheight,
			i + offset, offset + wallheight);
		i++;
	}
	//printf("\n");
	//exit(0);
}

//int	deltatime

int	ismovement(SDL_KeyCode code)
{
	return (code == SDLK_LEFT || code == SDLK_RIGHT || code == SDLK_UP || code == SDLK_DOWN) ||
			(code == SDLK_a || code == SDLK_d);
}

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
	t_bresenham	b; // static?
	int			i;
	int32_t		to[2];

	to[X] = plr->dest[X];
	to[Y] = plr->dest[Y];
	populate_bresenham(&b, plr->sp_pos, to);
	i = 0;
	if (plr->dest[X] != 0 && plr->dest[Y] != 0)
	{
		printf("plr pos %i %i, mov %i %i, distance %i d2 %i\n", plr->pos[X], plr->pos[Y], to[X], to[Y],
											v2dist(plr->pos, to), v2dist(to, b.local));
		while (v2dist(plr->sp_pos, b.local) < deltatime)
		{
			step_bresenham(&b);
			//printf("br %i %i \n", b.local[X], b.local[Y]);
		}
		plr->sp_pos[X] = b.local[X];
		plr->sp_pos[Y] = b.local[Y];
		printf("%i %i \n", plr->sp_pos[X], plr->sp_pos[Y]);
		if (ft_abs(plr->sp_pos[X]) > MOVESPEED)
		{
			plr->pos[X] += (plr->sp_pos[X] < 0) ? -1 : 1;
			plr->sp_pos[X] = 0;
		}
		if (ft_abs(plr->sp_pos[Y]) > MOVESPEED)
		{
			plr->pos[Y] += (plr->sp_pos[Y] < 0) ? -1 : 1;
			plr->sp_pos[Y] = 0;
		}
	}
	//printf("distance %i \n", v2dist(plr->pos, b.local));
	
	//plr->pos[X] = b.local[X];
	//plr->pos[Y] = b.local[Y];

	//plr->pos[X] += plr->mov[X] * deltatime * MOVESPEED;
	//plr->pos[Y] += plr->mov[Y] * deltatime * MOVESPEED;
	plr->angle += plr->rot * deltatime;
}

void	playerinput(SDL_Keycode kc, t_player *plr)
{
	plr->rot = (kc == SDLK_RIGHT) * TURNSPEED;
	plr->rot -= (kc == SDLK_LEFT) * TURNSPEED;
	if (kc == SDLK_UP)
	{
		plr->dest[X] = sin(plr->angle + 0.63) * 1000; //Have plr destination variable and move player with bresenham?? must be a simpler way to do it tho.
		plr->dest[Y] = cos(plr->angle + 0.63) * 1000;
	}
	if (kc == SDLK_DOWN)
	{
		plr->dest[X] = sin(plr->angle + 0.63) * -1000;
		plr->dest[Y] = cos(plr->angle + 0.63) * -1000;
	}
}

//void raycast()

void gameloop (t_gamecontext *gc)
{
	SDL_Event	event;
	int			scan_h;
	int32_t		ray_d[2];
	float		scan_angle;
	int			wallheights[512];

	ft_bzero(gc->player.sp_pos, sizeof(int32_t[2]));
	gc->player.pos[X] = 3;
	gc->player.pos[Y] = 6;
	gc->player.angle = 0;
	while (1)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
				gc->player.pos[X] = 3;
				gc->player.pos[Y] = 6;
			}
			if (event.type == SDL_KEYDOWN && ismovement(event.key.keysym.sym))
				playerinput(event.key.keysym.sym, &gc->player);
			if (event.type == SDL_KEYUP)
			{
				if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
					gc->player.rot = 0;
				if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
					ft_bzero(gc->player.dest, sizeof(Sint8[2]));
			}
			if (event.type == SDL_QUIT) return ;
		}
		moveplayer(&gc->player,gc->clock.delta);
		update_deltatime(&gc->clock);
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 0 , 255);
		SDL_RenderClear(gc->sdlcontext->renderer);
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 255, 0, 0, 255);
		rendergame(*gc->sdlcontext, wallheights, 512);
		render2Dmap(*gc->sdlcontext);
		scan_h = 0;
		scan_angle = gc->player.angle;
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 255, 3);
		while (scan_h < 512)
		{
			scan_angle += 0.0025; //512 * 0.005 is under 1 rad
			ray_d[X] = sin(scan_angle) * RAY_LENGTH;
			ray_d[Y] = cos(scan_angle) * RAY_LENGTH;
			ray_d[X] += gc->player.pos[X];
			ray_d[Y] += gc->player.pos[Y];
			wallheights[scan_h] = raycast_len(gc->player.pos, ray_d, *gc->sdlcontext);
			scan_h++;
		}
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 255, 0, 0, 255);
		DrawLineWithRenderScale(gc->sdlcontext->renderer, gc->player.pos, gc->player.dest);
		
		SDL_RenderPresent(gc->sdlcontext->renderer);


	}
	
}