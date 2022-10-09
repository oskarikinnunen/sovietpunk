/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:57:09 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 21:12:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"



void	update_deltatime(t_clock *c)
{
	Uint32			time_now;
	static Uint32	prev_fpstime;
	static Uint32	add;
	static Uint32	betweenframes;
	static char	fps[4];

	time_now = SDL_GetTicks();
	c->delta = time_now - c->prev_time;
	c->prev_time = time_now;
	add += c->delta;
	betweenframes++;
	#ifdef FPSCOUNTER
	
	if (time_now >= prev_fpstime + 1000)
	{
		printf("fps: %i\n",1000 / (add / betweenframes));
		add = 0;
		betweenframes = 0;
		prev_fpstime = time_now;
	}
	#endif
}