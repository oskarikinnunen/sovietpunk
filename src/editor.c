/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 16:28:39 by okinnune         ###   ########.fr       */
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
			clr = CLR_GRAY;
			if (crd[X] == ed.cursor[X] && crd[Y] == ed.cursor[Y])
				clr = CLR_PRPL;
			//printf("pos %i %i \n", crd[X], crd[Y]);
			tile = samplemap(ed.mapdata, crd);
			//drawimagescaled(&context, scr_crd, tile, TILESIZE);
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
	int				selector_pos[2];
	t_sdlcontext	sdl;
	//t_editorcontext? Has textures in it and stuff?

	sdl = gc.sdl;
	ft_bzero(&ed, sizeof(t_mapeddata));
	ed.wall_select = -1;
	fd = open(mapname, O_RDWR);
	if (fd < 0)
		exit(0);
	printf("read %lu from file \n", read(fd, ed.mapdata, sizeof(u_int32_t [MAPSIZE * MAPSIZE])));
	close(fd);
	v2cpy(selector_pos, (int [2]){WINDOW_W / 2 + 64, WINDOW_H / 2});
	while (1)
	{
		if (ed_eventloop(&ed))
			break ;
		draw_buttons(&sdl, ed);
		drawmapstate(sdl, ed); //draw map state
		/*if (ed.wall_select == -1)
			drawimagescaled(&sdl, scr_middle, ed.cursoritem, TILESIZE * 2);
		else*/
		drawquadtile(&sdl, selector_pos, ed.cursoritem, TILESIZE2);
		SDL_UpdateWindowSurface(sdl.window);
	}
	fd = open(mapname, O_TRUNC | O_RDWR);
	if (fd >= 0)
	{
		(void)!write(fd, ed.mapdata, sizeof(u_int32_t) * MAPSIZE * MAPSIZE);
		close(fd);
	}
	exit (0);
}


/*
==108817== 
m count 9 
read 1024 from file 
==108817== Invalid write of size 4
==108817==    at 0x10CA6C: placeitem (editor_eventloop.c:55)
==108817==    by 0x10CA6C: ed_eventloop (editor_eventloop.c:78)
==108817==    by 0x109C5B: mapcreator (editor.c:75)
==108817==    by 0x1094EB: main (main.c:80)
==108817==  Address 0x101010101010571 is not stack'd, malloc'd or (recently) free'd
==108817== 
==108817== 
==108817== Process terminating with default action of signal 11 (SIGSEGV)
==108817==  General Protection Fault
==108817==    at 0x10CA6C: placeitem (editor_eventloop.c:55)
==108817==    by 0x10CA6C: ed_eventloop (editor_eventloop.c:78)
==108817==    by 0x109C5B: mapcreator (editor.c:75)
==108817==    by 0x1094EB: main (main.c:80)
==108817== 

*/