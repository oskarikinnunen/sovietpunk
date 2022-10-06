/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor_eventloop.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/25 18:46:26 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/06 14:50:05 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SP1947.h"
#include "SP1947_MAPED.h"

//TODO: render minitexture for walls, indicate enemies and player with just a different color
int	pollitemid(SDL_Event *e)
{
	int	id;

	id = 0;
	printf("\e[1;1H\e[2J");
	while (1)
	{
		SDL_PollEvent(e);
		if (e->type != SDL_KEYDOWN)
			continue ;
		id -= e->key.keysym.sym == SDLK_LEFT;
		id += e->key.keysym.sym == SDLK_RIGHT;
		id = ft_clamp(id, 0, 6); //TODO: clamp to max_item_id
		printf("\e[1;1H\e[2J");
		printf("ITEM ID == %i (SPACE/ENTER TO CONFIRM)\n", id);
		if (e->key.keysym.sym == SDLK_RETURN ||
			e->key.keysym.sym == SDLK_SPACE)
			break ;
	}
	return (id);
}

void	placeitem(t_mapeddata ed)
{
	int i;

	i = 0;
	while (ed.cursoritem == 3 && i++ < MAPSIZE * MAPSIZE)
		ed.mapdata[i] = (ed.mapdata[i] != 3) * ed.mapdata[i];
	ed.mapdata[ed.cursor[X] + ed.cursor[Y] * MAPSIZE] = ed.cursoritem;
}

int	ed_eventloop(t_mapeddata *ed)
{
	static SDL_Event	event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_KEYDOWN)
		{
			printf("\e[1;1H\e[2J");
			ed->cursor[X] -= event.key.keysym.sym == SDLK_LEFT;
			ed->cursor[X] += event.key.keysym.sym == SDLK_RIGHT;
			ed->cursor[Y] -= event.key.keysym.sym == SDLK_UP;
			ed->cursor[Y] += event.key.keysym.sym == SDLK_DOWN;
			ed->cursor[X] = ft_clamp(ed->cursor[X], 0, MAPSIZE - 1);
			ed->cursor[Y] = ft_clamp(ed->cursor[Y], 0, MAPSIZE - 1);
			if (event.key.keysym.sym == SDLK_SPACE)
				ed->cursoritem = pollitemid(&event);
			if (event.key.keysym.sym == SDLK_RETURN)
				placeitem(*ed);
			printf("PLACING ITEM %i\nCURRENT TILE == %i\n",
				ed->cursoritem,
				ed->mapdata[ed->cursor[X] + ed->cursor[Y] * MAPSIZE]);
		}
		if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
			return (1);
	}
	return (0);
}