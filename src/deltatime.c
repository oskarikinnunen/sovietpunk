/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deltatime.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 20:57:09 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 16:22:16 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

void	update_deltatime(t_clock *clock)
{
	uint32_t		time_now;
	static uint32_t	prev_fpstime;
	static uint32_t	add;
	static uint32_t	betweenframes;

	time_now = SDL_GetTicks();
	clock->delta = time_now - clock->prev_time;
	clock->prev_time = time_now;
	add += clock->delta;
	betweenframes++;
	if (time_now >= prev_fpstime + 1000)
	{
		ft_putstr(CLRSCR);
		ft_putstr("FPS: ");
		ft_putnbr(1000 / (add / betweenframes));
		add = 0;
		betweenframes = 0;
		prev_fpstime = time_now;
	}
}
