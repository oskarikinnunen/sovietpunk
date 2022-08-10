/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/09 20:39:17 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
#include "SP_PNG.h"
#include "SP_OBJ.h"

//TODO: draw text, update map state, whatever youll figure it out

void	drawrect(SDL_Renderer *r, int *crd)
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
	//SDL_RenderPresent(r);
}

Uint64	coloratpoint(t_pngdata png, int ix, int iy)
{
	int	index;
	
	index = ix + ((iy * png.size[Y]));
	if (png.data[index] > png.palette.length || png.data[index] < 0)
		printf("weird index value %i \n", png.data[index]);
	/*if (ix == 159 && iy == 1)
		printf("second line last pixel index = %i", index);
	if (ix == 0 && iy == 2)
		printf("NEXT line last pixel index = %i", index);*/
	//printf("%i ", png.data[index]);
	return (png.palette.plte[png.data[index]]);
}

void	drawtexture(t_sdlcontext context, int x, int y)
{
	int	iy;
	int ix;

	iy = 0;
	ix = 0;
	printf("drawing texture with size %i %i\n", context.textures[0].size[X], context.textures[0].size[Y]);
	while (++iy < context.textures[0].size[Y] - 1)
	{
		while (ix < context.textures[0].size[X])
		{
			int color = coloratpoint(context.textures[0], ix + iy, iy);
			
			SDL_SetRenderDrawColor(context.renderer,
				color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, 0xFF);
			SDL_RenderDrawPoint(context.renderer, ix + x, iy + y);
			ix++;
		}
		//printf("\n", ix);
		ix = 0;
	}
}

void	drawmapstate(t_sdlcontext	context, t_mapeddata data)
{
	int	gridsize = 10;
	int	crd[2];

	ft_bzero(crd, sizeof(int [2]));
	
	while (crd[Y] < gridsize * TILESIZE)
	{
		while (crd[X] < gridsize * TILESIZE)
		{
			SDL_SetRenderDrawColor(context.renderer, 0, 255, 0, 255);
			if (crd[X] / TILESIZE == data.cursor[X] && crd[Y] / TILESIZE == data.cursor[Y])
				SDL_SetRenderDrawColor(context.renderer, 255, 0, 0, 255);
			drawrect(context.renderer, crd);
			crd[X] += TILESIZE;
		}
		crd[X] = 0;
		crd[Y] += TILESIZE;
	}
}

void	mapcreator_eventloop(int fd, t_sdlcontext context)
{
	SDL_Event	event;
	t_mapeddata	data;

	//SDL_SetRenderDrawColor(context.renderer, UINT8_MAX, 0, 0, UINT8_MAX);
	ft_bzero(data.cursor, sizeof(int[2]));
	while (1)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_KEYDOWN)
			{
				//SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 255);
				SDL_RenderClear(context.renderer);
				drawtexture(context, 10, 10);
				
				printf("key: %i\n", event.key.keysym.sym);
				context.objects[0].view[X] -= (event.key.keysym.sym == SDLK_LEFT) * 10;
				context.objects[0].view[X] += (event.key.keysym.sym == SDLK_RIGHT) * 10;
				drawimage(context, 50, 50);
				fdf_update(&context.objects[0]);
				
				data.cursor[X] -= (event.key.keysym.sym == SDLK_LEFT);
				data.cursor[X] += (event.key.keysym.sym == SDLK_RIGHT);
				data.cursor[Y] -= (event.key.keysym.sym == SDLK_UP);
				data.cursor[Y] += (event.key.keysym.sym == SDLK_DOWN);
				drawmapstate(context, data);
				SDL_RenderPresent(context.renderer);
				//SDL_UpdateWindowSurface(context.window);
			}
				
			if (event.type == SDL_QUIT)
				return ;
		}
	}
	return ;
}

void	mapcreator(char *mapname, t_sdlcontext context) //Maybe just make this a standalone program?
{
	int			fd;
	t_pngdata	png;
	t_simpleimg	img;
	t_fdf		fdf;
	t_obj		obj;

	fd = open(mapname, O_CREAT | O_RDWR); //USE protect function (sp_open)
	pngparse(&png);
	close(fd);
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
}