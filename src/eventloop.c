/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eventloop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 23:03:04 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 12:12:28 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

static void	setkeystates(int32_t *keystate, SDL_Event event)
{
	*keystate |= keyismoveleft(event);
	*keystate |= keyismoveright(event) << KEYS_RGHTMASK;
	*keystate |= keyismoveup(event) << KEYS_UPMASK;
	*keystate |= keyismovedown(event) << KEYS_DOWNMASK;
	*keystate |= keyisturnleft(event) << KEYS_TURNLEFTMASK;
	*keystate |= keyisturnright(event) << KEYS_TURNRGHTMASK;
}

static int	key_events(SDL_Event event, t_gamecontext *game)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (iskey(event, SDLK_ESCAPE))
			return (1);
		if (iskey(event, SDLK_TAB))
		{
			game->relativemousemode = !game->relativemousemode;
			SDL_SetRelativeMouseMode(game->relativemousemode);
		}
		setkeystates(&game->keystate, event);
	}
	else if (event.type == SDL_KEYUP)
	{
		game->keystate &= ~(keyismoveleft(event));
		game->keystate &= ~(keyismoveright(event) << KEYS_RGHTMASK);
		game->keystate &= ~(keyismoveup(event) << KEYS_UPMASK);
		game->keystate &= ~(keyismovedown(event) << KEYS_DOWNMASK);
		game->keystate &= ~(keyisturnleft(event) << KEYS_TURNLEFTMASK);
		game->keystate &= ~(keyisturnright(event) << KEYS_TURNRGHTMASK);
	}
	return (0);
}

static void	strafe(SDL_Keycode keycode, t_player *plr, float angle)
{
	if (keycode == SDLK_a)
	{
		plr->dest[X] = -sin(plr->angle - RAD90 + angle);
		plr->dest[Y] = -cos(plr->angle - RAD90 + angle);
	}
	if (keycode == SDLK_d)
	{
		plr->dest[X] = sin(plr->angle - RAD90 + angle);
		plr->dest[Y] = cos(plr->angle - RAD90 + angle);
	}
}

void	playerinput(SDL_Keycode keycode, t_player *plr)
{
	float	angle;

	plr->rot = (keycode == SDLK_LEFT) * KEYTURNSPEED;
	plr->rot -= (keycode == SDLK_RIGHT) * KEYTURNSPEED;
	angle = RAYSLICE * (WINDOW_W / 2);
	if (keycode == SDLK_UP)
	{
		plr->dest[X] = sin(plr->angle + angle);
		plr->dest[Y] = cos(plr->angle + angle);
	}
	if (keycode == SDLK_DOWN)
	{
		plr->dest[X] = -sin(plr->angle + angle);
		plr->dest[Y] = -cos(plr->angle + angle);
	}
	strafe(keycode, plr, angle);
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
