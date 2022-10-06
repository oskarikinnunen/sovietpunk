/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 14:53:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
#include "SP_PNG.h"
#include "SP_OBJ.h"

//TODO: draw text, update map state, whatever youll figure it out

void	drawrect(uint32_t *pxls, int crd[2], int clr)
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		pxls[crd[X] + i + crd[Y] * WINDOW_W] = clr;
		pxls[crd[X] + (crd[Y] + i) * WINDOW_W] = clr;
		pxls[crd[X] + TILESIZE + (crd[Y] + i) * WINDOW_W] = clr;
		pxls[crd[X] + i + (crd[Y] + TILESIZE) * WINDOW_W] = clr;
		/*SDL_RenderDrawPoint(r, crd[X] + i, crd[Y]);
		SDL_RenderDrawPoint(r, crd[X], crd[Y] + i);
		SDL_RenderDrawPoint(r, crd[X] + i, crd[Y] + TILESIZE);
		SDL_RenderDrawPoint(r, crd[X] + TILESIZE, crd[Y] + i);*/
		i++;
	}
}

/*int		isnonempty(u_int32_t *mapdata, int *crd)
{
	int local[2];
}*/

void	drawmapstate(t_sdlcontext	context, t_mapeddata ed) //TODO: CONVERT TO PIXEL DRAW
{
	int	clr;
	int	crd[2];

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < MAPSIZE)
	{
		while (crd[X] < MAPSIZE)
		{
			clr = INT_MAX;
			if (crd[X] == ed.cursor[X] && crd[Y] == ed.cursor[Y])
				clr = CLR_PRPL;
			else if (ed.mapdata[crd[X] + crd[Y] * TILESIZE] >= 0 && ed.mapdata[crd[X] + crd[Y] * TILESIZE] <= 2 )
				drawimagescaled(&context,
					(int [2]){crd[X] + crd[X] * TILESIZE,
					crd[Y] + crd[Y] * TILESIZE},
					ed.mapdata[crd[X] + crd[Y] * TILESIZE], //Make its own function? with clamp or somsom
					TILESIZE);
			drawrect((uint32_t *)context.surface->pixels,
				(int[2]){crd[X] + crd[X] * TILESIZE, crd[Y] + crd[Y] * TILESIZE},
				clr);
			crd[X]++;
		}
		crd[X] = 0;
		crd[Y]++;
	}
}

void	mapcreator(char *mapname, t_sdlcontext sdl)
{
	int			fd;
	t_mapeddata	ed;
	//t_editorcontext? Has textures in it and stuff?

	ft_bzero(&ed, sizeof(t_mapeddata));
	ed.mapdata = ft_memalloc(sizeof(u_int32_t) * MAPSIZE * MAPSIZE);
	if (ed.mapdata == NULL)
		error_exit("couldn't allocate mapdata");
	fd = sp_fileopen(mapname, O_RDWR);
	printf("read %lu from file \n", read(fd, ed.mapdata, sizeof(u_int32_t) * MAPSIZE * MAPSIZE));
	close(fd);
	fd = sp_fileopen(mapname, O_TRUNC | O_RDWR);
	//loadpngs(&sdl);
	while (1)
	{
		if (ed_eventloop(&ed))
			break ;
		drawmapstate(sdl, ed); //draw map state
		//drawimage(sdl, 40, 40);
		//SDL_RenderPresent(sdl.renderer);
		SDL_UpdateWindowSurface(sdl.window);
	}
	printf("wrote %lu b in file\n", write(fd, ed.mapdata, sizeof(u_int32_t) * MAPSIZE * MAPSIZE));
	close(fd);
}
