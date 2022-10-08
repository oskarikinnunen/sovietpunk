/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shade.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/26 17:26:45 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/08 11:51:25 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

//# define SHADETRIPPY

#ifdef SHADE1

u_int32_t	shade(u_int32_t clr, int wallheight)
{
	float		mul;
	uint32_t	final;

	//return (clr);
	mul = (float)wallheight / DARKNESS;
	if (mul > 1.0f)
		mul = 1.0f;
	mul *= mul;
	final = (clr & 0xFF) * mul;
	final += (uint32_t)((clr >> 8 & 0xFF) * mul * mul)  << 8;
	final += (uint32_t)((clr >> 16 & 0xFF) * mul * mul) << 16;
	return (final);
}

#elif SHADETRIPPY

static u_int8_t color_intensity(u_int32_t clr)
{
	uint32_t	combine;

	combine = 0;
	combine += (clr & 0xFF);
	combine += (clr >> 8 & 0xFF);
	combine += (clr >> 16 & 0xFF);
	combine /= 3;
	return ((u_int8_t)combine);
}

u_int32_t	shade(u_int32_t clr, int wallheight)
{
	float		mul;
	uint32_t	final;
	uint8_t		intensity;
	const int	div = 15;

	//return (clr);

	intensity = color_intensity(clr);
	if (intensity < 33)
		return (1);
	mul = (float)wallheight / DARKNESS;
	if (mul > 1.0f)
		mul = 1.0f;
	final = (((clr & 0xFF) / div) * div * mul);
	mul *= mul;
	final += (uint32_t)(((clr >> 8 & 0xFF) / div) * div * mul) << 8;
	final += (uint32_t)(((clr >> 16 & 0xFF) / div) * div * mul) << 16;
	return (final);
}

#else

u_int32_t	shade(u_int32_t clr, int wallheight)
{
	float		mul;
	uint32_t	final;

	//return (clr);
	mul = (float)wallheight / DARKNESS;
	if (mul > 1.0f)
		mul = 1.0f;
	final = (clr & 0xFF) * mul;
	final += (uint32_t)((clr >> 8 & 0xFF) * mul) << 8;
	final += (uint32_t)((clr >> 16 & 0xFF) * mul * mul) << 16;
	return (final);
}

#endif


u_int32_t	vanilla_shade(u_int32_t clr, int wallheight)
{
	float		mul;
	uint32_t	final;

	//return (clr);
	mul = (float)wallheight / DARKNESS;
	if (mul > 1.0f)
		mul = 1.0f;
	final = (clr & 0xFF) * mul;
	final += (uint32_t)((clr >> 8 & 0xFF) * mul) << 8;
	final += (uint32_t)((clr >> 16 & 0xFF) * mul * mul) << 16;
	return (final);
}