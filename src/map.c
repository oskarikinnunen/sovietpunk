/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 10:01:47 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/08 12:42:41 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"

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
	SDL_RenderPresent(r);
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
}


void	mapcreator_eventloop(int fd, t_SDL_Context context)
{
	SDL_Event	event;
	t_mapeddata	data;

	SDL_SetRenderDrawColor(context.renderer, UINT8_MAX, 0, 0, UINT8_MAX);
	ft_bzero(data.cursor, sizeof(int[2]));
	while (1)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_KEYDOWN)
			{
				drawmapstate(context);
				printf("key: %i\n", event.key.keysym.sym);
				data.cursor[X] -= (event.key.keysym.sym == SDLK_LEFT);
				data.cursor[X] += (event.key.keysym.sym == SDLK_RIGHT);
				data.cursor[Y] -= (event.key.keysym.sym == SDLK_UP);
				data.cursor[Y] += (event.key.keysym.sym == SDLK_DOWN);
				//SDL_GetKeyboardState()
				printf("x = %i, y = %i \n", data.cursor[X], data.cursor[Y]);
				SDL_RenderDrawPoint(context.renderer, data.cursor[X], data.cursor[Y]);
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
	int	fd;

	fd = open(mapname, O_CREAT | O_RDWR); //Make protect function
	pngparse();
	mapcreator_eventloop(fd, context);
}