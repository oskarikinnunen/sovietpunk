/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 01:49:17 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/07 18:48:45 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

Uint32	samplecolor(t_simpleimg img, int ix, int iy)
{
	int	index;

	index = ix + (iy * img.size[Y]);
	index = ft_clamp(index, 0, img.length);
	return (img.data[index]);
}

void	drawimagescaled(t_sdlcontext *context, int p[2], int tid, int scale)
{
	int			iy;
	int			ix;
	int			color;
	t_simpleimg	img;

	iy = 0;
	ix = 0;
	//tid = ft_clamp(tid - 1, 0, 1);
	//tid = tid - 1;

	//if (tid > 2)
	//	tid = 2;
	
	img = context->images[tid];
	while (iy++ <= scale)
	{
		while (ix++ < scale)
		{
			//color = samplecolor(img, ix * (img.size[X] / scale) + iy * (img.size[X] / scale), iy * (img.size[Y] / scale));
			color = samplecolor(img, ix * (img.size[X] / scale), iy * (img.size[Y] / scale));
			int index = ix + p[X] + (iy + p[Y]) * WINDOW_W;
			index = ft_clamp(index, 0, WINDOW_H * WINDOW_W);
			if (color != 0 || ix == 1 || ix == scale -1)
				((int *)context->surface->pixels)[index] = color;
			
			//SDL_SetRenderDrawColor(context->renderer, color & 0xFF, color >> 8 & 0xFF, color >> 16 & 0xFF, 0xFF);
			//SDL_RenderDrawPoint(context->renderer, ix + p[X], iy + p[Y]);
		}
		ix = 0;
	}
}

void	drawimage(t_sdlcontext *context, int x, int y)
{
	int			iy;
	int			ix;
	int			color;
	t_simpleimg	img;

	iy = 0;
	ix = 0;
	img = context->images[3];
	while (iy++ <= img.size[Y] - 4)
	{
		while (ix++ < img.size[X])
		{
			color = samplecolor(img, ft_clamp(ix, 0, img.length), iy);
			if (color != 0)
				((int *)context->surface->pixels)[ix + x + (iy + y) * WINDOW_W] = color;
			//SDL_SetRenderDrawColor(context.renderer, color & 0xFF, color >> 8 & 0xFF, color >> 16 & 0xFF, 0xFF);
			//SDL_RenderDrawPoint(context.renderer, ix + x, iy + y);
		}
		ix = 0;
	}
}