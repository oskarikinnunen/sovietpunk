/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_fdf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:09:31 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/15 19:51:35 by okinnune         ###   ########.fr       */
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

void	draw_line(t_fdf *fdf, t_bresenham b, float z, uint32_t c)
{
	unsigned int	offset;
	int				b_res;
	//t_fdf			*fdf;
	//TODO: finish lol
	b_res = 0;
	//
	//printf("b %i %i trgt: %i %i \n", b.local[X], b.local[Y], b.target[X], b.target[Y]);
	while (b_res != 1)
	{
		offset = b.local[X] + (b.local[Y] * fdf->img->size[X]);
		if (offset < fdf->img->length && fdf->depth[offset] >= z)
			fdf->img->data[offset] = c;
			//put this z in depth and color the pixel
		
		
		/*if (offset < si->length)
			si->data[offset] = c;*/
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
	int		split[3]; 	//Vector that's used to form the subtriangles.
	int		sorted[3][3];	//Stack memory for the sorted triangles
	float	lerp;
	t_bresenham	b;

	sort_tris(ft_memcpy(sorted, tri, sizeof(int [3][3])));
	lerp = (float)(sorted[1][Y] - sorted[2][Y]) / (float)(sorted[0][Y] - sorted[2][Y]);
	//lerp = +0.00000001f;
	split[X] = sorted[2][X] + (lerp * (sorted[0][X] - sorted[2][X]));
	split[Y] = sorted[1][Y];
	//printf("split X %i \n", split[X]);
	split[Z] = sorted[1][Z];
	fill_sub_tri((int *[3]){(int *)&(sorted[0]), (int *)&(sorted[1]), (int *)&split}, fdf, z, c);
	fill_sub_tri((int *[3]){(int *)&(sorted[2]), (int *)&(sorted[1]), (int *)&split}, fdf, z, c);
	/*populate_bresenham(&b, sorted[0], split);
	draw_line(fdf, b, z, c);*/
}

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

void	fdf_draw(t_fdf fdf)
{
	int	i;
	int	i3[3][3];
	t_bresenham	b;
	uint32_t	c;

	i = 0;
	ft_bzero(fdf.img->data, fdf.img->length * sizeof(Uint32));
	ft_bzero(fdf.depth, sizeof(float) * fdf.img->length);
	printf("verts %i faces %i \n", fdf.obj->v_count, fdf.obj->f_count);
	while (i < fdf.obj->f_count)
	{
		if (fdf.obj->colors[i] != 0) {
			c = fdf.obj->mtlcolors[fdf.obj->colors[i] - 1];
			int cr = (c & 0xFF);
			int cg = (c >> 8 & 0xFF);
			int cb = (c >> 16 & 0xFF);
			c = (cb & 0xFF) + (cg << 8) + (cr << 16);
		}
		//get z depth as float, pass to fill_tri and drawline? bitshift to color
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][0]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][1]], i3[1]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][2]], i3[2]);

		//float **z = &(fdf.verts[fdf.obj->faces[i][0]]);
		
		//float z = -(i3[0][Z]) - 1000.0f;
		
		float z = -fdf.verts[fdf.obj->faces[i][0]][Z];
		//z -= +300.0f;
		c = (int)(z / 1.3f) & 0xFF;
		printf("z %f \n", z);
		z = -1000.0f;
		/*if (z <= -1020.0f)
			c = INT_MAX;*/
		fill_tri(i3, &fdf, z, c);

		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][0]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][1]], i3[1]);
		populate_bresenham(&b, i3[0], i3[1]);
		draw_line(&fdf, b, 0, c);
		

		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][1]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][2]], i3[1]);
		populate_bresenham(&b, i3[0], i3[1]);
		draw_line(&fdf, b, 0, c);

		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][2]], i3[0]);
		fv3_to_iv3(fdf.verts[fdf.obj->faces[i][0]], i3[1]);
		populate_bresenham(&b, i3[0], i3[1]);
		draw_line(&fdf, b, 0, c);

		i++;
	}
}

static void	calc_matrices(t_fdf *fdf)
{
	float	angles[2];

	ft_bzero(fdf->matrices, sizeof(float [2][3][3]));
	//angles[X] = ft_degtorad(fdf->view[Y]);
	angles[X] = fdf->view[Y];
	printf("angle %f \n", angles[X]);
	//doesn't work when abs(angle) < PI / 2

	fdf->matrices[0][X][X] = cos(angles[X]);
	
	fdf->matrices[0][Y][X] = sin(angles[X]);
	
	fdf->matrices[0][Z][Y] = 1.0;

	//These only affect z
	//fdf->matrices[0][X][Z] = 1.0f;
	fdf->matrices[0][Z][Z] = 1.0f;
	//fdf->matrices[0][Y][Z] = sin(angles[X]);
	//fdf->matrices[0][X][Z] = -cos(angles[X]);

	angles[Y] = ft_degtorad(fdf->view[X]);
	

	/*
	fdf->matrices[1][X][X] = 1.0f;
	fdf->matrices[1][Y][Y] = cos(angles[Y]);
	fdf->matrices[1][Y][Z] = sin(angles[Y]);
	fdf->matrices[1][Z][Y] = sin(angles[Y]);
	fdf->matrices[1][Z][Z] = -cos(angles[Y]);
	*/
	
	//CORRECT ROTATINO, WRONG Z?
	fdf->matrices[1][X][X] = 1.0f;
	fdf->matrices[1][Y][Y] = cos(angles[Y]);

	fdf->matrices[1][Y][Z] = 1.0f;

	//fdf->matrices[1][Z][Y] = sin(angles[Y]);
	fdf->matrices[1][Z][Z] = 1.0f;
	
}

//TODO: fdf_init: allocates the memory and handles errors if that fails
int	fdf_init(t_fdf *fdf, t_simpleimg *img, t_obj *object)
{
	int	i;

	ft_bzero(fdf, sizeof(t_fdf));
	fdf->depth = ft_memalloc(sizeof(float) * img->length);
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
	//printf("matrices %f %f %f \n", fdf->matrices[Y][X][X], fdf->matrices[Y][X][Y], fdf->matrices[Y][X][Z]);
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
		//v3_mul(fdf->matrices[Y], fdf->verts[i]);
		v3_add(fdf->verts[i], (float [3]) {fdf->img->size[X] / 2, 400, 0});
		i++;
	}
	ft_bzero(fdf->img->data, fdf->img->size[X] * fdf->img->size[Y] * sizeof(int));
	fdf_draw(*fdf);
}
