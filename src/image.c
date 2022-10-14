/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/27 01:49:17 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 16:11:38 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

Uint32	samplecolor(t_simpleimg img, int ix, int iy)
{
	int	index;

	index = ix + (iy * img.size[Y]);
	if (index < 0)
		index = 0;
	if (ix >= (img.size[X] * img.size[Y]) - 1 || iy >= img.size[Y] - 1)
		index = img.length - 1;
	if (img.length == 0)
		return (0);
	return (img.data[index]);
}

void	alloc_image(t_simpleimg *img, int width, int height)
{
	img->size[X] = width;
	img->size[Y] = width;
	img->length = width * height;
	img->data = ft_memalloc(img->length * sizeof(uint32_t));
	if (img->data == NULL)
		error_exit("alloc fail");
}

void	put_image_to_screen(t_sdlcontext *context, t_simpleimg img, int p[2])
{
	int			iy;
	int			ix;
	uint32_t	color;

	iy = 0;
	while (iy++ < img.size[Y])
	{
		ix = 0;
		if (iy + p[Y] < 0 || iy + p[Y] > WINDOW_H)
			continue ;
		while (ix++ < img.size[X] - 1)
		{
			if (ix + p[X] < 0 || ix + p[X] >= WINDOW_W)
				continue ;
			color = samplecolor(img, ix, iy);
			draw(context->surface->pixels,
				(int [2]){ix + p[X], iy + p[Y]}, color);
		}
	}
}

void	drawimagescaled(t_sdlcontext *context, int p[2], int tid, int scale)
{
	int			iy;
	int			ix;
	int			color;
	float		scalar;
	t_simpleimg	img;

	iy = 0;
	tid = ft_clamp(tid, 0, PNG_COUNT - 1);
	img = context->images[tid];
	scalar = ((float)img.size[X] / (float)scale);
	while (iy++ < scale - 1)
	{
		ix = 0;
		if (iy + p[Y] < 0 || iy + p[Y] > WINDOW_H)
			continue ;
		while (ix++ < scale)
		{
			if (ix + p[X] < 0 || ix + p[X] >= WINDOW_W)
				continue ;
			color = samplecolor(img, (ix + iy) * scalar, (iy * scalar));
			if (color != 0)
				draw(context->surface->pixels,
					(int [2]){ix + p[X], iy + p[Y]}, color);
		}
	}
}
