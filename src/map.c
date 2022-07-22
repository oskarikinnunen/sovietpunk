/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/20 06:20:50 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
#include "SP_PNG.h"

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
	return (png.palette.plte[png.data[index]]);
}

void	drawtexture(t_SDL_Context context, int x, int y)
{
	int	iy;
	int ix;

	iy = 0;
	ix = 0;
	while (iy < context.textures[0].size[Y])
	{
		while (ix <= context.textures[0].size[X])
		{
			int color = coloratpoint(context.textures[0], ix, iy);
			
			SDL_SetRenderDrawColor(context.renderer,
				color & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, 0xFF);
			SDL_RenderDrawPoint(context.renderer, x + ix, y + iy);
			ix++;
		}
		ix = 0;
		iy++;
	}
	printf("final pixel %i %i \n", ix, iy);
	SDL_RenderPresent(context.renderer);
}

void	drawmapstate(t_SDL_Context	context)
{
	int	gridsize = 10;
	int	crd[2];

	ft_bzero(crd, sizeof(int [2]));
	while (crd[Y] < gridsize * TILESIZE)
	{
		while (crd[X] < gridsize * TILESIZE)
		{
			drawrect(context.renderer, crd);
			//draw rectangle
			crd[X] += TILESIZE;
		}
		crd[X] = 0;
		crd[Y] += TILESIZE;
	}
	drawtexture(context, 100, 100);
}


void	mapcreator_eventloop(int fd, t_SDL_Context context)
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
				SDL_RenderClear(context.renderer);
				//drawmapstate(context);
				drawtexture(context, 10, 10);
				printf("key: %i\n", event.key.keysym.sym);
				data.cursor[X] -= (event.key.keysym.sym == SDLK_LEFT);
				data.cursor[X] += (event.key.keysym.sym == SDLK_RIGHT);
				data.cursor[Y] -= (event.key.keysym.sym == SDLK_UP);
				data.cursor[Y] += (event.key.keysym.sym == SDLK_DOWN);
				//SDL_GetKeyboardState()
				printf("x = %i, y = %i \n", data.cursor[X], data.cursor[Y]);
				//SDL_RenderDrawPoint(context.renderer, data.cursor[X], data.cursor[Y]);
				SDL_RenderPresent(context.renderer);
			}
				
			if (event.type == SDL_QUIT)
				return ;
		}
	}
	return ;
}

void	mapcreator(char *mapname, t_SDL_Context context) //Maybe just make this a standalone program?
{
	int			fd;
	t_pngdata	png;

	fd = open(mapname, O_CREAT | O_RDWR); //Make protect function
	pngparse(&png);
	context.textures = &png;
	mapcreator_eventloop(fd, context);
}