/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_buttons.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 09:00:36 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 19:52:52 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"

//4 buttons, each with x, y and size (int [3])
void	draw_buttons(t_sdlcontext *sdl, t_mapeddata ed)
{
	static int	btns[4][3] = 
	{
		{WINDOW_W - 128, CENTER_Y - TILESIZE2, TILESIZE2},
		{WINDOW_W - 128 - TILESIZE2, CENTER_Y, TILESIZE2},
		{WINDOW_W - 128 + TILESIZE2, CENTER_Y, TILESIZE2},
		{WINDOW_W - 128, CENTER_Y + TILESIZE2, TILESIZE2}
	};
	int	i;

	i = 0;
	while (i < 4)
	{
		drawimagescaled(sdl, btns[i], getindexedwall(ed.cursoritem, i), btns[i][2]);
		if (i == ed.wall_select)
			drawrect(sdl->surface->pixels, btns[i], CLR_PRPL, btns[i][2]);
		else
			drawrect(sdl->surface->pixels, btns[i], CLR_GRAY, btns[i][2]);
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
		{WINDOW_W - 128, CENTER_Y - TILESIZE2, TILESIZE2},
		{WINDOW_W - 128 - TILESIZE2, CENTER_Y, TILESIZE2},
		{WINDOW_W - 128 + TILESIZE2, CENTER_Y, TILESIZE2},
		{WINDOW_W - 128, CENTER_Y + TILESIZE2, TILESIZE2}
	};
	int			i;

	i = 0;
	while (i < 4)
	{
		if (pointinsquare(ed->mousepos, btns[i]))
			return (i);
		i++;
	}
	return -1;
}
