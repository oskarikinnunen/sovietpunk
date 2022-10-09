/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 01:49:17 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 12:17:06 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

Uint32	samplecolor(t_simpleimg img, int ix, int iy)
{
	int	index;

	index = ix + (iy * img.size[Y]);
	if (index < 0 || index >= img.length)
		return (0);
	//index = ft_clamp(index, 0, img.length - 1);
	return (img.data[index]);
}

void	alloc_image(t_simpleimg *img, int width, int height)
{
	img->size[X] = width;
	img->size[Y] = width;
	img->length = width * height * sizeof(uint32_t);
	img->data = ft_memalloc(img->length);
}

void	drawimagescaled(t_sdlcontext *context, int p[2], int tid, int scale)
{
	int			iy;
	int			ix;
	int			color;
	float		scalar;
	t_simpleimg	img;

	iy = 0;
	
	tid = ft_clamp(tid, 0, PNG_COUNT - 1); //TODO: texturecount
	img = context->images[tid];
	scalar = ((float)img.size[X] / (float)scale);
	while (iy++ < scale - 1)
	{
		ix = 0;
		if (iy + p[Y] < 0 || iy + p[Y] > WINDOW_H)
			continue;
		while (ix++ < scale)
		{
			if (ix + p[X] < 0 || ix + p[X] >= WINDOW_W)
				continue;
			color = samplecolor(img, (float)(ix + iy) * scalar, (float)(iy * scalar));
			if (color != 0)
				draw(context->surface->pixels, (int [2]){ix + p[X], iy + p[Y]}, color);
		}
	}
}
