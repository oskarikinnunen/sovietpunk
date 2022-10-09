/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP1947_MAPED.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:37:07 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/09 16:27:02 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP1947_MAPED_H
# define SP1947_MAPED_H

# define TILESIZE 24
# define TILESIZE2 32

typedef struct s_mapeddata
{
	int				cursor[2];
	int				mapsize[2];
	int				cursoritem;
	int				mousepos[2];
	int				mouseheld;
	int				wall_select;
	u_int32_t		mapdata[MAPSIZE * MAPSIZE];
} t_mapeddata;

/* editor_eventloop.c */
int	ed_eventloop(t_mapeddata *ed);

/* editor_buttons.c */
void	draw_buttons(t_sdlcontext *sdl, t_mapeddata ed);
int		hit_button(t_mapeddata *ed);

#endif