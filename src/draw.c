/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/04 05:48:12 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 16:13:59 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "bresenham.h"

void	draw(uint32_t *pxls, int crd[2], uint32_t clr)
{
	if (crd[X] < 0 || crd[X] >= WINDOW_W - 1
		|| crd[Y] < 0 || crd[Y] >= WINDOW_H - 1)
		return ;
	//printf("drawing %i %i \n", crd[X], crd[Y]);
	pxls[crd[X] + crd[Y] * WINDOW_W] = clr;
}

void	drawrect(uint32_t *pxls, int crd[2], int clr, int size)
{
	int	i;
	int	pos[2];

	i = 0;
	while (i < size)
	{
		v2cpy(pos, (int [2]){crd[X] + i, crd[Y]});
		draw(pxls, pos, clr);
		v2cpy(pos, (int [2]){crd[X], crd[Y] + i});
		draw(pxls, pos, clr);
		v2cpy(pos, (int [2]){crd[X] + size, crd[Y] + i});
		draw(pxls, pos, clr);
		v2cpy(pos, (int [2]){crd[X] + i, crd[Y] + size});
		draw(pxls, pos, clr);
		i++;
	}
}

void	drawline(uint32_t *pxls, int from[2], int to[2], uint32_t clr)
{
	static t_bresenham	b;

	populate_bresenham(&b, from, to);
	draw(pxls, b.local, clr);
	while (step_bresenham(&b) != 1)
		draw(pxls, b.local, clr);
	draw(pxls, b.local, clr);
}


void	drawcircle(uint32_t *pxls, int crd[2], int size, uint32_t clr)
{
	int		edges[CIRCLESIDES + 1][2];
	int		i;
	float	angl;

	i = 0;
	angl = 0.0f;
	while (i < CIRCLESIDES + 1)
	{
		edges[i][X] = crd[X] + ((sin(angl) * (float)size));
		edges[i][Y] = crd[Y] + ((cos(angl) * (float)size));
		if (i >= 1)
			drawline(pxls, edges[i - 1], edges[i], clr);
		angl += FULLRAD / CIRCLESIDES;
		i++;
	}
}

/*void	imgtoscreen(uint32_t *pxls, t_img *img)
{
	int			p[2];
	uint32_t	clr;

	p[Y] = 0;
	while(p[Y] < img->size[Y])
	{
		p[X] = 0;
		while(p[X] < img->size[X])
		{
			clr = img->data[p[X] + p[Y] * img->size[X]]; //TODO: sampleimg funtion;
			if (clr != 0)
				draw(pxls, p, clr); //TODO: sampleimg function
			p[X]++;
		}
		p[Y]++;
	}
}*/
