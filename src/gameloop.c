/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/30 04:25:48 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"
#define RAY_LENGTH	200
#define WALLSCALE	2
#define GAMESCALE	40
#define RENDERSCALE	20

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
	float f = ((float)RENDERSCALE / (float)GAMESCALE);
	SDL_RenderDrawLine(r, origin[X] * f, origin[Y] * f,
							dest[X] * f, dest[Y] * f);
}

//TODO: implement dda
int	raycast_len(int crd[2], int dst[2], t_gamecontext *gc) //TODO use int[2] as params?
{
	t_bresenham	b;
	int			l_crd[2];
	float		d_dst[2]; //DISTANCE, not destination in this context, TODO: rename

	//ft_bzero(l_crd, sizeof(int [2]));
	/*ft_bzero(d_dst, sizeof(float [2]));

	
	d_dst[X] = 1 / dst[X];*/

	
	ft_bzero(&b, sizeof(t_bresenham));
	populate_bresenham(&b, crd, dst);
	while (b.local[X] > 0 && b.local[X] < MAPSIZE * GAMESCALE
			&& b.local[Y] > 0 && b.local[Y] < MAPSIZE * GAMESCALE)
	{
		step_bresenham(&b);
		if (gc->map[(b.local[Y] / GAMESCALE) * MAPSIZE + (b.local[X] / GAMESCALE)] != 0)
		{
			//store lastwall index to access texture
			//calculate stripindex fr
			gc->lastwall = (b.local[Y] / GAMESCALE) * MAPSIZE + (b.local[X] / GAMESCALE);
			gc->tex_x = (b.local[Y] / GAMESCALE) - b.local[Y];
			// calculate relative location from current tile
			break;
		}
	}
	gc->tex_x = b.local[X] % GAMESCALE;
	assert(gc->tex_x <= GAMESCALE && gc->tex_x >= 0);
	if (b.local[X] % GAMESCALE == 0 || b.local[X] % GAMESCALE == GAMESCALE - 1) {
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 122, 0, 122, 255);
		gc->tex_x = b.local[Y] % GAMESCALE;
	}
	else
		SDL_SetRenderDrawColor(gc->sdlcontext->renderer, 0, 0, 255, 255);
	DrawLineWithRenderScale(gc->sdlcontext->renderer, crd, b.local);
	return (v2dist(crd, b.local));
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
		//if (walls[i] < MAPSIZE * GAMESCALE)
		wallheight = (MAPSIZE * GAMESCALE) - (walls[i] & 0xFFFF);
		wallheight /= 4;
		int ii = 0;
		int index = ((float)(walls[i] >> 16) / 40.0f) * sdl.images[1].size[X];
		assert(walls[i] >> 16 >= 0 && walls[i] >> 16 <= GAMESCALE);
		float ystep = (sdl.images->size[X] / wallheight);
		while (ii < wallheight)
		{
			int r = (sdl.images[1].data[index] & 0xFF);
			int g = (sdl.images[1].data[index] >> 8 & 0xFF);
			int b = (sdl.images[1].data[index] >> 16 & 0xFF);
			SDL_SetRenderDrawColor(sdl.renderer, r, g, b, 255);
			SDL_RenderDrawPoint(sdl.renderer, i + offset, ii + offset);
			//index += sdl.images->size[X];
			index += sdl.images->size[X] * ystep; // Use sampleimage instead plz, will fix it prolly
			/*if (index > sdl.images->size[X])
				index = index + (index % sdl.images->size[X]);*/
			ii++;
		}
		
		/*SDL_RenderDrawLine(sdl.renderer,
			i + offset, offset - wallheight,
			i + offset, offset + wallheight);*/
		i++;
	}
}

//int	deltatime

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
	scan_angle = angle + 1.57;
	SDL_SetRenderDrawColor(sdl->renderer, 20, 20, 255, 120);
	while (scan_h < 512)
	{
		scan_angle -= 0.0025; //512 * 0.005 is under 1 rad
		ray_d[X] = sin(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[Y] = cos(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[X] += (int)playerpos[X];
		ray_d[Y] += (int)playerpos[Y];
		wallheights[scan_h] = raycast_len((int[2]){playerpos[X], playerpos[Y]}, ray_d, &gc);
		wallheights[scan_h] += (gc.tex_x << 16);
		//int tex_x = ((ray_d[X] - playerpos[X])) / GAMESCALE;
		if (scan_h < 30)
			printf("%i tex %i \n", scan_h, gc.tex_x);
		//use lastwall index to get current texture, 
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
