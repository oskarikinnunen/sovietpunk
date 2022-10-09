/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_fdf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:09:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 00:22:21 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP_OBJ.h"
#include "v3lib.h"
#include "SP1947.h"

float	z_depth(float **fv3s)
{
	float	z;
	int		i;

	i = 0;
	z = -10000.0f;
	while (i < 3)
	{
		if (fv3s[i][Z] > z)
			z = fv3s[i][Z] + 10000.0f;
		i++;
	}
	return (z);
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
	int			cf;

	i = 0;
	cf = fdf.curframe;
	ft_bzero(fdf.img.data, fdf.img.length * sizeof(uint32_t));
	ft_bzero(fdf.depth, fdf.img.length * sizeof(float));
	while (i < fdf.obj->f_count)
	{
		float z =	-fdf.verts[fdf.obj[cf].faces[i][0]][Z] +
					-fdf.verts[fdf.obj[cf].faces[i][1]][Z] + 
					-fdf.verts[fdf.obj[cf].faces[i][2]][Z];
		if (fdf.obj->colors[i] != 0)
		{
			c = fdf.obj->mtlcolors[fdf.obj->colors[i] - 1];
			if (fdf.obj->colors[i] != 4)
				c = shade(c, fdf.scale);
		}
		fv3_to_iv3(fdf.verts[fdf.obj[cf].faces[i][0]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj[cf].faces[i][1]], i3[1]);
		fv3_to_iv3(fdf.verts[fdf.obj[cf].faces[i][2]], i3[2]);
		z *= 5.0f;
		z += 10000.0f;
		fill_tri(i3, &fdf, z, c);
		draw_line(&fdf, (populate_bresenham(&b, i3[0], i3[1]), b), z, c);
		draw_line(&fdf, (populate_bresenham(&b, i3[1], i3[2]), b), z, c);
		draw_line(&fdf, (populate_bresenham(&b, i3[2], i3[0]), b), z, c);
		i++;
	}
}

static void	calc_matrices(t_fdf *fdf)
{
	float	angles[2];

	ft_bzero(fdf->matrices, sizeof(float [2][3][3]));
	angles[X] = fdf->view[Y];
	fdf->matrices[1][X][X] = cos(angles[X]);
	fdf->matrices[1][Y][X] = sin(angles[X]);
	fdf->matrices[1][Z][Y] = 1.0f;
	fdf->matrices[1][Y][Z] = cos(angles[X]);
	fdf->matrices[1][X][Z] = -sin(angles[X]); // actually correct
}

//TODO: fdf_init: allocates the memory and handles errors if that fails
#include "assert.h"
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
		if (fdf->verts[i] == NULL) //TODO: Free the prev vertices in this case of fail
			return (-1);
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
	//fdf->clock->delta
	//return ;
	i = 0;
	while (i < fdf->obj->v_count)
	{
		fdf->verts[i][X] =
			(float)fdf->obj[fdf->curframe].
			verts[i][X];
		fdf->verts[i][Y] = (float)fdf->obj[fdf->curframe].verts[i][Y];
		fdf->verts[i][Z] = (float)fdf->obj[fdf->curframe].verts[i][Z];
		//v3_mul(fdf->matrices[X], fdf->verts[i]);
		v3_mul(fdf->matrices[Y], fdf->verts[i]);
		v3_add(fdf->verts[i], (float [3]) {fdf->img.size[X] / 2, 400, 0});
		i++;
	}
	ft_bzero(fdf->img.data, fdf->img.length * sizeof(uint32_t));
	fdf_draw(*fdf);
}
