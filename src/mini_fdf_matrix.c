/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_fdf_matrix.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 00:32:19 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 00:33:20 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	calc_matrices(t_fdf *fdf)
{
	float	angles[2];

	ft_bzero(fdf->matrices, sizeof(float [2][3][3]));
	angles[X] = fdf->view[Y];
	fdf->matrices[1][X][X] = cos(angles[X]);
	fdf->matrices[1][Y][X] = sin(angles[X]);
	fdf->matrices[1][Z][Y] = 1.0f;
	fdf->matrices[1][Y][Z] = cos(angles[X]);
	fdf->matrices[1][X][Z] = -sin(angles[X]);
}
