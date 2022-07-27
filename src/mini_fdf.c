/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_fdf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:09:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/27 02:36:57 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP_OBJ.h"
#include "v3lib.h"
#include "SP1947.h"
#include "bresenham.h"


static void fv3_to_iv3(float *f3, int *i3) //TODO: move to vectors
{
	i3[X] = (int)f3[X];
	i3[Y] = (int)f3[Y];
	i3[Z] = (int)f3[Z];
}

void	draw_line(t_simpleimg *si, t_bresenham b)
{
	int				b_res;
	unsigned int	offset;

	b_res = 0;
	//
	while (b_res != 1)
	{
		offset = b.local[X] + (b.local[Y] * si->size[X]);
		if (b.local[X] < si->size[X] && b.local[X] > 0 &&
			b.local[Y] < si->size[Y] && b.local[Y] > 0 && offset < (si->length / sizeof(Uint32)))
			si->data[offset] = INT_MAX;
		//printf("b localx %i, target = %i\n", b.local[X], b.target[X]);
		//printf("b localx %i \n", b.target[X]);
		b_res = step_bresenham(&b);
	}
}

void	fdf_drawskeleton(t_fdf fdf)
{
	int	i;
	int	i3[2][3];
	t_bresenham	b;

	//populate_bresenham(&b)
	i = 0;
	printf("faces in obj %i verts %i \n", fdf.obj->f_count, fdf.obj->v_count);
	ft_bzero(fdf.img->data, fdf.img->length * sizeof(Uint32));
	while (i < fdf.obj->f_count)
	{
		//Start vector
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][0]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][1]], i3[1]);
		populate_bresenham(&b, i3[0], i3[1]);
		draw_line(fdf.img, b);

		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][1]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][2]], i3[1]);
		populate_bresenham(&b, i3[0], i3[1]);
		draw_line(fdf.img, b);

		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][2]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][0]], i3[1]);
		populate_bresenham(&b, i3[0], i3[1]);
		draw_line(fdf.img, b);
		i++;
	}
}

static void	calc_matrices(t_fdf *fdf)
{
	float	angles[2];

	ft_bzero(fdf->matrices, sizeof(float [2][3][3]));
	angles[X] = ft_degtorad(fdf->view[Y]);
	/*fdf->matrices[0][X][X] = cos(angles[X]);
	fdf->matrices[0][X][Y] = -sin(angles[X]);
	fdf->matrices[0][Y][X] = sin(angles[X]);
	fdf->matrices[0][Y][Y] = cos(angles[X]);
	fdf->matrices[0][Z][Z] = 1.0;*/
	fdf->matrices[0][X][X] = cos(angles[X]);
	fdf->matrices[0][X][Z] = -sin(angles[X]);
	fdf->matrices[0][Y][X] = sin(angles[X]);
	fdf->matrices[0][Y][Z] = cos(angles[X]);
	fdf->matrices[0][Z][Y] = 1.0;
	angles[Y] = ft_degtorad(fdf->view[X]);
	angles[Y] = asin(ft_clampf(tan(angles[Y]), -1.0, 1.0));
	fdf->matrices[1][X][X] = 1.0;
	fdf->matrices[1][Y][Y] = cos(angles[Y]);
	fdf->matrices[1][Y][Z] = sin(angles[Y]);
	fdf->matrices[1][Z][Y] = -sin(angles[Y]);
	fdf->matrices[1][Z][Z] = cos(angles[Y]);
	/*fdf->matrices[1][X][X] = 1.0;
	fdf->matrices[1][Y][Z] = cos(angles[Y]);
	fdf->matrices[1][Y][Y] = sin(angles[Y]);
	fdf->matrices[1][Z][Z] = -sin(angles[Y]);
	fdf->matrices[1][Z][Y] = cos(angles[Y]);*/
}

//TODO: fdf_init: allocates the memory and handles errors if that fails
int	fdf_init(t_fdf *fdf, t_simpleimg *img, t_obj *object)
{
	int	i;

	ft_bzero(fdf, sizeof(t_fdf));
	fdf->depth = ft_memalloc(sizeof(int *) * object->v_count);
	fdf->verts = ft_memalloc(sizeof(float *) * object->v_count);
	fdf->img = img;
	fdf->obj = object; //TODO: might not work cause this might just be the object variable that's in local scope
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
	fdf->view[X] = 0;
	fdf->view[Y] = 0;
	calc_matrices(fdf);
	printf("matrices %f %f %f \n", fdf->matrices[Y][X][X], fdf->matrices[Y][X][Y], fdf->matrices[Y][X][Z]);
	return (1);
}

void	fdf_update(t_fdf *fdf)
{
	int	i;

	i = 0;
	calc_matrices(fdf);
	while (i < fdf->obj->v_count)
	{
		fdf->verts[i][X] = (float)fdf->obj->verts[i][X];
		fdf->verts[i][Y] = (float)fdf->obj->verts[i][Y];
		fdf->verts[i][Z] = (float)fdf->obj->verts[i][Z];
		v3_mul(fdf->matrices[X], fdf->verts[i]);
		v3_mul(fdf->matrices[Y], fdf->verts[i]);
		v3_add(fdf->verts[i], (float [3]) {58, 58, 0});
		i++;
	}
	//ft_bzero(fdf->img->data_addr, fdf->img->size[X] * fdf->img->size[Y] * sizeof(int));
	fdf_drawskeleton(*fdf);
}
