/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_buttons.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 09:00:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/08 09:36:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"

//4 buttons, each with x, y and size (int [3])
void	draw_buttons(t_sdlcontext sdl, int select)
{
	static int	btns[4][3] = 
	{
		{CENTER_X, CENTER_Y - TILESIZE2, TILESIZE2},
		{CENTER_X - TILESIZE2, CENTER_Y, TILESIZE2},
		{CENTER_X + TILESIZE2, CENTER_Y, TILESIZE2},
		{CENTER_X, CENTER_Y + TILESIZE2, TILESIZE2}
	};
	int i;

	i = 0;
	while (i < 4)
	{
		if (i == select)
			drawrect(sdl.surface->pixels, btns[i], CLR_PRPL, btns[i][2]);
		else
			drawrect(sdl.surface->pixels, btns[i], CLR_GRAY, btns[i][2]);
		i++;
	}
}

static bool pointinsquare(int pos[2], int square[3])
{
	return (pos[X] >= square[X] && pos[X] < square[X] + square[2]
		&& pos[Y] >= square[Y] && pos[Y] < square[Y] + square[2]);
}

int	hit_button(t_mapeddata *ed)
{
	static int	btns[4][3] = 
	{
		{CENTER_X, CENTER_Y - TILESIZE2, TILESIZE2},
		{CENTER_X - TILESIZE2, CENTER_Y, TILESIZE2},
		{CENTER_X + TILESIZE2, CENTER_Y, TILESIZE2},
		{CENTER_X, CENTER_Y + TILESIZE2, TILESIZE2}
	};
	int	i;

	i = 0;
	while (i < 4)
	{
		if (pointinsquare(ed->mousepos, btns[i]))
			return (i);
		i++;
	}
	return -1;
}
