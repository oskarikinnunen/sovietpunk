/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:05:51 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/20 05:19:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

Uint64	deltatime()
{
	static Uint64	prevtime = 0;
	static Uint64	time = 0;
	
	
	prevtime = time;
	//time = SDL_GetTicks64();
	while (SDL_GetTicks64() == prevtime)
		;
	time = SDL_GetTicks64();
	return (time - prevtime);
}

int	SDL_Loop(t_SDL_Context context)
{
	SDL_Event	event;
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_KEYDOWN)
		{
			//printf("keydown event \n");
			SDL_Delay(7);
		}
			
		if (event.type == SDL_QUIT)
			return (0);
	}
	return (1);
}

void	GameLoop(t_SDL_Context context)
{
	//loadmap();
	while (1)
	{
		printf("deltatime %li \n", deltatime());
		if (!SDL_Loop(context))
			break ;
		
	}
}


int	main(int argc, char **args)
{
	t_SDL_Context	context;
	ft_bzero(&context, sizeof(t_SDL_Context));
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_EVENTS) < 0)
		printf("SDL_Init error");
	context.window = SDL_CreateWindow("SovietPunk 1947", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	context.renderer = SDL_CreateRenderer(context.window, -1, 0);
	if (context.window == NULL)
		printf("Couldn't create SDL2 window :(");
	
	context.surface = SDL_GetWindowSurface(context.window);
	SDL_FillRect(context.surface, NULL, SDL_MapRGB(context.surface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(context.window);
	//SDL_Loop(context);
	//GameLoop(context);
	mapcreator("newmap", context);
	SDL_DestroyWindow(context.window);
	SDL_Quit();
	return (0);
}