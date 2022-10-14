/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_eventloop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 18:46:26 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 15:23:11 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
//#include "inputhelp.h"

static void	get_item_id(int *itemid, int wallselect, int dir)
{
	static int	selector;

	selector += dir;
	selector = ft_clamp(selector, 0, PNG_COUNT - 1);
	if (wallselect == -1)
	{
		*itemid = 0;
		*itemid |= selector << (NORTHWALL * 8);
		*itemid |= selector << (WESTWALL * 8);
		*itemid |= selector << (EASTWALL * 8);
		*itemid |= selector << (SOUTHWALL * 8);
	}
	else
	{
		*itemid &= ~(0xFF << (wallselect * 8));
		*itemid |= selector << (wallselect * 8);
	}
}

void	removeitem(t_mapeddata *ed)
{
	ed->mapdata[ed->cursor[X] + ed->cursor[Y] * MAPSIZE] = 0;
}

void	placeitem(t_mapeddata *ed)
{
	int	i;

	i = 0;
	if (samplemap(ed->mapdata, ed->cursor) == ed->cursoritem)
		return ;
	ed->mapdata[ed->cursor[X] + (ed->cursor[Y] * MAPSIZE)] = ed->cursoritem;
}

static void	mouse_events(t_mapeddata *ed, SDL_Event event)
{
	if (event.type == SDL_MOUSEWHEEL)
		get_item_id(&ed->cursoritem, ed->wall_select, event.wheel.y);
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		ed->wall_select = hit_button(ed);
		ed->mouseheld = (event.button.button) * (ed->wall_select == -1);
	}
	if (event.type == SDL_MOUSEBUTTONUP)
		ed->mouseheld = 0;
	if (ed->mouseheld == SDL_BUTTON_LEFT)
		placeitem(ed);
	if (ed->mouseheld == SDL_BUTTON_RIGHT)
		removeitem(ed);
}

int	ed_eventloop(t_mapeddata *ed)
{
	static SDL_Event	event;

	SDL_GetMouseState(&ed->mousepos[X], &ed->mousepos[Y]);
	ed->cursor[X] = ((float)ed->mousepos[X] / ((float)TILESIZE + 1));
	ed->cursor[Y] = ((float)ed->mousepos[Y] / ((float)TILESIZE + 1));
	v2clamp_xy(ed->cursor, 0, MAPSIZE - 1);
	while (SDL_PollEvent(&event) != 0)
	{
		mouse_events(ed, event);
		if (event.type == SDL_KEYDOWN && iskey(event, SDLK_ESCAPE))
			return (1);
		else if (event.type == SDL_QUIT)
			return (1);
	}
	return (0);
}
