/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/06 16:46:18 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
#include "SP_PNG.h"
#include "SP_OBJ.h"

//TODO: draw text, update map state, whatever youll figure it out

void	drawrect(SDL_Renderer *r, int crd[2])
{
	int	i;

	i = 0;
	while (i < TILESIZE)
	{
		SDL_RenderDrawPoint(r, crd[X] + i, crd[Y]);
		SDL_RenderDrawPoint(r, crd[X], crd[Y] + i);
		SDL_RenderDrawPoint(r, crd[X] + i, crd[Y] + TILESIZE);
		SDL_RenderDrawPoint(r, crd[X] + TILESIZE, crd[Y] + i);
		i++;
	}
}

int		isnonempty(u_int32_t *mapdata, int *crd)
{
	int local[2];
}

void	drawmapstate(t_sdlcontext	context, t_mapeddata ed) //TODO: CONVERT TO PIXEL DRAW
{
	int	crd[2];

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < MAPSIZE)
	{
		while (crd[X] < MAPSIZE)
		{
			SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 101);
			if (crd[X] == ed.cursor[X] && crd[Y] == ed.cursor[Y])
				SDL_SetRenderDrawColor(context.renderer, 255, 0, 0, 255);
			else if (ed.mapdata[crd[X] + crd[Y] * TILESIZE] != 0)
				drawimagescaled(&context,
					(int [2]){crd[X] + crd[X] * TILESIZE,
					crd[Y] + crd[Y] * TILESIZE},
					ed.mapdata[crd[X] + crd[Y] * TILESIZE] - 1, //Make its own function? with clamp or somsom
					TILESIZE);
			drawrect(context.renderer,
				(int[2]){crd[X] + crd[X] * TILESIZE, crd[Y] + crd[Y] * TILESIZE});
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
		SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, 255);
		SDL_RenderClear(sdl.renderer);
		if (ed_eventloop(&ed))
			break ;
		drawmapstate(sdl, ed); //draw map state
		//drawimage(sdl, 40, 40);
		SDL_RenderPresent(sdl.renderer);
	}
	printf("wrote %lu b in file\n", write(fd, ed.mapdata, sizeof(u_int32_t) * MAPSIZE * MAPSIZE));
	close(fd);
}

/*void	mapcreator(char *mapname, t_sdlcontext context) //Maybe just make this a standalone program?
{
	int			fd;
	t_pngdata	png;
	t_simpleimg	img;
	t_fdf		fdf;
	t_obj		obj;

	//fd = open(mapname, O_CREAT | O_RDWR); //USE protect function (sp_open)
	pngparse(&png);
	//close(fd);
	context.textures = &png;
	ft_memcpy(img.size, png.size, sizeof(Uint32[2]));
	img.length = png.size[X] * png.size[Y] * sizeof(Uint32);
	img.data = ft_memalloc(img.length * sizeof(Uint32));
	ft_memset(img.data, 255, img.length);
	
	parse_obj(&obj);
	fdf_init(&fdf, &img, &obj); //TODO: call fdf in mapcreator loop
	fdf_update(&fdf);
	context.images = &img;
	context.objects = &fdf;
	mapcreator_eventloop(fd, context);
}*/