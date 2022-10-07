/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simpleimg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 02:40:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/07 16:03:45 by okinnune         ###   ########.fr       */
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
	t_pngdata	png_stack[4];
	int			i;
	
	//nullcheck
	pngparse(png_stack, "tex/flooruc.png");
	pngparse(&png_stack[1], "tex/wallu.png");
	pngparse(&png_stack[2], "tex/walluc2.png");
	pngparse(&png_stack[3], "tex/pxlflag_d.png");

	//Converting png to simpleimg
	sdl->images	= ft_memalloc(sizeof(t_simpleimg) * 4); //4th one WAS for mini_fdf
	i = 0;
	while (i < 4)
	{
		pngtosimpleimg(&png_stack[i], &sdl->images[i]);
		i++;
	}
}