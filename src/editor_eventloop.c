/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_eventloop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 18:46:26 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/08 11:35:55 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"
//#include "inputhelp.h"

//TODO: render minitexture for walls, indicate enemies and player with just a different color
int	get_item_id(int *itemid, int dir)
{
	static int selector;
	selector += dir;
	selector = ft_clamp(selector, 0, PNG_COUNT - 1);
	*itemid = selector;
}

void	removeitem(t_mapeddata ed)
{
	if (samplemap(ed.mapdata, ed.cursor) == 0)
		return ;
	ed.mapdata[ed.cursor[X] + ed.cursor[Y] * MAPSIZE] = 0;
}

void	placeitem(t_mapeddata ed)
{
	int i;

	i = 0;
	if (samplemap(ed.mapdata, ed.cursor) == ed.cursoritem)
		return ;
	/*while (ed.cursoritem == TILE_SPAWN && i++ < MAPSIZE * MAPSIZE) // REMOVE SPAWNS
		ed.mapdata[i] = (ed.mapdata[i] != TILE_SPAWN) * ed.mapdata[i];*/
	ed.mapdata[ed.cursor[X] + ed.cursor[Y] * MAPSIZE] = ed.cursoritem;
}

int	ed_eventloop(t_mapeddata *ed)
{
	static SDL_Event	event;

	SDL_GetMouseState(&ed->mousepos[X], &ed->mousepos[Y]);
	ed->cursor[X] = ((float)ed->mousepos[X] / ((float)TILESIZE + 1));
	ed->cursor[Y] = ((float)ed->mousepos[Y] / ((float)TILESIZE + 1));
	v2clamp_xy(ed->cursor, 0, TILESIZE - 1);
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_MOUSEWHEEL)
			get_item_id(&ed->cursoritem, event.wheel.y);
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			ed->wall_select = hit_button(ed);
			printf("wall select %i \n", ed->wall_select);
			ed->mouseheld = (event.button.button) * (ed->wall_select == -1);
		}
		if (event.type == SDL_MOUSEBUTTONUP)
			ed->mouseheld = 0;
		if (ed->mouseheld == SDL_BUTTON_LEFT)
			placeitem(*ed);
		if (ed->mouseheld == SDL_BUTTON_RIGHT)
			removeitem(*ed);
		if (event.type == SDL_KEYDOWN)
		{
			get_item_id(&ed->cursoritem, iskey(event, SDLK_UP));
			get_item_id(&ed->cursoritem, -iskey(event, SDLK_DOWN));
			if (iskey(event, SDLK_ESCAPE))
				return (1);
		}
		else if (event.type == SDL_QUIT)
			return (1);
	}
	return (0);
}