/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 20:22:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 20:38:00 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

/* FLOATING POINT VECTORS */

void	f2tov2(float f[2], int v[2])
{
	v[X] = (int)f[X];
	v[Y] = (int)f[Y];
}

void	f2cpy(float to[2], float from[2])
{
	to[X] = from[X];
	to[Y] = from[Y];
}

void	f2mul(float f[2], float mul)
{
	f[X] *= mul;
	f[Y] *= mul;
}

//Adds right side to left side
void	f2add(float f[2], float of[2])
{
	f[X] += of[X];
	f[Y] += of[Y];
}

int v2dist(int *v, int *ov)
{
	return (
			sqrt(
			(v[X] - ov[X]) * (v[X] - ov[X]) +
			(v[Y] - ov[Y]) * (v[Y] - ov[Y])
			)
			);
}