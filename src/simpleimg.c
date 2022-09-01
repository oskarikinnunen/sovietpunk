/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simpleimg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 02:40:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/01 00:46:40 by okinnune         ###   ########.fr       */
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
	pngparse(png_stack, "textures/tex2finalUC.png");
	pngparse(&png_stack[1], "textures/walluc2.png");
	pngparse(&png_stack[2], "textures/spawn.png");

	//Converting png to simpleimg
	sdl->images	= ft_memalloc(sizeof(t_simpleimg) * 3);
	pngtosimpleimg(png_stack, sdl->images);
	pngtosimpleimg(&png_stack[1], &sdl->images[1]); //TODO LOOP
	pngtosimpleimg(&png_stack[2], &sdl->images[2]);
	//textures can be free here, simpleimg can be the only data remaining
}