/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v3_basics.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/26 17:31:46 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 12:10:04 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../v3lib.h"

float	*v3_new(float x, float y, float z)
{
	float	*v3;

	v3 = (float *)malloc(sizeof(float) * 3);
	v3[X] = x;
	v3[Y] = y;
	v3[Z] = z;
	return (v3);
}

void	v3_add_xyz(float *v3, float *add)
{
	v3[X] += add[X];
	v3[Y] += add[Y];
	v3[Z] += add[Z];
}

void	v3_mul_matrix(float matrix[3][3], float *v3)
{
	float	t[3];
	int		index;

	index = X;
	while (index <= Z)
	{
		t[index]
			= (v3[X] * matrix[0][index])
			+ (v3[Y] * matrix[1][index])
			+ (v3[Z] * matrix[2][index]);
		index++;
	}
	v3[X] = t[X];
	v3[Y] = t[Y];
	v3[Z] = t[Z];
}

//TODO: consistent naming
void	v3listmul_matrix(float matrix[3][3], float **v3s, int len)
{
	while (len >= 0)
		v3_mul_matrix(matrix, v3s[len--]);
}

void	v3listadd_xyz(float **v3s, float *add, int len)
{
	while (len >= 0)
		v3_add_xyz(v3s[len--], add);
}
