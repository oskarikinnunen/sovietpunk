/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   v2.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/29 20:22:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/29 20:22:27 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int v2dist(int *v, int *ov)
{
	return (
			sqrt(
			(v[X] - ov[X]) * (v[X] - ov[X]) +
			(v[Y] - ov[Y]) * (v[Y] - ov[Y])
			)
			);
}