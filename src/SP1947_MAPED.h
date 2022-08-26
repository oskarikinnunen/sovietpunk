/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP1947_MAPED.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <eino.oskari.kinnunen@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:37:07 by okinnune          #+#    #+#             */
/*   Updated: 2022/08/25 19:56:10 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP1947_MAPED_H
# define SP1947_MAPED_H

# define TILESIZE 16

typedef struct s_mapeddata
{
	int				cursor[2];
	int				mapsize[2];
	int				cursoritem;
	u_int32_t		*mapdata;
} t_mapeddata;

/* editor_eventloop.c */
int	ed_eventloop(t_mapeddata *ed);

#endif