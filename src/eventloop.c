/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventloop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 23:03:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/08 10:48:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

int	ismovement(SDL_KeyCode code)
{
	return (code == SDLK_LEFT || code == SDLK_RIGHT || code == SDLK_UP || code == SDLK_DOWN) ||
			(code == SDLK_a || code == SDLK_d);
}

static int	key_events(SDL_Event e, t_gamecontext *game)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (iskey(e, SDLK_ESCAPE))
			return (1);
		if (iskey(e, SDLK_TAB)) {
			game->relativemousemode = !game->relativemousemode;
			SDL_SetRelativeMouseMode(game->relativemousemode);
		}
		game->keystate |= keyismoveleft(e);
		game->keystate |= keyismoveright(e) << KEYS_RGHTMASK;
		game->keystate |= keyismoveup(e) << KEYS_UPMASK;
		game->keystate |= keyismovedown(e) << KEYS_DOWNMASK;
	}
	else if(e.type == SDL_KEYUP)
	{
		game->keystate &= ~(keyismoveleft(e));
		game->keystate &= ~(keyismoveright(e) << KEYS_RGHTMASK);
		game->keystate &= ~(keyismoveup(e) << KEYS_UPMASK);
		game->keystate &= ~(keyismovedown(e) << KEYS_DOWNMASK);
	}
	return (0);
}

void	playerinput(SDL_Keycode kc, t_player *plr)
{
	float	angle;
	plr->rot = (kc == SDLK_LEFT) * TURNSPEED;
	plr->rot -= (kc == SDLK_RIGHT) * TURNSPEED;
	angle = RAYSLICE * (WINDOW_W / 2);
	if (kc == SDLK_UP)
	{
		plr->dest[X] = sin(plr->angle + angle);
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
	int					key_event_return;

	if (gc->relativemousemode)
		SDL_GetRelativeMouseState(&gc->mouse_delta[X], &gc->mouse_delta[Y]);
	while (SDL_PollEvent(&event) != 0)
	{
		key_event_return = key_events(event, gc);
		if (event.type == SDL_QUIT || key_event_return)
			return (1);
	}
	return (0);
}