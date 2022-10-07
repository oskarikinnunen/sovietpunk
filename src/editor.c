/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/07 16:07:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
#include "SP_PNG.h"
#include "SP_OBJ.h"

//TODO: draw text, update map state, whatever youll figure it out

/*int		isnonempty(u_int32_t *mapdata, int *crd)
{
	int local[2];
}*/

void	drawmapstate(t_sdlcontext	context, t_mapeddata ed) //TODO: CONVERT TO PIXEL DRAW
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
			clr = INT_MAX;
			if (crd[X] == ed.cursor[X] && crd[Y] == ed.cursor[Y])
				clr = CLR_PRPL;
			tile = samplemap(ed.mapdata, crd);
			drawimagescaled(&context, scr_crd, tile, TILESIZE);
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
	int				scr_middle[2];
	t_sdlcontext	sdl;
	//t_editorcontext? Has textures in it and stuff?

	sdl = gc.sdl;
	ft_bzero(&ed, sizeof(t_mapeddata));
	ed.mapdata = ft_memalloc(sizeof(u_int32_t) * MAPSIZE * MAPSIZE);
	if (ed.mapdata == NULL)
		exit(0);
	fd = sp_fileopen(mapname, O_RDWR);
	printf("read %lu from file \n", read(fd, ed.mapdata, sizeof(u_int32_t) * MAPSIZE * MAPSIZE));
	close(fd);
	fd = sp_fileopen(mapname, O_TRUNC | O_RDWR);
	v2cpy(scr_middle, (int [2]){WINDOW_W / 2, WINDOW_H / 2});
	while (1)
	{
		if (ed_eventloop(&ed))
			break ;
		drawmapstate(sdl, ed); //draw map state
		drawimagescaled(&sdl, scr_middle, ed.cursoritem, TILESIZE * 2);
		SDL_UpdateWindowSurface(sdl.window);
	}
	printf("wrote %lu b in file\n", write(fd, ed.mapdata, sizeof(u_int32_t) * MAPSIZE * MAPSIZE));
	close(fd);
	exit (0);
}
