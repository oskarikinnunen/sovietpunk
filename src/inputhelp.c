/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:14:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 16:13:17 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

bool	iskey(SDL_Event e, int keycode)
{
	return (e.key.keysym.sym == keycode);
}

bool	keyismoveleft(SDL_Event e)
{
	return (iskey(e, SDLK_a));
}

bool	keyismoveright(SDL_Event e)
{
	return (iskey(e, SDLK_d));
}

bool	keyismoveup(SDL_Event e)
{
	return (iskey(e, SDLK_UP) || iskey(e, SDLK_w));
}

bool	keyismovedown(SDL_Event e)
{
	return (iskey(e, SDLK_DOWN) || iskey(e, SDLK_s));
}
