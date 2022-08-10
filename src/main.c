/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:05:51 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/10 02:50:28 by okinnune         ###   ########.fr       */
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

int	SDL_Loop(t_sdlcontext context)
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

int	main(int argc, char **args)
{
	t_gamecontext	gcontext;
	t_sdlcontext	context;
	ft_bzero(&context, sizeof(t_sdlcontext));
	ft_bzero(&gcontext, sizeof(t_gamecontext));
	
	gcontext.sdlcontext = &context;
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_EVENTS) < 0)
		printf("SDL_Init error");
	context.renderer = NULL;
	context.window = SDL_CreateWindow("SovietPunk 1947",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	context.renderer = SDL_CreateRenderer(context.window, -1, 0); //Read documentation to find out if surface is created aswell!
	if (context.renderer == NULL)
		printf("Still null? \n"); //Error exit
	if (context.window == NULL)
		printf("Couldn't create SDL2 window :("); //error exit
	printf("sdl context and init \n");
	context.surface = SDL_GetWindowSurface(context.window);
	SDL_UpdateWindowSurface(context.window);
	//mapcreator("newmap", context);
	gameloop(&gcontext);


	SDL_DestroyWindow(context.window);
	SDL_Quit();
	return (0);
}