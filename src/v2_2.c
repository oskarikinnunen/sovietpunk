/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 12:13:02 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 00:07:56 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

/*bool	v2cmp(int v[2], int ov[2])
{
	return (v[X] == ov[X] && v[Y] == ov[Y]);
}*/

void	v2clamp_xy(int v[2], int min, int max)
{
	v[X] = ft_clamp(v[X], min, max);
	v[Y] = ft_clamp(v[Y], min, max);
}

void	v2cpy(int to[2], int from[2]) 
{
	to[X] = from[X];
	to[Y] = from[Y];
}

void	v2div(int v[2], int div)
{
	if (div == 0)
		return ;
	v[X] /= div;
	v[Y] /= div;
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