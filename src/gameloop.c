/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/01 01:05:25 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
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
	static t_bresenham	b;
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
		/*if (gc->map[(b.local[Y] / GAMESCALE) * MAPSIZE + (b.local[X] / GAMESCALE)] != 0)
		{
			gc->tex_x = (b.local[Y] / GAMESCALE) - b.local[Y];
			break;
		}
		step_bresenham_y(&b);*/
		if (gc->map[(b.local[Y] / GAMESCALE) * MAPSIZE + (b.local[X] / GAMESCALE)] != 0)
		{
			gc->tex_x = (b.local[Y] / GAMESCALE) - b.local[Y];
			break;
		}
	}
	gc->tex_x = b.local[X] % GAMESCALE;
	//assert(gc->tex_x <= GAMESCALE && gc->tex_x >= 0);
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
		if ((walls[i] & 0xFFFF) != 0)
			wallheight = (50 * 512) / (walls[i] & 0xFFFF);
		//wallheight = 512 - (((walls[i] & 0xFFFF) * 1.41f) / 64); //Figure out what is the correlation between euc distance and width of texture? to get correct relative height
		float iy = 0.0f;
		float ty = 0.0f;
		int ix = ((float)(walls[i] >> 16) / 64.0f) * sdl.images[1].size[X];
		//assert(walls[i] >> 16 >= 0 && walls[i] >> 16 <= GAMESCALE);
		float ystep = ((float)sdl.images[1].size[Y] / (float)wallheight);
		while (iy < wallheight - 1 && wallheight != 0)
		{
			Uint32 clr = samplecolor(sdl.images[1], ix + (iy * ystep), (int)(iy * ystep));
			float mul = ft_clampf((float)((float)wallheight / (float)128), 0, 1.0f);
			int r = (clr & 0xFF) * mul * mul;
			int g = (clr >> 8 & 0xFF) * mul;
			int b = (clr >> 16 & 0xFF) * mul;
			SDL_SetRenderDrawColor(sdl.renderer, r, g, b, 255);
			SDL_RenderDrawPoint(sdl.renderer, i + 100, 300 + (iy - (wallheight / 2)));
			//index += sdl.images->size[X];
			iy++;
			/*if (index > sdl.images->size[X])
				index = index + (index % sdl.images->size[X]);*/
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

void floorcast(t_sdlcontext sdl, float pos[2], float ang[2])
{
	float	step[2];
	int		x;
	int		y;
	int		p;
	float	row;

	y = 0;
	
	while (y < 512)
	{
		x = 0;
		p = y - 256; //TODO: unhardcode
		if (p == 0) p += 1;
		row = 256.0f / (float)p;
		step[X] = row / 512.0f;
		step[Y] = row / 512.0f;
		float floor[2];
		floor[X] = pos[X] / GAMESCALE + (row * cos(ang[0]));
		floor[Y] = pos[Y] / GAMESCALE + (row * sin(ang[0]));
		while (x < 512)
		{
			int t[2];
			t[X] = (int)(sdl.images->size[X] * (floor[X] - floorf(floor[X])));
			t[Y] = (int)(sdl.images->size[X] * (floor[Y] - floorf(floor[Y])));
			floor[X] += step[X];
			floor[Y] += step[Y];
			int clr = samplecolor(sdl.images[1], t[X], t[Y] *  sdl.images->size[X]);
			SDL_SetRenderDrawColor(sdl.renderer, clr & 0xFF, clr >> 8 & 0xFF, clr >> 16 & 0xFF, 255);
			SDL_RenderDrawPoint(sdl.renderer, x + 100 , y + 100);
			x++;
		}
		y++;
	}

}

int *raycast(float playerpos[2], float angle, t_sdlcontext *sdl, t_gamecontext gc) //TODO: remove sdl context, only used for debug?
{
	static int	wallheights[512];
	int					scan_h;
	int32_t				ray_d[2];
	float				scan_angle;

	scan_h = 0;
	scan_angle = angle + 1.57;
	//floorcast(*sdl, gc.player.pos, (float [2]){scan_angle, scan_angle - (512 * 0.0022)}); //TODO make fov define
	SDL_SetRenderDrawColor(sdl->renderer, 20, 20, 255, 120);
	while (scan_h < 512)
	{
		scan_angle -= 0.0022; //512 * 0.005 is under 1 rad
		ray_d[X] = sin(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[Y] = cos(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[X] += (int)playerpos[X];
		ray_d[Y] += (int)playerpos[Y];
		//wallheights[scan_h] = raycast_len((int[2]){playerpos[X] + ((255 - scan_h) / 20 * cos(scan_angle)), playerpos[Y] - ((255 - scan_h) / 20 * sin(scan_angle))}, ray_d, &gc);
		wallheights[scan_h] = raycast_len((int[2]){playerpos[X], playerpos[Y]}, ray_d, &gc);
		wallheights[scan_h] += (gc.tex_x << 16);
		//int tex_x = ((ray_d[X] - playerpos[X])) / GAMESCALE;
		/*if (scan_h < 30)
			printf("%i tex %i \n", scan_h, gc.tex_x);*/
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
		moveplayer(&gc->player, gc->clock.delta, gc->map);
		update_deltatime(&gc->clock);
		render2Dmap(*gc->sdlcontext, gc->map);
		rendergame(*gc->sdlcontext,
			raycast(gc->player.pos, gc->player.angle, gc->sdlcontext, *gc),
			512);
		SDL_RenderPresent(gc->sdlcontext->renderer);
	}
}
