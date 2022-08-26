/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:05:51 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/26 02:41:51 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	createsdlcontext(t_sdlcontext *context)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_EVENTS) < 0)
		printf("SDL_Init error");
	context->renderer = NULL;
	context->window = SDL_CreateWindow("SovietPunk 1947",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	context->renderer = SDL_CreateRenderer(context->window, -1, 0); //Read documentation to find out if surface is created aswell!
	if (context->renderer == NULL)
		printf("Still null? \n"); //Error exit
	if (context->window == NULL)
		printf("Couldn't create SDL2 window :("); //error exit
	printf("sdl context and init \n");
	context->surface = SDL_GetWindowSurface(context->window);
	SDL_UpdateWindowSurface(context->window);
}

int	main(int argc, char **args)
{
	t_gamecontext	gcontext;
	t_sdlcontext	context;
	ft_bzero(&context, sizeof(t_sdlcontext));
	ft_bzero(&gcontext, sizeof(t_gamecontext));
	
	createsdlcontext(&context);
	gcontext.sdlcontext = &context;
	loadpngs(&context);
	//load images
	if (argc == 3 && ft_strcmp(args[1], "-e") == 0 && ft_strlen(args[2]) > 0)
		mapcreator(args[2], context);
	else
		gameloop(&gcontext);
	SDL_DestroyWindow(context.window);
	SDL_Quit();
	return (0);
}