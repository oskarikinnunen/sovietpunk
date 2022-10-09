/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_fdf_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 00:08:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 00:15:18 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	fv3_to_iv3(float *f3, int *i3)
{
	i3[X] = (int)f3[X];
	i3[Y] = (int)f3[Y];
	i3[Z] = (int)f3[Z];
}

void	draw_line(t_fdf *fdf, t_bresenham b, float z, uint32_t c)
{
	unsigned int	offset;
	int				b_res;

	b_res = 0;
	while (b_res != 1)
	{
		offset = b.local[X] + (b.local[Y] * fdf->img.size[X]);
		if (offset < fdf->img.length && fdf->depth[offset] <= z)
		{
			fdf->depth[offset] = z;
			fdf->img.data[offset] = c;
		}
		b_res = step_bresenham(&b);
	}
}

void	sort_tris(int tris[3][3])
{
	int	s_x;
	int	s_j;
	int	temp[3];

	s_x = 0;
	s_j = 0;
	while (s_x < 2)
	{
		while (s_j < 2 - s_x)
		{
			if (tris[s_j][Y] < tris[s_j + 1][Y])
			{
				ft_memcpy(temp, tris[s_j], sizeof(int) * 3);
				ft_memcpy(tris[s_j], tris[s_j + 1], sizeof(int) * 3);
				ft_memcpy(tris[s_j + 1], temp, sizeof(int) * 3);
			}
			s_j++;
		}
		s_j = 0;
		s_x++;
	}
}

void	fill_sub_tri(int *tris[3], t_fdf *fdf, float z, int c)
{
	t_bresenham	b[2];
	t_bresenham	bf;

	populate_bresenham(&(b[0]), tris[0], tris[1]);
	populate_bresenham(&(b[1]), tris[0], tris[2]);
	while (b[0].local[Y] != tris[1][Y])
	{
		populate_bresenham(&bf, b[0].local, b[1].local);
		draw_line(fdf, bf, z, c);
		while (b[0].local[Y] == b[1].local[Y])
			step_bresenham(&(b[0]));
		while (b[1].local[Y] != b[0].local[Y])
			step_bresenham(&(b[1]));
	}
	populate_bresenham(&bf, b[0].local, b[1].local);
	draw_line(fdf, bf, z, c);
}

void	fill_tri(int tri[3][3], t_fdf *fdf, float z, uint32_t c)
{
	int			split[3];
	int			sorted[3][3];
	float		lerp;
	t_bresenham	b;

	sort_tris(ft_memcpy(sorted, tri, sizeof(int [3][3])));
	lerp = (float)(sorted[1][Y] - sorted[2][Y])
		/ (float)(sorted[0][Y] - sorted[2][Y]);
	split[X] = sorted[2][X] + (lerp * (sorted[0][X] - sorted[2][X]));
	split[Y] = sorted[1][Y];
	split[Z] = sorted[1][Z];
	fill_sub_tri((int *[3]){(int *)&(sorted[0]),
		(int *)&(sorted[1]), (int *)&split}, fdf, z, c);
	fill_sub_tri((int *[3]){(int *)&(sorted[2]),
		(int *)&(sorted[1]), (int *)&split}, fdf, z, c);
	if (sorted[0][X] != split[X] && sorted[0][Y] != split[Y])
	{
		populate_bresenham(&b, sorted[0], split);
		draw_line(fdf, b, z, c);
	}
}
