/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_drawline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 03:54:03 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/22 20:40:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP_OBJ.h"
#include "SP1947.h"
#include "bresenham.h"
#include "v3lib.h"

/*static void	draw_line(t_fdf fdf, t_bresenham b)
{
	int				b_res;
	unsigned int	offset;

	b_res = 0;
	while (b_res != 1)
	{
		offset = b.local[X] + (b.local[Y] * (fdf.img->sz_line / sizeof(int)));
		if (b.local[X] < fdf.img->size[X] && b.local[Y] < fdf.img->size[Y] &&
			b.local[X] >= 0 && b.local[Y] >= 0)
			*((unsigned int *)(fdf.img->data_addr) + offset) = INT_MAX;
		b_res = step_bresenham(&b);
	}
}*/

