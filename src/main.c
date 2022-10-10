/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:05:51 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 14:53:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"

int	*floortable(t_sdlcontext *sdl)
{
	int			*t;
	int			i;
	float		angle;
	int			ray[2];
	t_bresenham	b;

	t = ft_memalloc(sizeof(u_int32_t) * (WINDOW_H / 2));
	if (t == NULL)
		error_exit("alloc fail");
	i = 0;
	angle = RAD90;
	while (i < WINDOW_H / 2)
	{
		angle += RAYSLICE;
		ray[X] = sin(angle) * 64 * 1000;
		ray[Y] = cos(angle) * 64 * 1000;
		ray[Y] += GAMESCALE / 2;
		populate_bresenham(&b, (int [2]){0, GAMESCALE / 2}, ray);
		while (b.local[Y] > 0)
			step_bresenham(&b);
		t[i] = b.local[X];
		i++;
	}
	return (t);
}

void	createsdlcontext(t_sdlcontext *context)
{
	ft_bzero(context, sizeof(t_sdlcontext));
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || SDL_Init(SDL_INIT_EVENTS) < 0)
		error_exit("sdl init error");
	context->window = SDL_CreateWindow("SovietPunk 1947",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	if (context->window == NULL)
		error_exit("couldn't create sdl window");
	context->surface = SDL_GetWindowSurface(context->window);
	if (context->surface == NULL)
		error_exit("couldn't create sdl surface");
	context->ft = floortable(context);
}

static char	*getmapname(int argc, char **args)
{
	if (argc == 3 && ft_strcmp(args[1], "-o") == 0)
		return (args[2]);
	if (argc == 1)
		return ("map.map");
	return ("error_wrong_arg");
}

int	main(int argc, char **args)
{
	t_gamecontext	gc;
	t_obj			objs[ANIMFRAMES];

	ft_bzero(&gc, sizeof(t_gamecontext));
	createsdlcontext(&gc.sdl);
	loadpngs(&gc.sdl);
	parse_obj(objs);
	ft_bzero(gc.sdl.fdfs, sizeof(t_fdf));
	alloc_image(&gc.sdl.fdfs->img, 400, 400);
	fdf_init(gc.sdl.fdfs, objs);
	gc.sdl.fdfs->crd[X] = 640;
	gc.sdl.fdfs->crd[Y] = 640;
	if (argc == 3 && ft_strcmp(args[1], "-e") == 0 && ft_strlen(args[2]) > 0)
		mapcreator(args[2], gc);
	else
	{
		gc.mapname = getmapname(argc, args);
		gameloop(gc);
	}
	SDL_DestroyWindow(gc.sdl.window);
	SDL_Quit();
	return (0);
}
