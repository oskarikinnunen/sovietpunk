/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simpleimg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 02:40:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/26 19:54:19 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP_PNG.h"

void	pngtosimpleimg(t_pngdata *png, t_simpleimg *img)
{
	int	i;

	ft_memcpy(img->size, png->size, sizeof(Uint32[2]));
	img->length = png->size[X] * png->size[Y];
	img->data = ft_memalloc(img->length * sizeof(Uint32));
	i = 0;
	while (i < img->length)
	{
		img->data[i] = png->palette.plte[png->data[i]];
		i++;
	}
	//ft_memcpy(sdl->images->data, sdl->textures->data, sdl->images->length);
}

void	loadpngs(t_sdlcontext	*sdl)
{
	t_pngdata	*pngs;
	t_pngdata	png_stack[3];
	
	//pngs = ft_memalloc(2 * sizeof(t_pngdata));
	
	//nullcheck
	pngparse(png_stack, "tex/flooruc.png");
	pngparse(&png_stack[1], "tex/wallu.png");
	pngparse(&png_stack[2], "textures/spawn.png");

	//Converting png to simpleimg
	sdl->images	= ft_memalloc(sizeof(t_simpleimg) * 4); //4th one is for mini_fdf
	sdl->images[3].data = ft_memalloc(sizeof(Uint32) * 400 * 400);
	sdl->images[3].size[X] = 400;
	sdl->images[3].size[Y] = 400;
	sdl->images[3].length = 400 * 400;

	pngtosimpleimg(png_stack, sdl->images);
	pngtosimpleimg(&png_stack[1], &sdl->images[1]); //TODO LOOP
	pngtosimpleimg(&png_stack[2], &sdl->images[2]);
	//textures can be free here, simpleimg can be the only data remaining
}