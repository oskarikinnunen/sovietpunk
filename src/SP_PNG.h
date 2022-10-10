/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP_PNG.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:11:21 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/10 11:55:26 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP_PNG_H
# define SP_PNG_H

# include "SP1947.h"

typedef struct s_pngpalette
{
	Uint64	*plte;
	Uint32	length;
}	t_pngpalette;

typedef struct s_pngdata
{
	Uint32			size[2];
	Uint8			*data;
	Uint8			bitdepth;
	Uint8			colortype;
	t_pngpalette	palette;
}	t_pngdata;

/* parses a png image */
void	pngparse(t_pngdata *data, char *filename);

#endif