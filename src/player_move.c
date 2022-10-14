/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_move.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 13:41:07 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 14:53:33 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

static void	updatemovementvector(float move_f2[2],
	int32_t keystate, float angle)
{
	angle = angle + (FOV / 2.0f);
	if ((keystate >> KEYS_UPMASK) & 1)
	{
		move_f2[X] += sin(angle);
		move_f2[Y] += cos(angle);
	}
	if ((keystate >> KEYS_DOWNMASK) & 1)
	{
		move_f2[X] -= sin(angle);
		move_f2[Y] -= cos(angle);
	}
	if ((keystate >> KEYS_LEFTMASK) & 1)
	{
		move_f2[X] += sin(angle + RAD90);
		move_f2[Y] += cos(angle + RAD90);
	}
	if ((keystate >> KEYS_RGHTMASK) & 1)
	{
		move_f2[X] += -sin(angle + RAD90);
		move_f2[Y] += -cos(angle + RAD90);
	}
}

void	moveplayer(t_gamecontext *gc)
{
	float	move_f2[2];
	float	angle;
	float	potential_plrpos[2];
	int		mapcrd[2];

	ft_bzero(move_f2, sizeof(float [2]));
	angle = 0;
	angle -= gc->mouse_delta[X] * MOUSESPEED;
	angle += ((gc->keystate >> KEYS_TURNLEFTMASK) & 1) * KEYTURNSPEED;
	angle -= ((gc->keystate >> KEYS_TURNRGHTMASK) & 1) * KEYTURNSPEED;
	angle *= gc->clock.delta;
	gc->player.angle += angle;
	updatemovementvector(move_f2, gc->keystate, gc->player.angle);
	f2mul(move_f2, gc->clock.delta * MOVESPEED);
	f2cpy(potential_plrpos, gc->player.pos);
	f2add(potential_plrpos, move_f2);
	f2mul(potential_plrpos, 1.0f / GAMESCALE);
	f2tov2(potential_plrpos, mapcrd);
	if (is_in_map(mapcrd) && samplemap(gc->map, mapcrd) == 0)
		f2add(gc->player.pos, move_f2);
}
