/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/10 03:17:02 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"
#define MAP_W 8
#define MAP_H 8
#define RAY_LENGTH	1500
#define WALLSCALE	2
#define RAY_SCALE	10
#define RENDERSCALE	20
#define TURNSPEED	0.2f


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


int	raycast_len(int *crd, int *dst, t_sdlcontext sdl) //TODO use int[2] as params?
{
	int			lcrd[2];
	int			len;
	t_bresenham	b;

	ft_memcpy(lcrd, crd, sizeof(int[2]));
	lcrd[X] *= RAY_SCALE;
	lcrd[Y] *= RAY_SCALE;
	//dst[X] *= RAY_SCALE;
	//dst[Y] *= RAY_SCALE;
	ft_bzero(&b, sizeof(t_bresenham));
	populate_bresenham(&b, lcrd, dst);
	len = 0;
	//step until we got past map length, then track how many steps the bresenham did
	while (b.local[X] > 0 && b.local[X] < MAP_W * RAY_SCALE
			&& b.local[Y] > 0 && b.local[Y] < MAP_H * RAY_SCALE)
	{
		step_bresenham(&b);
		SDL_RenderDrawPoint(sdl.renderer, b.local[X] * 2, b.local[Y] * 2);
		//SDL_RenderPresent(sdl.renderer);
		//usleep(2000);
		len++;
	}
	//printf("ray length %i \n", len);
	len = sqrt(((crd[X] * RAY_SCALE) - b.local[X]) * ((crd[X] * RAY_SCALE) - b.local[X]) + 
				((crd[Y] * RAY_SCALE) - b.local[Y]) * ((crd[Y] * RAY_SCALE) - b.local[Y]));
	
	//exit(0);
	return len;
}

int		calc_wallheight(int wall)
{
	return (((RAY_LENGTH * RAY_SCALE) - wall) * WALLSCALE);
}

void	rendergame(t_sdlcontext sdl, int *walls, int max)
{
	int	i;
	int offset = 300;
	int wallheight;

	i = 0;
	while (i < max)
	{
		wallheight = (MAP_H * RAY_SCALE) - (walls[i]);
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

Uint32	*get_staticmap ()
{
	static Uint32 map[8][8] =
	{
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}
	};
}

int	ismovement(SDL_KeyCode code)
{
	return (code == SDLK_LEFT || code == SDLK_RIGHT || code == SDLK_UP || code == SDLK_DOWN) ||
			(code == SDLK_a || code == SDLK_d);
}

void gameloop (t_gamecontext *gc)
{
	SDL_Event	event;
	int			scan_h;
	int			ray_d[2];
	float		scan_angle;
	int			wallheights[512];

	gc->player.pos[X] = 3;
	gc->player.pos[Y] = 6;
	gc->player.angle = 0;
	while (1)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_KEYDOWN && ismovement(event.key.keysym.sym))
			{
				gc->player.pos[X] -= (event.key.keysym.sym == SDLK_LEFT);
				gc->player.pos[X] += (event.key.keysym.sym == SDLK_RIGHT);
				gc->player.pos[Y] -= (event.key.keysym.sym == SDLK_UP);
				gc->player.pos[Y] += (event.key.keysym.sym == SDLK_DOWN);
				gc->player.angle += (event.key.keysym.sym == SDLK_d) * TURNSPEED;
				gc->player.angle -= (event.key.keysym.sym == SDLK_a) * TURNSPEED;
				SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 0 , 255);
				SDL_RenderClear(gc->sdlcontext->renderer);
				SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 255, 0, 0, 255);
				render2Dmap(*gc->sdlcontext);
				scan_h = 0;
				scan_angle = gc->player.angle;
				SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 255, 20);
				while (scan_h < 512)
				{
					scan_angle += 0.0025; //512 * 0.005 is under 1 rad
					//OFFSET RAY ORIGIN BY SCAN_H
					ray_d[X] = sin(scan_angle) * RAY_LENGTH * RAY_SCALE;
					ray_d[Y] = cos(scan_angle) * RAY_LENGTH * RAY_SCALE;
					ray_d[X] += gc->player.pos[X] * RAY_SCALE;
					ray_d[Y] += gc->player.pos[Y] * RAY_SCALE;
					
					wallheights[scan_h] = raycast_len(gc->player.pos, ray_d, *gc->sdlcontext);
					//printf("scan h %i \n", scan_h);
					//rendergame(*gc->sdlcontext, wallheights, scan_h);
					//SDL_RenderPresent(gc->sdlcontext->renderer);
					/*SDL_RenderDrawPoint(gc->sdlcontext->renderer,
										ray_d[X] + (gc->player.pos[X] * RENDERSCALE),
										ray_d[Y] + (gc->player.pos[X] * RENDERSCALE));*/
					
					/*SDL_RenderDrawLine(gc->sdlcontext->renderer,
									gc->player.pos[X] * RENDERSCALE,
									gc->player.pos[Y] * RENDERSCALE,
									(gc->player.pos[X] + ray_d[X]) * RENDERSCALE,
									(gc->player.pos[Y] + ray_d[Y]) * RENDERSCALE);*/
									
					
					scan_h++;
				}
				printf("end of scan \n");
				SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 255, 0, 0, 255);
				rendergame(*gc->sdlcontext, wallheights, 512);
				SDL_RenderPresent(gc->sdlcontext->renderer);
			}
			if (event.type == SDL_QUIT) return ;
		}
	}
	
}