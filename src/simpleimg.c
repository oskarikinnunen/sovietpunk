/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simpleimg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 02:40:52 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 23:03:01 by okinnune         ###   ########.fr       */
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
	pngparse(png_stack, "tex/output/floory2.png");
	pngparse(&png_stack[1], "tex/output/floor1.png");
	pngparse(&png_stack[2], "tex/output/wall4.png");
	pngparse(&png_stack[3], "tex/output/wall4_ruin2.png");
	pngparse(&png_stack[4], "tex/output/wall4_ruin_pc.png");
	pngparse(&png_stack[5], "tex/output/wall2.png");
	pngparse(&png_stack[6], "tex/output/wall3_ruin.png");
	pngparse(&png_stack[7], "tex/output/wall3_ruin2.png");
	pngparse(&png_stack[8], "tex/output/wall3.png");
	pngparse(&png_stack[9], "tex/output/wall4_pc_blue.png");
	pngparse(&png_stack[10], "tex/output/wall4_pc.png");
	pngparse(&png_stack[11], "tex/output/wall4_ruin.png");
	pngparse(&png_stack[12], "tex/output/wall4.png");
	pngparse(&png_stack[TILE_SPAWN], "tex/output/spawn.png"); //Spawn always last

	//Converting png to simpleimg
	sdl->images	= ft_memalloc(sizeof(t_simpleimg) * PNG_COUNT); //4th one WAS for mini_fdf
	i = 0;
	while (i < PNG_COUNT)
	{
		pngtosimpleimg(&png_stack[i], &sdl->images[i]);
		i++;
	}
}