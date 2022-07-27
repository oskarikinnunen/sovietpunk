/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 01:49:17 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/27 02:09:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

Uint32	samplecolor(t_simpleimg img, int ix, int iy)
{
	int	index;

	index = ix + (iy * img.size[Y]);
	return (img.data[index]);
}

void	drawimage(t_SDL_Context context, int x, int y)
{
	int			iy;
	int			ix;
	int			color;
	t_simpleimg	img;

	iy = 0;
	ix = 0;
	img = context.images[0];
	while (iy++ < img.size[Y])
	{
		while (ix++ < img.size[X])
		{
			color = samplecolor(img, ix, iy);
			SDL_SetRenderDrawColor(context.renderer, color & 0xFF, color >> 8 & 0xFF, color >> 16 & 0xFF, 0xFF);
			SDL_RenderDrawPoint(context.renderer, ix + x, iy + y);
		}
		ix = 0;
	}
}