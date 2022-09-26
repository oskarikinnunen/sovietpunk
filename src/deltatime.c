/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:57:09 by okinnune          #+#    #+#             */
/*   Updated: 2022/09/26 21:00:55 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	update_deltatime(t_clock *c)
{
	Uint32			time_now;
	static	Uint32	prev_fpstime;

	time_now = SDL_GetTicks();
	c->delta = time_now - c->prev_time;
	c->prev_time = time_now;
	#ifdef FPSCOUNTER
	
	if (time_now >= prev_fpstime + 1000) {
		printf("FPS: %i\n", 1000 / c->delta);
		prev_fpstime = time_now;
	}
	#endif
}