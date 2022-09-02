/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:05:51 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/02 04:54:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"

int	*floortable(t_sdlcontext *sdl)
{
	int		*t;
	int		i;
	float	angle;
	int		ray[2];
	t_bresenham	b;

	t = ft_memalloc(sizeof(u_int32_t) * 240);
	i = 0;
	angle = 1.5708f;
	
	SDL_SetRenderDrawColor(sdl->renderer, 100,100,100,255);
	while (i < 240)
	{
		angle += RAYSLICE; //FOV STEP constant
		ray[X] = sin(angle) * 2000 * 64; 
		ray[Y] = cos(angle) * 2000 * 64;						//
		ray[Y] += 32;
		populate_bresenham(&b, (int[2]){0, 32}, ray);
		
		SDL_RenderPresent(sdl->renderer);
		while(b.local[Y] > 0)
			step_bresenham(&b);
		# define SCL 5
		SDL_RenderDrawLine(sdl->renderer, (0 + 100) / SCL, (32 + 100) / SCL, (b.local[X] + 100) / SCL, (b.local[Y] + 100) / SCL);
		t[i] = b.local[X];
		i++;
	}
	printf("exit ft \n");
	//i = 240;
	/*while (i > 1)
	{
		//t[i] = ft_abs(t[i] - t[i - 1]);
		printf("diff %i \n", t[i]);
		i--;
	}*/
	//SDL_Delay(5000);
	return (t);
}

void	createsdlcontext(t_sdlcontext *context)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_EVENTS) < 0)
		printf("SDL_Init error");
	context->renderer = NULL;
	context->window = SDL_CreateWindow("SovietPunk 1947",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED); //Read documentation to find out if surface is created aswell!
	if (context->renderer == NULL)
		printf("Still null? \n"); //Error exit
	if (context->window == NULL)
		printf("Couldn't create SDL2 window :("); //error exit
	printf("sdl context and init \n");
	context->surface = SDL_GetWindowSurface(context->window);
	SDL_PixelFormat *f =  SDL_AllocFormat(SDL_PIXELFORMAT_ABGR32);
	//context->surface = SDL_ConvertSurface(context->surface, f, 0);
	//context->tex = SDL_CreateTextureFromSurface(context->renderer, context->surface);
	//SDL_Pixel
	context->ft = floortable(context);
	int i = 0;
	//SDL_UpdateWindowSurface(context->window);
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