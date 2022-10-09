/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawquadtile.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 12:41:30 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 15:50:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"

#define QD_CENTER 2

static void	draw_westside(uint32_t *pxls, int data[4],
	t_simpleimg img, float scalar)
{
	t_bresenham	b;
	int			i;
	int			iy;
	uint32_t	clr;
	bool		reached;

	i = 0;
	iy = data[QD_CENTER];
	while (i < data[QD_CENTER])
	{
		populate_bresenham(&b,
			(int [2]){iy, i + data[QD_CENTER]},
			(int [2]){iy, -i + data[QD_CENTER]});
		reached = false;
		while (!reached)
		{
			clr = samplecolor(img,
					(b.local[X] + b.local[Y]) * scalar, b.local[Y] * scalar);
			draw(pxls,
				(int [2]){b.local[X] + data[X], b.local[Y] + data[Y]}, clr);
			reached = step_bresenham(&b);
		}
		iy--;
		i++;
	}
}

static void	draw_eastside(uint32_t *pxls, int data[4],
	t_simpleimg img, float scalar)
{
	t_bresenham	b;
	int			i;
	int			iy;
	uint32_t	clr;
	bool		reached;

	i = 0;
	iy = data[QD_CENTER];
	while (i < data[QD_CENTER])
	{
		populate_bresenham(&b,
			(int [2]){iy, i + data[QD_CENTER]},
			(int [2]){iy, -i + data[QD_CENTER]});
		reached = false;
		while (!reached)
		{
			clr = samplecolor(img,
					(b.local[X] + b.local[Y]) * scalar, b.local[Y] * scalar);
			draw(pxls,
				(int [2]){b.local[X] + data[X], b.local[Y] + data[Y]}, clr);
			reached = step_bresenham(&b);
		}
		iy++;
		i++;
	}
}

static void	drawnorthside(uint32_t *pxls, int data[4],
	t_simpleimg img, float scalar)
{
	t_bresenham	b;
	int			i;
	int			iy;
	uint32_t	clr;
	bool		reached;

	i = 1;
	iy = data[QD_CENTER];
	while (i < data[QD_CENTER])
	{
		populate_bresenham(&b,
			(int [2]){i + data[QD_CENTER], iy},
			(int [2]){-i + data[QD_CENTER], iy});
		reached = false;
		while (!reached)
		{
			clr = samplecolor(img,
					(b.local[X] + b.local[Y]) * scalar, b.local[Y] * scalar);
			draw(pxls,
				(int [2]){b.local[X] + data[X], b.local[Y] + data[Y]}, clr);
			reached = step_bresenham(&b);
		}
		iy--;
		i++;
	}
}

static void	drawsouthside(uint32_t *pxls, int data[4],
	t_simpleimg img, float scalar)
{
	t_bresenham	b;
	int			i;
	int			iy;
	uint32_t	clr;
	bool		reached;

	i = 0;
	iy = data[QD_CENTER];
	while (i < data[QD_CENTER])
	{
		populate_bresenham(&b,
			(int [2]){i + data[QD_CENTER], iy},
			(int [2]){-i + data[QD_CENTER], iy});
		reached = false;
		while (!reached)
		{
			clr = samplecolor(img,
					(b.local[X] + b.local[Y]) * scalar, b.local[Y] * scalar);
			draw(pxls,
				(int [2]){b.local[X] + data[X], b.local[Y] + data[Y]}, clr);
			reached = step_bresenham(&b);
		}
		iy++;
		i++;
	}
}

void	drawquadtile(t_sdlcontext *context, int p[2], uint32_t wall, int scale)
{
	float		scalar;
	t_simpleimg	img;
	int			quadtilingdata[4];

	v2cpy(quadtilingdata, p);
	quadtilingdata[QD_CENTER] = scale / 2;
	img = context->images[getindexedwall(wall, NORTHWALL)];
	scalar = ((float)img.size[X] / (float)scale);
	drawnorthside(context->surface->pixels, quadtilingdata, img, scalar);
	img = context->images[getindexedwall(wall, WESTWALL)];
	scalar = ((float)img.size[X] / (float)scale);
	draw_westside(context->surface->pixels, quadtilingdata, img, scalar);
	img = context->images[getindexedwall(wall, EASTWALL)];
	scalar = ((float)img.size[X] / (float)scale);
	draw_eastside(context->surface->pixels, quadtilingdata, img, scalar);
	img = context->images[getindexedwall(wall, SOUTHWALL)];
	scalar = ((float)img.size[X] / (float)scale);
	drawsouthside(context->surface->pixels, quadtilingdata, img, scalar);
}
