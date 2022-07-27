/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP1947_MAPED.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:37:07 by okinnune          #+#    #+#             */
/*   Updated: 2022/07/27 03:37:07 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP1947_MAPED_H
# define SP1947_MAPED_H

# define TILESIZE 16

typedef struct s_mapeddata
{
	int				cursor[2];
	int				mapsize[2];
	u_int8_t		*mapdata;
} t_mapeddata;

#endif