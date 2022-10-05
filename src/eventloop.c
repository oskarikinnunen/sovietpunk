/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventloop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 23:03:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/05 17:49:19 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

static int	iskey(SDL_Event e, int keycode)
{
	return (e.key.keysym.sym == keycode);
}

int	ismovement(SDL_KeyCode code)
{
	return (code == SDLK_LEFT || code == SDLK_RIGHT || code == SDLK_UP || code == SDLK_DOWN) ||
			(code == SDLK_a || code == SDLK_d);
}

void	playerinput(SDL_Keycode kc, t_player *plr)
{
	float	angle;
	plr->rot = (kc == SDLK_LEFT) * TURNSPEED;
	plr->rot -= (kc == SDLK_RIGHT) * TURNSPEED;
	angle = RAYSLICE * (WINDOW_W / 2);
	if (kc == SDLK_UP)
	{
		plr->dest[X] = sin(plr->angle + angle); //Have plr destination variable and move player with bresenham?? must be a simpler way to do it tho.
		plr->dest[Y] = cos(plr->angle + angle);
	}
	if (kc == SDLK_DOWN)
	{
		plr->dest[X] = -sin(plr->angle + angle);
		plr->dest[Y] = -cos(plr->angle + angle);
	}
	if (kc == SDLK_a)
	{
		plr->dest[X] = -sin(plr->angle - RAD90 + angle);
		plr->dest[Y] = -cos(plr->angle - RAD90 + angle);
	}
	if (kc == SDLK_d)
	{
		plr->dest[X] = sin(plr->angle - RAD90 + angle);
		plr->dest[Y] = cos(plr->angle - RAD90 + angle);
	}
	plr->dest[X] *= MOVESPEED;
	plr->dest[Y] *= MOVESPEED;
}

int	eventloop(t_gamecontext *gc)
{
	static SDL_Event	event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_KEYDOWN)
		{
			if (iskey(event, SDLK_SPACE))
			{
				gc->player.pos[X] = 3;
				gc->player.pos[Y] = 6;
			}
			if (ismovement(event.key.keysym.sym))
				playerinput(event.key.keysym.sym, &gc->player);
			if (iskey(event, SDLK_ESCAPE))
				return (1);
		}
		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT)
				gc->player.rot = 0;
			if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN)
				ft_bzero(gc->player.dest, sizeof(float[2]));
		}
		if (event.type == SDL_QUIT)
			return (1);
	}
	return (0);
}