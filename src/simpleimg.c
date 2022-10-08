/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simpleimg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 02:40:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/08 11:33:21 by okinnune         ###   ########.fr       */
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
	free(png->data);
	free(png->palette.plte); // check if palette exists?
}

void	loadpngs(t_sdlcontext	*sdl)
{
	t_pngdata	png_stack[PNG_COUNT];
	int			i;
	
	//nullcheck
	pngparse(png_stack, "tex/floor1.png");
	pngparse(&png_stack[1], "tex/wall_cf.png");
	pngparse(&png_stack[2], "tex/wall_orange.png");
	pngparse(&png_stack[3], "tex/console.png");
	pngparse(&png_stack[4], "tex/wall_red.png");
	pngparse(&png_stack[TILE_SPAWN], "tex/pxlflag_d.png"); //Spawn always last

	//Converting png to simpleimg
	sdl->images	= ft_memalloc(sizeof(t_simpleimg) * PNG_COUNT); //4th one WAS for mini_fdf
	i = 0;
	while (i < PNG_COUNT)
	{
		pngtosimpleimg(&png_stack[i], &sdl->images[i]);
		i++;
	}
}