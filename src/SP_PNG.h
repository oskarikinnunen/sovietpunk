/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SP_PNG.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:11:21 by okinnune          #+#    #+#             */
/*   Updated: 2022/10/12 12:22:55 by okinnune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SP_PNG_H
# define SP_PNG_H

# include "SP1947.h"

typedef struct s_pngpalette
{
	uint64_t	*plte;
	uint32_t	length;
}	t_pngpalette;

typedef struct s_pngdata
{
	uint32_t		size[2];
	uint8_t			*data;
	uint8_t			bitdepth;
	uint8_t			colortype;
	t_pngpalette	palette;
}	t_pngdata;

/* parses a png image */
void	pngparse(t_pngdata *data, char *filename);

#endif