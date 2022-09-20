/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:05:51 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/20 14:25:31 by okinnune         ###   ########.fr       */
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

	t = ft_memalloc(sizeof(u_int32_t) * (WINDOW_H / 2));
	i = 0;
	angle = 1.5708f;
	
	SDL_SetRenderDrawColor(sdl->renderer, 100,100,100,255);
	while (i < WINDOW_H / 2)
	{
		angle += RAYSLICE; //FOV STEP constant
		ray[X] = sin(angle) * 2000 * 64; 
		ray[Y] = cos(angle) * 2000 * 64;						//
		int height = 32;
		ray[Y] += height;
		populate_bresenham(&b, (int[2]){0, height}, ray);
		
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
	/*context->renderer = SDL_CreateRenderer(context->window, -1, SDL_RENDERER_ACCELERATED); //Read documentation to find out if surface is created aswell!
	if (context->renderer == NULL)
		printf("Still null? \n"); //Error exit */
	if (context->window == NULL)
		printf("Couldn't create SDL2 window :("); //error exit
	printf("sdl context and init \n");
	
	context->surface = SDL_GetWindowSurface(context->window);
	//context->surface = SDL_ConvertSurfaceFormat(context->surface, SDL_PIXELFORMAT_ARGB32, 0); //WHY NO WORK
	
	
	if (context->surface == NULL)
		(printf("WTH SURFACE NULL"), exit(0));
	
	
	SDL_LockSurface(context->surface);
	
	printf("%s \n", SDL_GetError());
	//exit(0);
	context->ft = floortable(context);
	int i = 0;
	//SDL_UpdateWindowSurface(context->window);
}

int	main(int argc, char **args)
{
	t_gamecontext	gc;
	t_sdlcontext	sdl;
	t_obj			objs[20];
	t_fdf			fdfs[1];
	ft_bzero(&sdl, sizeof(t_sdlcontext));
	ft_bzero(&gc, sizeof(t_gamecontext));
	
	createsdlcontext(&sdl);
	gc.sdlcontext = &sdl;
	gc.sdlcontext->fdfs = fdfs;
	
	loadpngs(&sdl);

	//obj load
	ft_bzero(objs, sizeof (t_obj [20]));
	parse_obj(objs);
	gc.sdlcontext->fdfs->obj = objs;
	//printf("clr obj1 %i %i %i \n", objs[1].mtlcolors[720] & 0xFF, objs[1].mtlcolors[720] << 8 & 0xFF, objs[1].mtlcolors[720] << 16 & 0xFF);
	//printf("clr obj0 %i %i %i \n", objs[0].mtlcolors[720] & 0xFF, objs[0].mtlcolors[720] << 8 & 0xFF, objs[0].mtlcolors[720] << 16 & 0xFF);
	//exit(0);
	fdf_init(gc.sdlcontext->fdfs, &gc.sdlcontext->images[3], &objs[1]);
	fdfs->clock = gc.clock;
	gc.sdlcontext->fdfs->crd[X] = 420;
	gc.sdlcontext->fdfs->crd[Y] = 512;
	
	//load images
	if (argc == 3 && ft_strcmp(args[1], "-e") == 0 && ft_strlen(args[2]) > 0)
		mapcreator(args[2], sdl);
	else
		gameloop(gc);
	SDL_DestroyWindow(sdl.window);
	SDL_Quit();
	return (0);
}