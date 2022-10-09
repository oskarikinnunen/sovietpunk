/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 01:12:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
#include "SP_PNG.h"
#include "SP_OBJ.h"

void	drawmapstate(t_sdlcontext	context, t_mapeddata ed)
{
	int			clr;
	uint32_t	tile;
	int			crd[2];
	int			scr_crd[2];

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < MAPSIZE)
	{
		while (crd[X] < MAPSIZE)
		{
			v2cpy(scr_crd, crd);
			v2mul(scr_crd, TILESIZE);
			v2add(scr_crd, crd);
			clr = CLR_GRAY;
			if (crd[X] == ed.cursor[X] && crd[Y] == ed.cursor[Y])
				clr = CLR_PRPL;
			tile = samplemap(ed.mapdata, crd);
			drawquadtile(&context, scr_crd, tile, TILESIZE);
			drawrect(context.surface->pixels, scr_crd, clr, TILESIZE);
			crd[X]++;
		}
		crd[X] = 0;
		crd[Y]++;
	}
}

void	mapcreator(char *mapname, t_gamecontext gc)
{
	int				fd;
	t_mapeddata		ed;

	ft_bzero(&ed, sizeof(t_mapeddata));
	fd = open(mapname, O_RDWR);
	if (fd < 0)
		exit(0);
	(void)!read(fd, ed.mapdata, sizeof(u_int32_t [MAPSIZE * MAPSIZE]));
	close(fd);
	while (!ed_eventloop(&ed))
	{
		draw_buttons(&gc.sdl, ed);
		drawmapstate(gc.sdl, ed);
		drawquadtile(&gc.sdl,
			(int [2]){WINDOW_W - 128, WINDOW_H / 2}, ed.cursoritem, TILESIZE2);
		SDL_UpdateWindowSurface(gc.sdl.window);
	}
	fd = open(mapname, O_TRUNC | O_RDWR);
	if (fd >= 0)
	{
		(void)!write(fd, ed.mapdata, sizeof(u_int32_t) * MAPSIZE * MAPSIZE);
		close(fd);
	}
}
