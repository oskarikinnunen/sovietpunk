/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inputhelp_turn.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:14:20 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/11 16:13:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"

bool	keyisturnright(SDL_Event e)
{
	return (iskey(e, SDLK_RIGHT));
}

bool	keyisturnleft(SDL_Event e)
{
	return (iskey(e, SDLK_LEFT));
}
