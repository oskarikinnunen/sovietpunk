/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameloop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/09 20:25:33 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/13 18:35:34 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	floorcast(int16_t *floor, t_bresenham b, t_gamecontext *gc)
{
	int			left;
	int			i;
	int			ind;
	int16_t		res;

	left = CENTER_Y;
	i = 10;
	while (i < left / 2)
	{
		ind = i * 2;
		while (sqrt(
				(b.local[X] - b.target[X]) * (b.local[X] - b.target[X])
				+ (b.local[Y] - b.target[Y]) * (b.local[Y] - b.target[Y])
			) > gc->sdl.ft[ind])
		{
			if (step_bresenham(&b))
				break ;
		}
		res = ((b.local[Y] % GAMESCALE) & 63)
			+ (((b.local[X] % GAMESCALE) & 63) << 8);
		floor[ind] = res;
		floor[ind + 1] = res;
		i++;
	}
}

int	getraytex(uint32_t sample, int pos[2])
{
	int		modx;
	uint8_t	walldecider;

	modx = pos[X] % GAMESCALE;
	walldecider = NORTHWALL;
	if (modx == 0)
		walldecider = WESTWALL;
	else if (modx == GAMESCALE - 1)
		walldecider = EASTWALL;
	if (pos[Y] % GAMESCALE == GAMESCALE - 1)
		walldecider = SOUTHWALL;
	return (getindexedwall(sample, walldecider));
}

int	raycast_len(int crd[2], int dst[2], t_gamecontext *gc, int16_t *floor)
{
	static t_bresenham	b;
	int					tex_sample;
	int					res;
	int					tx;
	int					end;

	populate_bresenham(&b, crd, dst);
	while (b.local[X] > 0 && b.local[X] < MAPSIZE * GAMESCALE - 1
		&& b.local[Y] > 0 && b.local[Y] < MAPSIZE * GAMESCALE - 1
		&& !step_bresenham(&b) && gc->map[((b.local[Y] & 0xFFFFFFC0) >> 6)
			* MAPSIZE + ((b.local[X] & 0xFFFFFFC0) >> 6)] == 0)
		;
	end = gc->map[((b.local[Y] & 0xFFFFFFC0) >> 6)
		* MAPSIZE + ((b.local[X] & 0xFFFFFFC0) >> 6)];
	tex_sample = b.local[X] % GAMESCALE;
	if (b.local[X] % GAMESCALE == 0
		|| b.local[X] % GAMESCALE == GAMESCALE - 1)
		tex_sample = b.local[Y] % GAMESCALE;
	res = WALLTHING / v2dist(crd, b.local);
	tx = getraytex(end, b.local);
	tx = tx << 24;
	populate_bresenham(&b, b.local, crd);
	floorcast(floor, b, gc);
	res += ((tex_sample & 0xFF) << 16) + tx;
	return (res);
}

int	*raycast(float playerpos[2], float angle,
	t_sdlcontext *sdl, t_gamecontext gc)
{
	static int			wallheights[WINDOW_W];
	static int16_t		floor_tex[WINDOW_W];
	int					scan_h;
	int					ray_d[2];
	float				scan_angle;

	scan_h = 0;
	scan_angle = angle + FOV;
	while (scan_h < WINDOW_W)
	{
		ray_d[X] = sin(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[Y] = cos(scan_angle) * RAY_LENGTH * GAMESCALE;
		ray_d[X] += (int)playerpos[X];
		ray_d[Y] += (int)playerpos[Y];
		wallheights[scan_h] = raycast_len((int [2])
			{playerpos[X], playerpos[Y]}, ray_d, &gc, floor_tex);
		render_floor(*sdl, floor_tex,
			scan_h, wallheights[scan_h] & 0xFFFF);
		scan_angle -= RAYSLICE;
		scan_h++;
	}
	return (wallheights);
}

void	gameloop(t_gamecontext gc)
{
	int	*walls;
	int	mapcrd[2];

	openmap(&gc, gc.map);
	spawnplayer(&gc);
	alloc_image(&gc.perfgraph.image, PERFGRAPH_SAMPLES, 64);
	while (1)
	{
		update_deltatime(&gc.clock);
		drawperfgraph(&gc.perfgraph, gc.clock.delta);
		if (eventloop(&gc))
			break ;
		moveplayer(&gc);
		walls = raycast(gc.player.pos, gc.player.angle, &gc.sdl, gc);
		rendergame(&gc.sdl, walls, &gc, -1);
		renderobj(&gc, gc.sdl.fdfs);
		drawfdf(&gc.sdl, gc.sdl.fdfs[0], walls);
		rendertopdownmap(&gc);
		put_image_to_screen(&gc.sdl, gc.perfgraph.image,
			(int [2]){WINDOW_W - 64, WINDOW_H - 64});
		SDL_UpdateWindowSurface(gc.sdl.window);
	}
	exit(0);
}
