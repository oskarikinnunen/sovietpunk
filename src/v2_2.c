/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 12:13:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/07 12:17:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

/*bool	v2cmp(int v[2], int ov[2])
{
	return (v[X] == ov[X] && v[Y] == ov[Y]);
}*/

void	v2cpy(int to[2], int from[2]) 
{
	to[X] = from[X];
	to[Y] = from[Y];
}

void	v2mul_to_f2(int v[2], float mul, float f[2])
{
	f[X] = (float)v[X] * mul;
	f[Y] = (float)v[Y] * mul;
}

//takes difference of v and ov and stores it in rv
// rv = result vector
void	v2diff(int v[2], int ov[2], int rv[2])
{
	rv[X] = v[X] - ov[X];
	rv[Y] = v[Y] - ov[Y];
}

void	v2mul(int v[2], int mul)
{
	v[X] *= mul;
	v[Y] *= mul;
}

//Adds right side to left side
void	v2add(int v[2], int ov[2])
{
	v[X] += ov[X];
	v[Y] += ov[Y];
}