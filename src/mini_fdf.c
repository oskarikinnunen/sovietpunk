/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_fdf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:09:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 14:54:23 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "v3lib.h"
#include "SP1947.h"

float	z_depth(t_fdf fdf, int i)
{
	float	z;
	int		cf;

	cf = fdf.curframe;
	z = -fdf.verts[fdf.obj[cf].faces[i][0]][Z]
		-fdf.verts[fdf.obj[cf].faces[i][1]][Z]
		-fdf.verts[fdf.obj[cf].faces[i][2]][Z];
	return (z + 10000.0f);
}

void	anim(t_fdf *fdf)
{
	static uint32_t	tick;

	tick += fdf->clock->delta;
	if (tick > 100)
	{
		fdf->curframe++;
		if (fdf->curframe >= ANIMFRAMES)
			fdf->curframe = 0;
		tick = 0;
	}
}

void	fdf_draw(t_fdf fdf)
{
	int			i;
	int			i3[3][3];
	t_bresenham	b;
	uint32_t	c;
	float		z;

	i = 0;
	while (i < fdf.obj->f_count)
	{
		z = z_depth(fdf, i);
		if (fdf.obj->colors[i] != 0)
		{
			c = fdf.obj->mtlcolors[fdf.obj->colors[i] - 1];
			if (fdf.obj->colors[i] != 4)
				c = shade(c, fdf.scale);
		}
		fv3_to_iv3(fdf.verts[fdf.obj[fdf.curframe].faces[i][0]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj[fdf.curframe].faces[i][1]], i3[1]);
		fv3_to_iv3(fdf.verts[fdf.obj[fdf.curframe].faces[i][2]], i3[2]);
		fill_tri(i3, &fdf, z, c);
		draw_line(&fdf, (populate_bresenham(&b, i3[0], i3[1]), b), z, c);
		draw_line(&fdf, (populate_bresenham(&b, i3[1], i3[2]), b), z, c);
		draw_line(&fdf, (populate_bresenham(&b, i3[2], i3[0]), b), z, c);
		i++;
	}
}

int	fdf_init(t_fdf *fdf, t_obj *object)
{
	int	i;

	fdf->depth = ft_memalloc(sizeof(float) * fdf->img.length);
	fdf->verts = ft_memalloc(sizeof(float *) * object->v_count);
	fdf->obj = &object[0];
	if (fdf->depth == NULL || fdf->verts == NULL)
		return (-1);
	i = 0;
	while (i < fdf->obj->v_count)
	{
		fdf->verts[i] = ft_memalloc(sizeof(float *) * 3);
		if (fdf->verts[i] == NULL)
			error_exit("alloc fail");
		i++;
	}
	calc_matrices(fdf);
	return (1);
}

void	fdf_update(t_fdf *fdf)
{
	int	i;

	calc_matrices(fdf);
	anim(fdf);
	i = 0;
	while (i < fdf->obj->v_count)
	{
		fdf->verts[i][X] = (float)fdf->obj[fdf->curframe].verts[i][X];
		fdf->verts[i][Y] = (float)fdf->obj[fdf->curframe].verts[i][Y];
		fdf->verts[i][Z] = (float)fdf->obj[fdf->curframe].verts[i][Z];
		v3_mul(fdf->matrices[Y], fdf->verts[i]);
		v3_add(fdf->verts[i], (float [3]){fdf->img.size[X] / 2, 400, 0});
		i++;
	}
	ft_bzero(fdf->img.data, fdf->img.length * sizeof(uint32_t));
	ft_bzero(fdf->depth, fdf->img.length * sizeof(float));
	fdf_draw(*fdf);
}
