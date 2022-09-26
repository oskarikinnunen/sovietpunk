/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:26:45 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/26 17:48:57 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

u_int32_t	shade(u_int32_t clr, int wallheight)
{
	float		mul;
	uint32_t	final;

	mul = (float)wallheight / DARKNESS;
	if (mul > 1.0f)
		mul = 1.0f;
	final = (clr & 0xFF) * mul;
	final += (uint32_t)((clr >> 8 & 0xFF) * mul) << 8;
	final += (uint32_t)((clr >> 16 & 0xFF) * mul * mul) << 16;
	return (final);
}
