/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:57:09 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/02 04:46:55 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	update_deltatime(t_clock *c)
{
	Uint32	time_now;

	time_now = SDL_GetTicks();
	c->delta = time_now - c->prev_time;
	c->prev_time = time_now;
	#ifdef FPSCOUNTER
		printf("FPS: %i\n", 1000 / c->delta);
	#endif
}